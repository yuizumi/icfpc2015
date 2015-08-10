SRCS=$(wildcard *.cpp)
HDRS=$(wildcard *.h)

TARGETS=basic
LIBAI_DIR=interface

CXX=g++
CXXFLAGS=-O3 -Wall -std=c++11 -stdlib=libc++ -I$(LIBAI_DIR)

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

basic: main.o game.o $(LIBAI_DIR)/ai_main.o $(LIBAI_DIR)/libai.a
	$(CXX) $(CXXFLAGS) -o $@ $^

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
