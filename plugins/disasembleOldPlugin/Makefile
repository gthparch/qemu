CFLAGS += -I../include -fno-PIE -fPIC -O3 -lcapstone
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
#	$(CXX) -c -v -o $@ $< $(CFLAGS)

all: $(FILES)
	$(CC) $(LDFLAGS) -v -o $(BIN) $(FILES)

cs_disas.o : cs_disas.c cs_disas.h
	$(CC) $(CFLAGS) -c cs_disas.c  -o cs_disas.o

qsim-plugin.o : cs_disas.o qsim-plugin.c
	$(CC) $(CFLAGS)  -c qsim-plugin.c -o qsim-plugin.o


clean:
	rm $(FILES)
	rm $(BIN)
