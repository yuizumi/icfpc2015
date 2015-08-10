SRCS=$(wildcard *.cc)
HDRS=$(wildcard *.h)

TARGETS=libai.a ai_main.o

# Debug mode.
CXX=g++
CXXFLAGS=-O3 -Wall -std=c++11 -stdlib=libc++
# Release mode.
#CXX=g++
#CXXFLAGS=-O3 -Wall -std=c++11 -DNDEBUG

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

libai.a: basic.o board.o debug.o game.o input.o unit.o
	$(AR) -ru $@ $^

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
