SRCS=$(wildcard *.cc)
HDRS=$(wildcard *.h)

BINDIR=.

OBJS=basic.o board.o debug.o game.o input.o unit.o
TARGETS=$(BINDIR)/fake_ai $(BINDIR)/translate $(BINDIR)/validate

# Debug mode.
CXX=clang++
CXXFLAGS=-O3 -Wall -std=c++11 -stdlib=libc++
# Release mode.
#CXX=g++
#CXXFLAGS=-O3 -Wall -std=c++11 -DNDEBUG

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

$(BINDIR)/translate: translate.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%: $(OBJS) %.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/fake_ai: ai_main.o

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
