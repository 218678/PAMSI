#include "../inc/libs.hh"
#include "../inc/trasa.hh"
#include "../inc/przystanek.hh"

class Graf {
public://private:
  std::vector<Przystanek*> przystanki;
  short** macierz_sasiedzctw;
  int rozmiar_macierzy;
public:
  Graf();
  ~Graf();
  // void dodajPrzystanek();
  // void dodajTrase();
  vector<Przystanek*> podajSasiadow(int a);
  bool czyPoloczone(int a, int b);
  int zwrocRozmiar_macierzy();

  void wczytajPrzystanki();
  void parsujMacierz(string nazwa_pliku_do_zapisu); // oddzielna funkcja, poniewaz trwa dlugo i szybciej czyta gotowca z pliku
  void wczytajMacierzZPliku(string nazwa_pliku_zrodlowgo);
  int znajdz_index_po_stop_id(int fun_stop_id);
  int heuresticFun(int a, int b) {return 0;}

  vector<Przystanek*> astar(int a_stop_id, int b_stop_id);
  vector<Przystanek*> breadthFirst(int a_stop_id, int b_stop_id);

private:
  istream& wczytajTrase(istream& is, Trasa& trasa);
};
