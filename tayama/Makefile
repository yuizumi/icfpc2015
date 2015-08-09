all: greedy translate
	./eval.sh

greedy: *.cc ../yuizumi/*.cc
	g++ -g -std=c++11 -stdlib=libc++ -o greedy *.cc ../yuizumi/board.cc ../yuizumi/debug.cc ../yuizumi/game.cc ../yuizumi/unit.cc

translate:
	g++ -std=c++11 -stdlib=libc++ -o translate ../yuizumi/translate.cc

clean:
	rm greedy translate
