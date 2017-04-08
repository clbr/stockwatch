.PHONY: all clean

name = stockwatch

FLTKCONFIG = $(shell which fltk-config13 || which fltk-config)

CXXFLAGS += -Wall -Wextra -g
LDFLAGS += -Wl,-gc-sections

CXXFLAGS += $(shell $(FLTKCONFIG) --cxxflags | sed 's@-I@-isystem@g')
LDFLAGS += $(shell $(FLTKCONFIG) --ldflags)

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

all: $(name)

$(name): $(obj)
	$(CXX) -o $(name) $(obj) $(CXXFLAGS) $(LDFLAGS)

$(obj): $(wildcard *.h)

clean:
	rm -f $(name) *.o
