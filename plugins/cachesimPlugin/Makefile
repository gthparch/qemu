CXXFLAGS ?= -g -O2 -std=c++0x -Wall -I../qsimCore -Wl,--no-as-needed

LDLIBS ?= -pthread -ldl -lrt

TESTS = cachesim

all: $(TESTS)

qsim-load.o: ../qsimCore/qsim-load.cpp ../qsimCore/qsim-load.h ../qsimCore/qsim.h
	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-load.cpp

qsim-prof.o: ../qsimCore/qsim-prof.cpp ../qsimCore/qsim-prof.h ../qsimCore/qsim.h
	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-prof.cpp

qsim.o: qsim-load.o qsim-prof.o ../qsimCore/qsim.cpp ../qsimCore/qsim.h ../qsimCore/qsim-vm.h \
            ../qsimCore/qsim-regs.h ../qsimCore/qsim-x86-regs.h ../qsimCore/qsim-arm64-regs.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../qsimCore/qsim.cpp qsim-prof.o qsim-load.o


cs_disas.o : cs_disas.cpp cs_disas.h ../qsimCore/capstone.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -lcapstone -c cs_disas.cpp 

%: cs_disas.o qsim.o %.cpp
	$(CXX) $(CXXFLAGS) cachesim.cpp cs_disas.o qsim.o -lcapstone -o $(TESTS)

clean:
	rm -f $(TESTS)
	rm *.o
