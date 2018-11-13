#include <stdint.h>
#include <stdio.h>
#include "plugins.h"

// get export definitions for:
// set_atomic_cb, set_inst_cb, set_int_cb, run, run_cpu
// set_mem_cb, set_magic_cb, set_io_cb, set_reg_cb,
// set_trans_cb, set_gen_cbs, set_sys_cbs, qemu_init, 
#include "../../../qsim-func.h"

// need for qsim_ucontext_t used in run and run_cpu
#include "../../../qsim-context.h"

// remaining set of function definitions needed
int interrupt(uint8_t vec);

uint64_t get_reg(int cpu_idx, int r) {
    void * cpu = getCPUStateFromId(cpu_idx);
    uint32_t reg;
    qemulib_read_register(cpu, (uint8_t*)&reg, r);
    return reg;
}
void set_reg(int cpu_idx, int r, uint64_t val) {
    void* cpu = getCPUStateFromId(cpu_idx);
    qemulib_write_register(cpu, &val, r);
}

uint8_t mem_rd(uint64_t paddr) {
    void *cpu = NULL;
    qemulib_translate_memory(cpu, paddr); 
}
void mem_wr(uint64_t paddr, uint8_t val) {
    void *cpu = NULL;
    qemulib_write_memory(cpu, paddr, uint8_t *buf, int len)

}

uint8_t mem_rd_virt(int cpu_idx, uint64_t vaddr) {return 0; }
void mem_wr_virt(int cpu_idx, uint64_t vaddr, uint8_t val);
void qsim_savevm_state(const char *filename) {}
int qsim_loadvm_state(const char *filename) {return 0; }

interupt_cb_t qsim_interupt_cb = NULL;
atomic_cb_t qsim_atomic_cb = NULL;
magic_cb_t  qsim_magic_cb  = NULL;
int_cb_t    qsim_int_cb    = NULL;
mem_cb_t    qsim_mem_cb    = NULL;
inst_cb_t   qsim_inst_cb   = NULL;
io_cb_t     qsim_io_cb     = NULL;
reg_cb_t    qsim_reg_cb    = NULL;
trans_cb_t qsim_trans_cb   = NULL;
int qsim_gen_callbacks = 0;
bool qsim_sys_callbacks = false;
qsim_ucontext_t main_context;
qsim_ucontext_t qemu_context;
int qsim_id;
// -1: not yet called
//  0: Total system run mode
//  1: Per cpu run mode
int run_mode = -1;

int64_t     qsim_icount   = 10000000;


void set_atomic_cb(atomic_cb_t cb) { qsim_atomic_cb = cb; }
void set_inst_cb  (inst_cb_t   cb) { qsim_inst_cb   = cb; }
void set_int_cb   (int_cb_t    cb) { qsim_int_cb    = cb; }
void set_mem_cb   (mem_cb_t    cb) { qsim_mem_cb    = cb; }
void set_magic_cb (magic_cb_t  cb) { qsim_magic_cb  = cb; }
void set_io_cb    (io_cb_t     cb) { qsim_io_cb     = cb; }
void set_reg_cb   (reg_cb_t    cb) { qsim_reg_cb    = cb; }
void set_trans_cb (trans_cb_t cb) { qsim_trans_cb = cb; }
void set_interupt_cb(interupt_cb_t cb) { qsim_interupt_cb = cb;}


void set_gen_cbs (bool state)
{
    // Mark all generated TBs as stale so that new TBs are generated
    if (state) {
        qsim_gen_callbacks++;
    } else {
        qsim_gen_callbacks--;
    }
}

// Generate callbacks for system/application instructions only
void set_sys_cbs  (bool state) {
  qsim_sys_callbacks = state;
}



uint64_t run(uint64_t insts)
{
    run_mode = 0;

    qsim_icount = insts;

    swapcontext(&main_context, &qemu_context);
    checkcontext();

    return insts - qsim_icount;
}

uint64_t run_cpu(int cpu_id, uint64_t insts)
{
    run_mode = 1;

    qsim_icount = insts;
    qsim_id = cpu_id;
    swapcontext(&main_context, &qemu_context);
    checkcontext();

    return insts - qsim_icount;
}


void test_inst_cb(int c, uint64_t v, uint64_t p, uint8_t l,
             const uint8_t *b,  enum inst_type type)
{
    fprintf(stderr, "executing instruction at pc: %lx\n", v);
    return;
}

void test_mem_cb(int c, uint64_t v, uint64_t p, int size, int w)
{
    fprintf(stderr, "core: %d, vaddr: %lx, paddr: %lx, \
                     size: %d, write: %d\n", c, v, p, size, w);

    return;
}


bool plugin_init(const char *args)
{
    set_inst_cb(test_inst_cb); //test we did not break
    set_mem_cb(test_mem_cb); //test we did not break
    return true;
}

extern bool enable_instrumentation;

bool plugin_needs_before_insn(uint64_t pc, void *cpu)
{
    if (enable_instrumentation) {
        return true;
    }

    return false;
}

void plugin_before_insn(uint64_t pc, void *cpu)
{
    uint8_t inst_buffer[4];

    qemulib_read_memory(cpu, pc, inst_buffer, sizeof(uint32_t));
    qsim_inst_cb(qemulib_get_cpuid(cpu), pc, qemulib_translate_memory(cpu, pc),
            sizeof(uint32_t), inst_buffer, QSIM_INST_NULL);
}

void plugin_after_mem(void *cpu, uint64_t v, int size, int type)
{
    qsim_mem_cb(qemulib_get_cpuid(cpu), v,
           qemulib_translate_memory(cpu, v), size, type);
}


