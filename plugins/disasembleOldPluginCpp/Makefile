CXXFLAGS += -I../include -fno-PIE -fPIC -O3 -lcapstone
LDFLAGS += -shared
# TODO: Windows
UNAME_S := $(shell uname -s)
DSOSUF := .so

ifeq ($(UNAME_S),Darwin)
       DSOSUF := .dylib
endif

NAME:= qsim-plugin
BIN := $(NAME)$(DSOSUF)

FILES := qsim-plugin.o cs_disas.o

#%.o: %.cpp
#	$(CXX) -c -v -o $@ $< $(CXXFLAGS)

all: $(FILES)
	$(CXX) $(LDFLAGS) -v -o $(BIN) $(FILES)

cs_disas.o : cs_disas.cpp cs_disas.h
	$(CXX) $(CXXFLAGS) -c cs_disas.cpp  -o cs_disas.o

qsim-plugin.o : cs_disas.o qsim-plugin.cpp
	$(CXX) $(CXXFLAGS)  -c qsim-plugin.cpp -o qsim-plugin.o


clean:
	rm $(FILES)
	rm $(BIN)
