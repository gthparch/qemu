CFLAGS += -I../include -fno-PIE -fPIC -O3
LDFLAGS += -shared
# TODO: Windows
DSOSUF := .so

NAME:= qsim-plugin
BIN := $(NAME)$(DSOSUF)

FILES := qsim-plugin.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(FILES)
	$(CC) $(LDFLAGS) -o $(BIN) $(FILES)

clean:
	rm $(FILES)
	rm $(BIN)
