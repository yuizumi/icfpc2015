SRCS=$(wildcard *.cc)
HDRS=$(wildcard *.h)

TARGETS=greedy translate validate
LIBAI_DIR=../libai

CXX=g++
CXXFLAGS=-O3 -Wall -std=c++11 -stdlib=libc++ -I$(LIBAI_DIR)

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

greedy: greedy.o $(LIBAI_DIR)/ai_main.o $(LIBAI_DIR)/libai.a
	$(CXX) $(CXXFLAGS) -o $@ $^

translate: translate.o $(LIBAI_DIR)/libai.a
	$(CXX) $(CXXFLAGS) -o $@ $^

validate: validate.o $(LIBAI_DIR)/libai.a
	$(CXX) $(CXXFLAGS) -o $@ $^

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
