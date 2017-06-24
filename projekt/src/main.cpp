#include "../inc/libs.hh"
#include "../inc/graf.hh"
#include "../inc/przystanek.hh"


void testujGraf(Graf graf);
void testujMacierz(Graf graf);

int main() {
  Graf graf;

  testujGraf(graf);

  cout << "\nBYE\n";
  return 0;
}









///////////////////////////////// kod do testowania ///// do usuniecia

void testujGraf(Graf graf) {

  graf.wczytajPrzystanki();
  ////////////////////////////////////////////////////////////////////////////// testowanie przystankow
  cout << graf.przystanki.size() << "\n";
  // for (unsigned int i=0;i<graf.przystanki.size();++i)
  //   cout << graf.przystanki[i]->stop_id << "\n";

  ////////////////////////////////////////////////////////////////////////////// testowanie wczytywania macierzy
  graf.wczytajMacierzZPliku("data/macierz_backup.txt");
  testujMacierz(graf);

  //////////////////////////////////////////////////////////////////////////////////////////
  graf.parsujMacierz("data/macierz_test.txt");  // troche trwa
  testujMacierz(graf);
  //
  // for (unsigned int i=0; i<graf.przystanki.size(); ++i) {
  //   for (unsigned int j=0; j<graf.przystanki.size(); ++j) {
  //       if (graf.macierz_sasiedzctw[i][j] > 20000) {
  //         cout << "m: " <<graf.macierz_sasiedzctw[i][j] << "\n";
  //         cout << "i: " << graf.przystanki[i]->stop_id << " " << graf.przystanki[i]->stop_name << "\n";
  //         cout << "j: " << graf.przystanki[j]->stop_id << " " << graf.przystanki[j]->stop_name << "\n";
  //     }
  //   }
  // }

  int i = graf.znajdz_index_po_stop_id(1218), j = graf.znajdz_index_po_stop_id(449);
  cout << "\n\n\nm: " <<graf.macierz_sasiedzctw[i][j] << "\n";
  cout << "i: " << graf.przystanki[i]->stop_id << " " << graf.przystanki[i]->stop_name << "\n";
  cout << "j: " << graf.przystanki[j]->stop_id << " " << graf.przystanki[j]->stop_name << "\n";
}


void testujMacierz(Graf graf) {
  vector<int> wektor_jakie;
  vector<int> wektor_ile(50, 0);// ile,jakawartosc
  wektor_jakie.push_back(0);
  short rozne_czasy = 1;

  for (unsigned int i=0; i<graf.przystanki.size(); ++i) {
    for (unsigned int j=0; j<graf.przystanki.size(); ++j) {

      // sprawdzam czy sa jakies czasy
      if (graf.macierz_sasiedzctw[i][j] >= 0) {
        // cout << graf.macierz_sasiedzctw[i][j] << " ";

        // sprawdzam jakie sa wszystkie mozliwe czasy dojazdu
        for(unsigned int k=0; k<wektor_jakie.size(); ++k) {
          // cout << graf.macierz_sasiedzctw[i][j] << "|" << wektor_jakie[k] << "\n";

          if (graf.macierz_sasiedzctw[i][j] != wektor_jakie[k]) {
            if (k == wektor_jakie.size()-1) {
              wektor_jakie.push_back(graf.macierz_sasiedzctw[i][j]);
              rozne_czasy++;
            }
          }

          if (graf.macierz_sasiedzctw[i][j] == wektor_jakie[k]) {
            wektor_ile[k] = wektor_ile[k] + 1;
            break;
          }

        }
      }
    }
  }
  cout << "\n";

  cout << "\nCzasy dojazdu\n";
  for (unsigned int i=0;i<wektor_jakie.size();++i) {
    cout << "jakie:" << wektor_jakie[i] << " \tile:" << wektor_ile[i] << "\n";
  }
  cout << "Rozne czasy " << rozne_czasy << "\n";
}
