#include "qsim.h"
#include <capstone.h>
#include "cs_disas.h"
#include <stdint.h>
#include <iostream>

Qsim::QemuCpu cpu;

cs_disas dis(CS_ARCH_ARM64, CS_MODE_ARM);

void test_inst_cb(int c, uint64_t v, uint64_t p, uint8_t l,
             const uint8_t *b,  enum inst_type type)
{
    FILE *insFile = fopen ("inst.log","a");
    cs_insn *insn = NULL;
    int count = dis.decode((unsigned char *)b, l, insn);
    //std::cerr << "0x" << std::hex << v << std::dec << " " << insn[0].mnemonic  << " " << insn[0].op_str << std::endl;
    fprintf(insFile, "0x%lx\n %s %s", v, insn[0].mnemonic, insn[0].op_str);
    dis.free_insn(insn, count);
    fclose(insFile);
    return;
}

bool plugin_init(const char *args)
{
    FILE *insFile = fopen ("inst.log","w");
    //fprintf (insFile, "");
    fclose(insFile);
    cpu.set_inst_cb(test_inst_cb); //test we did not break
    Qsim::setCurrCpu(&cpu);
    return true;
}



