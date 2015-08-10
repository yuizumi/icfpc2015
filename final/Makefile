SRCS=$(wildcard *.cc)
HDRS=$(wildcard *.h)

TARGETS=calc_score placer_ai greedy
LIBAI_DIR=../libai

CXX=g++
CXXFLAGS=-O3 -Wall -std=c++11 -stdlib=libc++ -I$(LIBAI_DIR) -DNDEBUG

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

calc_score: calc_score.o $(LIBAI_DIR)/libai.a
	$(CXX) $(CXXFLAGS) -o $@ $^

placer_ai: placer_ai.o $(LIBAI_DIR)/libai.a $(LIBAI_DIR)/ai_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

greedy: greedy.o $(LIBAI_DIR)/ai_main.o $(LIBAI_DIR)/libai.a
	$(CXX) $(CXXFLAGS) -o $@ $^

.depend: $(SRCS) $(HDRS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) >> ./.depend
-include .depend

.PHONY: clean
