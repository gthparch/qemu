CXXFLAGS += -I ../qsimCore -I../include -fno-PIE -fPIC -O3 -ldl -lrt
LDFLAGS += -shared
# TODO: Windows
UNAME_S := $(shell uname -s)
DSOSUF := .so

ifeq ($(UNAME_S),Darwin)
       DSOSUF := .dylib
endif

NAME:= disas-plugin
BIN := $(NAME)$(DSOSUF)

FILES := qsim.o cs_disas.o disas-plugin.o

#%.o: %.cpp ../qsimCore/%.cpp
#	$(CC) -c -v -o $@ $< $(CXXFLAGS)

all: $(FILES)
	$(CXX) $(LDFLAGS) -v -o $(BIN) $(FILES)

#qsim-load.o: ../qsimCore/qsim-load.cpp ../qsimCore/qsim-load.h ../qsimCore/qsim.h
#	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-load.cpp

#qsim-prof.o: ../qsimCore/qsim-prof.cpp ../qsimCore/qsim-prof.h ../qsimCore/qsim.h
#	$(CXX) $(CXXFLAGS) -I./ -fPIC -c ../qsimCore/qsim-prof.cpp

qsim.o: ../qsimCore/qsim.cpp ../qsimCore/qsim.h ../qsimCore/qsim-vm.h \
            ../qsimCore/qsim-regs.h ../qsimCore/qsim-x86-regs.h ../qsimCore/qsim-arm64-regs.h
	$(CXX) $(CXXFLAGS) -fPIC -ldl -lrt -c ../qsimCore/qsim.cpp -o qsim.o


cs_disas.o : cs_disas.cpp cs_disas.h ../qsimCore/capstone.h
	$(CXX) $(CXXFLAGS) -lcapstone -c cs_disas.cpp  -o cs_disas.o

disas-plugin.o : qsim.o cs_disas.o disas-plugin.cpp
	$(CXX) $(CXXFLAGS)  -c disas-plugin.cpp -o disas-plugin.o

clean:
	rm $(FILES)
	rm $(BIN)
