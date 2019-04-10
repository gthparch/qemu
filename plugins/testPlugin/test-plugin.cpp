#include "qsim.h"
#include <stdint.h>
#include <stdio.h>

Qsim::QemuCpu cpu;

void test_inst_cb(int c, uint64_t v, uint64_t p, uint8_t l,
             const uint8_t *b,  enum inst_type type)
{
    FILE *insFile = fopen ("inst.log","a");
    //fprintf(stderr, "executing instruction at pc: %lx\n", v);
    fprintf(insFile, "executing instruction at pc: %lx\n", v);
    fclose(insFile);
    return;
}

void test_mem_cb(int c, uint64_t v, uint64_t p, uint8_t size, int w)
{
     FILE *memFile = fopen ("mem.log","a");
    //fprintf(stderr, "core: %d, vaddr: %lx, paddr: %lx, \
    //                 size: %d, write: %d\n", c, v, p, size, w);

    fprintf(memFile, "core: %d, vaddr: %lx, paddr: %lx, \
                     size: %d, write: %d\n", c, v, p, size, w);
    fclose(memFile);
    return;
}

bool plugin_init(const char *args)
{
    FILE *insFile = fopen ("inst.log","w");
    FILE *memFile = fopen ("mem.log","w");
    fprintf (insFile, "");
    fprintf (memFile, "");
    fclose(insFile);
    fclose(memFile);
    cpu.set_inst_cb(test_inst_cb); //test we did not break
    cpu.set_mem_cb(test_mem_cb); //test we did not break
    Qsim::setCurrCpu(&cpu);
    return true;
}



