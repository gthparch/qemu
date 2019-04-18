CAPSTONE := `pkg-config capstone --cflags --libs`
CXXFLAGS := -I../include -fno-PIE -fPIC -O3 
CXXFLAGS += $(CAPSTONE)
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

%.o: %.cpp
	$(CXX) -c -v -o $@ $< $(CXXFLAGS)

all: $(FILES)
	$(CXX) $(LDFLAGS) -v -o $(BIN) $(FILES)

clean:
	rm $(FILES)
	rm $(BIN)
