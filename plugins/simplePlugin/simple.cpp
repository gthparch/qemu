/*****************************************************************************\
* Qemu Simulation Framework (qsim)                                            *
* Qsim is a modified version of the Qemu emulator (www.qemu.org), couled     *
* a C++ API, for the use of computer architecture researchers.                *
*                                                                             *
* This work is licensed under the terms of the GNU GPL, version 2. See the    *
* COPYING file in the top-level directory.                                    *
\*****************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

#include <qsim.h>
#include <qsim-load.h>
#include <capstone.h>

#include "cs_disas.h"

using Qsim::OSDomain;

using std::ostream;

class TraceWriter {
public:
  TraceWriter(OSDomain &osd, ostream &tracefile) : 
	  osd(osd), tracefile(tracefile), finished(false),
	  dis(CS_ARCH_ARM64, CS_MODE_ARM)
  { 
    osd.set_app_start_cb(this, &TraceWriter::app_start_cb);
  }

  bool hasFinished() { return finished; }

  int app_start_cb(int c) {
    static bool ran = false;
    if (!ran) {
      ran = true;
      osd.set_inst_cb(this, &TraceWriter::inst_cb);
      osd.set_mem_cb(this, &TraceWriter::mem_cb);
      //osd.set_int_cb(this, &TraceWriter::int_cb);
      osd.set_app_end_cb(this, &TraceWriter::app_end_cb);

      return 1;
    }

    return 0;
  }

  int app_end_cb(int c)   { finished = true; return 0; }

  void inst_cb(int c, uint64_t v, uint64_t p, uint8_t l, const uint8_t *b, 
               enum inst_type t)
  {
    cs_insn *insn = NULL;

    int count = dis.decode((unsigned char *)b, l, insn);
    insn[0].address = v;

    tracefile << std::dec << c << ": " << std::hex << insn[0].address << " : "
              << insn[0].mnemonic << " : " << insn[0].op_str << std::endl;

    dis.free_insn(insn, count);
    fflush(NULL);
    return;
  }

  void mem_cb(int c, uint64_t v, uint64_t p, uint8_t s, int w) {
    tracefile << std::dec << c << ":  " << (w?"WR":"RD") << "(0x" << std::hex
              << v << "/0x" << p << "): " << std::dec << (unsigned)(s*8) 
              << " bits.\n";
  }

  void reg_cb(int c, int r, uint8_t s, int type) {
    tracefile << std::dec << c << (s == 0?": Flag ":": Reg ") 
              << (type?"WR":"RD") << std::dec;

    if (s != 0) tracefile << ' ' << r << ": " << (unsigned)(s*8) << " bits.\n";
    else tracefile << ": mask=0x" << std::hex << r << '\n';
  }

  int int_cb(int c, uint8_t v) {
    tracefile << std::dec << c << ": Interrupt 0x" << std::hex << std::setw(2)
              << std::setfill('0') << (unsigned)v << '\n';
    return 0;
  }

private:
  OSDomain &osd;
  ostream &tracefile;
  bool finished;
  cs_disas dis;

  static const char * itype_str[];
};

const char *TraceWriter::itype_str[] = {
  "QSIM_INST_NULL",
  "QSIM_INST_INTBASIC",
  "QSIM_INST_INTMUL",
  "QSIM_INST_INTDIV",
  "QSIM_INST_STACK",
  "QSIM_INST_BR",
  "QSIM_INST_CALL",
  "QSIM_INST_RET",
  "QSIM_INST_TRAP",
  "QSIM_INST_FPBASIC",
  "QSIM_INST_FPMUL",
  "QSIM_INST_FPDIV"
};

bool plugin_init(const char *args) {
  using std::istringstream;
  using std::ofstream;
  ofstream *outfile(NULL);
  unsigned n_cpus = 1;
  
  OSDomain osd(n_cpus);
   outfile = new ofstream("trace.log");

  // Attach a TraceWriter if a trace file is given.
  TraceWriter tw(osd, outfile);
  tw.app_start_cb(0);
  osd.connect_console(std::cout);
  tw.app_start_cb(0);
  

  // NOTE: everythng from here to delete outfile needs to be in a different function
  
  // The main loop: run until 'finished' is true.
  unsigned long inst_per_iter = 1000000000, inst_run;
  inst_run = inst_per_iter;
  while (!(inst_per_iter - inst_run)) {
    inst_run = 0;
    inst_run = osd.run(inst_per_iter);
    osd.timer_interrupt();
  }
  
  if (outfile) { outfile->close(); }
  delete outfile;

  return true;
}
