CFLAGS += -I ../qsimCore -I../include -fno-PIE -fPIC -O3 -std=c++11
LDFLAGS += -shared
# TODO: Windows
UNAME_S := $(shell uname -s)
DSOSUF := .so

ifeq ($(UNAME_S),Darwin)
       DSOSUF := .dylib
endif

NAME:= test-plugin
BIN := $(NAME)$(DSOSUF)

FILES := test-plugin.o qsim.o

all: $(FILES)
	$(CXX) $(LDFLAGS) -v -o $(BIN) $(FILES)

qsim.o: ../qsimCore/qsim.cpp ../qsimCore/qsim.h ../qsimCore/qsim-vm.h \
            ../qsimCore/qsim-regs.h ../qsimCore/qsim-x86-regs.h ../qsimCore/qsim-arm64-regs.h
	$(CXX) $(CFLAGS) -fPIC -ldl -lrt -c ../qsimCore/qsim.cpp -o qsim.o

test-plugin.o : qsim.o test-plugin.cpp
	$(CXX) $(CFLAGS)  -c test-plugin.cpp -o test-plugin.o

#%.o: %.cpp
#	$(CXX) -c -v -o $@ $< $(CFLAGS)

clean:
	rm $(FILES)
	rm $(BIN)
