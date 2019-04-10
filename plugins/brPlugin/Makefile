CXXFLAGS ?= -g -O2 -std=c++0x -Wall -I../qsimOld -Wl,--no-as-needed

LDLIBS ?= -pthread -ldl -lrt

TESTS = tester

all: $(TESTS)

qsim-load.o: ../qsimOld/qsim-load.cpp ../qsimOld/qsim-load.h ../qsimOld/qsim.h
	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimOld/qsim-load.cpp

qsim-prof.o: ../qsimOld/qsim-prof.cpp ../qsimOld/qsim-prof.h ../qsimOld/qsim.h
	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimOld/qsim-prof.cpp

qsim.o: qsim-load.o qsim-prof.o ../qsimOld/qsim.cpp ../qsimOld/qsim.h ../qsimOld/qsim-vm.h \
            ../qsimOld/qsim-regs.h ../qsimOld/qsim-x86-regs.h ../qsimOld/qsim-arm64-regs.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../qsimOld/qsim.cpp qsim-prof.o qsim-load.o


cs_disas.o : cs_disas.cpp cs_disas.h ../qsimOld/capstone.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -lcapstone -c cs_disas.cpp 

%: cs_disas.o qsim.o %.cpp
	$(CXX) $(CXXFLAGS) tester.cpp cs_disas.o qsim.o -lcapstone -o $(TESTS)

clean:
	rm -f $(TESTS)
	rm *.o
