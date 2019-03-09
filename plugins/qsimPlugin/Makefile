CFLAGS += -I../include -fno-PIE -fPIC -O3 -framework CoreFoundation
LDFLAGS += -shared
# TODO: Windows
UNAME_S := $(shell uname -s)
DSOSUF := .so

ifeq ($(UNAME_S),Darwin)
       DSOSUF := .dylib
endif

NAME:= qsim-plugin
BIN := $(NAME)$(DSOSUF)

FILES := qsim-plugin.o

%.o: %.c
	$(CC) -c -v -o $@ $< $(CFLAGS)

all: $(FILES)
	$(CC) $(LDFLAGS) -v -o $(BIN) $(FILES)

clean:
	rm $(FILES)
	rm $(BIN)
