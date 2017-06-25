
FLAGS = -Wall -pedantic -std=c++11

__start__: a.out
	./a.out

a.out: obj/main.o obj/Tree.o
	g++ $(FLAGS) -o a.out obj/main.o obj/Tree.o

obj/main.o: src/main.cpp inc/Tree.hh
	g++ $(FLAGS) -c -o obj/main.o src/main.cpp

obj/Tree.o: src/Tree.cpp inc/Tree.hh inc/ITree.hh inc/TreeNode.hh
	g++ $(FLAGS) -c -o obj/Tree.o src/Tree.cpp

clean:
	rm -f obj/*.o a.out
