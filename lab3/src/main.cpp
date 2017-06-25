#include "../inc/Stoper.hh"
#include "../inc/Lista.hh"
#include "../inc/Kolejka.hh"
#include "../inc/Stos.hh"
#include "../inc/Libs.hh"

int main() {

	Lista* lista;
	Kolejka* kolejka;
	Stos* stos;
	Stoper stoper;

	lista = new Lista();
	kolejka = new Kolejka();
	stos = new Stos();
	std::ofstream file;

	int pom = 9999; // dowolna wartość, 2x będzie maksymalną wartością klucza, 1x szukany
	int ilosc_pomiarow = 20;
	int ilosc_elem_MAX = 1000000;

	srand(time(NULL));


	Elem *elem;
	for (int i=0;i<=10000;i++){
	elem = new Elem(i,i);
	kolejka->push(elem);	
	std::cout << i << "\n";
	}
	std::cout<<"\n";
	for (int i=0;i<=10000;i++){
	elem = kolejka->pop();
	std::cout << elem->get_priority() << "\n";
	delete elem;	
	}

	remove("lista.dat");
	file.open("lista.dat"); // std::ios::app
	for (int ilosc_elem = 10; ilosc_elem <= ilosc_elem_MAX; ilosc_elem *= 10) {
		stoper.wykonaj_pomiar(lista, ilosc_pomiarow, pom, ilosc_elem);
		stoper.wypisz_pomiar(file);
		stoper.wypisz_pomiar(std::cout);
	}
	file.close();

	remove("kolejka.dat");
	file.open("kolejka.dat"); // std::ios::app
	for (int ilosc_elem = 10; ilosc_elem <= ilosc_elem_MAX; ilosc_elem *= 10) {
		stoper.wykonaj_pomiar(kolejka, ilosc_pomiarow, pom, ilosc_elem);
		stoper.wypisz_pomiar(file);
		stoper.wypisz_pomiar(std::cout);
	}
	file.close();

	remove("stos.dat");
	file.open("stos.dat"); // std::ios::app
	for (int ilosc_elem = 10; ilosc_elem <= ilosc_elem_MAX; ilosc_elem *= 10) {
		stoper.wykonaj_pomiar(stos, ilosc_pomiarow, pom, ilosc_elem);
		stoper.wypisz_pomiar(file);
		stoper.wypisz_pomiar(std::cout);
	}
	file.close();

	return 0;
}
