CFLAGS += -I ../qsimCore -I../include -fno-PIE -fPIC -O3
LDFLAGS += -shared
# TODO: Windows
UNAME_S := $(shell uname -s)
DSOSUF := .so

ifeq ($(UNAME_S),Darwin)
       DSOSUF := .dylib
endif

NAME:= test-plugin
BIN := $(NAME)$(DSOSUF)

FILES := test-plugin.o

%.o: %.cpp
	$(CXX) -c -v -o $@ $< $(CFLAGS)

all: $(FILES)
	$(CXX) $(LDFLAGS) -v -o $(BIN) $(FILES)

clean:
	rm $(FILES)
	rm $(BIN)
