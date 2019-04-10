CXXFLAGS ?= -g -O2 -fno-PIE -fPIC   -std=c++0x -Wall -I../../include  -I../include -I../qsimCore -Wl,--no-as-needed

LDLIBS ?= -pthread -ldl -lrt

TESTS = simple

all: $(TESTS)

qsim-load.o: ../qsimCore/qsim-load.cpp ../qsimCore/qsim-load.h ../qsimCore/qsim.h
	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-load.cpp

qsim-prof.o: ../qsimCore/qsim-prof.cpp ../qsimCore/qsim-prof.h ../qsimCore/qsim.h
	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-prof.cpp

qemuplugin.o : ../plugins.c  ../qemulib.c ../helper.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../plugins.c

qsim.o: qemuplugin.o qsim-load.o qsim-prof.o ../qsimCore/qsim.cpp ../qsimCore/qsim.h ../qsimCore/qsim-vm.h \
            ../qsimCore/qsim-regs.h ../qsimCore/qsim-x86-regs.h ../qsimCore/qsim-arm64-regs.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../qsimCore/qsim.cpp qemuplugin.o qsim-prof.o qsim-load.o


cs_disas.o : cs_disas.cpp cs_disas.h ../qsimCore/capstone.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -lcapstone -c cs_disas.cpp 

%: cs_disas.o qsim.o %.cpp
	$(CXX) $(CXXFLAGS) simple.cpp cs_disas.o qsim.o -lcapstone -o $(TESTS)

clean:
	rm -f $(TESTS)
	rm *.o
