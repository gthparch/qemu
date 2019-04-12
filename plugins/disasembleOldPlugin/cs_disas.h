#include <capstone/capstone.h>

struct platform {
	cs_arch arch;
	cs_mode mode;
};

typedef struct cs_disas {
    csh handle;
    struct platform pf;
} cs_disas;

void cs_disasInit(cs_disas* cs, cs_arch arch, cs_mode mode);
void cs_disasDestruct(cs_disas* cs);
int decode(cs_disas* cs, unsigned char *code, int size, cs_insn *insn); 
void free_insn(cs_insn *insn, int count);
//int get_regs_access(cs_disas* cs, cs_insn *insn, cs_regs regs_read, cs_regs regs_write, uint8_t *regs_read_count,
//                     uint8_t *regs_write_count);
