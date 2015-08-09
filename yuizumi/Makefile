SRCS=$(wildcard *.cc)
HDRS=$(wildcard *.h)

BINDIR=.

OBJS=board.o debug.o game.o input.o main.o unit.o
TARGETS=$(BINDIR)/fake_ai $(BINDIR)/translate

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

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
