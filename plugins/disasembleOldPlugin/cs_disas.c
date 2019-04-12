#include "cs_disas.h"
#include <stdio.h>

void cs_disasInit(cs_disas* cs, cs_arch arch, cs_mode mode) {
    cs->pf.arch = arch;
    cs->pf.mode = mode;
    cs_err err = cs_open(cs->pf.arch, cs->pf.mode, &cs->handle);
    if (err) {
        fprintf(stderr, "Failed on cs_open with error: ");
        return;
    }
    cs_option(cs->handle, CS_OPT_DETAIL, CS_OPT_ON);
}

void cs_disasDestruct(cs_disas* cs)
{
    cs_close(&cs->handle);
}

int decode(cs_disas *cs, unsigned char *code, int size, cs_insn *insn)
{
    int count;

    count = cs_disasm(cs->handle, code, size, 0, 0, &insn);

    return count;
}

void free_insn(cs_insn *insn, int count)
{
    cs_free(insn, count);
}

//int get_regs_access(cs_disas *cs, cs_insn *insn, cs_regs regs_read, cs_regs regs_write, uint8_t *regs_read_count, uint8_t *regs_write_count)
//{
//    int ret;
//
//    ret = cs_regs_access(cs->handle, insn, regs_read, regs_read_count,
//                                       regs_write, regs_write_count);
//
//    return ret;
//}
