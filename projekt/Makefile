#ifndef _NAZWA_HH
#define _NAZWA_HH
#endif //_NAZWA_HH

FLAGS = -Wall -pedantic -std=c++11

__start__: a.out
	./a.out

a.out: obj/main.o obj/graf.o obj/przystanek.o
	g++ $(FLAGS) -o a.out obj/main.o obj/graf.o obj/przystanek.o

obj/main.o: src/main.cpp inc/libs.hh inc/graf.hh inc/przystanek.hh
	g++ $(FLAGS) -c -o obj/main.o src/main.cpp

obj/graf.o: src/graf.cpp inc/graf.hh inc/libs.hh inc/trasa.hh inc/przystanek.hh
	g++ $(FLAGS) -c -o obj/graf.o src/graf.cpp

obj/przystanek.o: inc/przystanek.hh inc/libs.hh
	g++ $(FLAGS) -c -o obj/przystanek.o src/przystanek.cpp

clean:
	rm -f obj/*.o a.out data/macierz_test.txt
