#include "../inc/libs.hh"
#include "../inc/trasa.hh"
#include "../inc/przystanek.hh"

class Graf {
public://private:
  std::vector<Przystanek*> przystanki;
  short** macierz_sasiedzctw;
public:
  Graf();
  ~Graf();
  // void dodajPrzystanek();
  // void dodajTrase();
  // vector<Przystanek*> podajSasiadow(Przystanek a);
  // bool czyPoloczone(Przystanek a, Przystanek b);

  void wczytajPrzystanki();
  void parsujMacierz(string nazwa_pliku_do_zapisu); // oddzielna funkcja, poniewaz trwa dlugo i szybciej czyta gotowca z pliku
  void wczytajMacierzZPliku(string nazwa_pliku_zrodlowgo);
  int znajdz_index_po_stop_id(int fun_stop_id);
private:
  istream& wczytajTrase(istream& is, Trasa& trasa);
};
