CXXFLAGS += -I ../qsimCore -I../include -fno-PIE -fPIC -O3
LDFLAGS += -shared
# TODO: Windows
UNAME_S := $(shell uname -s)
DSOSUF := .so

ifeq ($(UNAME_S),Darwin)
       DSOSUF := .dylib
endif

NAME:= disas-plugin
BIN := $(NAME)$(DSOSUF)

FILES := disas-plugin.o

all: $(FILES)
	$(CXX) $(LDFLAGS) -v -o $(BIN) $(FILES)

#qsim-load.o: ../qsimCore/qsim-load.cpp ../qsimCore/qsim-load.h ../qsimCore/qsim.h
#	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-load.cpp

#qsim-prof.o: ../qsimCore/qsim-prof.cpp ../qsimCore/qsim-prof.h ../qsimCore/qsim.h
#	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-prof.cpp

#qemuplugin.o : ../plugins.c  ../qemulib.c ../helper.h
#	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../plugins.c

#qsim.o: qsim-load.o qsim-prof.o ../qsimCore/qsim.cpp ../qsimCore/qsim.h ../qsimCore/qsim-vm.h \
            ../qsimCore/qsim-regs.h ../qsimCore/qsim-x86-regs.h ../qsimCore/qsim-arm64-regs.h
#	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../qsimCore/qsim.cpp qsim-prof.o qsim-load.o


#cs_disas.o : cs_disas.cpp cs_disas.h ../qsimCore/capstone.h
#	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -lcapstone -c cs_disas.cpp 

%.o: %.cpp ../qsimCore/qsim.cpp ../qsimCore/qsim.h ../qsimCore/qsim-vm.h ../qsimCore/qsim-regs.h \
           ../qsimCore/qsim-x86-regs.h ../qsimCore/qsim-arm64-regs.h
	$(CXX) -c -v -o $@ $< $(CXXFLAGS)

clean:
	rm $(FILES)
	rm $(BIN)
