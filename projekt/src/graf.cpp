#include "../inc/graf.hh"

void Graf::wczytajPrzystanki() {
  fstream file_stream;
  Przystanek *przystanek;
  string buffer;

  file_stream.open("data/stops.txt");
  getline(file_stream, buffer); // ignorujemy pierwszą linię która jest opisem kolumn

  while(!file_stream.eof()) { // fstream zwraca eof dopiero po odczytaniu ze strumienia
    przystanek = new Przystanek();
    file_stream >> przystanek;// dlatego pętla wchodzi na ostatnim dwa razy
    if (!file_stream.eof())   // a żeby czytało jak należy jest jeszcze jeden warunek
                              // zaraz za odczytaniem - brzydkie, ale nie mam lepszego pomyslu
    przystanki.push_back(przystanek);
  }
  file_stream.close();
}

void Graf::parsujMacierz(string nazwa_pliku_do_zapisu) {
  Trasa trasaA;
  Trasa trasaB;

  fstream file_stream;
  file_stream.open("data/stop_times.txt");
  string buffer;
  getline(file_stream, buffer); // ignorujemy pierwszą linię, opis kolumn

  // zajęcie miejsca - zakladam ze wskaźnik na fun_macierz_sasiedzctw jest wolna
  short **fun_macierz_sasiedzctw;
  fun_macierz_sasiedzctw = new short*[przystanki.size()];
  for (unsigned int i=0; i<przystanki.size(); ++i)
    fun_macierz_sasiedzctw[i] = new short[przystanki.size()];

  // wypelniam zerami
  for (unsigned int i=0; i<przystanki.size(); ++i)
    for (unsigned int j=0; j<przystanki.size(); ++j)
      fun_macierz_sasiedzctw[i][j] = 0;

  // wpisanie danych do macierzy
  while(!file_stream.eof()) {
  //for (int i=0; i<100; ++i) { // tymczasowa petla zeby nie ladowac wszystkiego
    wczytajTrase(file_stream, trasaB); // do B, bo pierwsza petla pusta, diff(B-A
    if (trasaA.stop_id != 0 && trasaB.stop_id != 0 && trasaA.trip_id == trasaB.trip_id) {
      fun_macierz_sasiedzctw[znajdz_index_po_stop_id(trasaA.stop_id)][znajdz_index_po_stop_id(trasaB.stop_id)] = difftime(mktime(&trasaB.struct_czas),mktime(&trasaA.struct_czas));
      //cout << "A:" << trasaA.stop_id << " B:" << trasaB.stop_id << " difftime:" << difftime(mktime(&trasaB.struct_czas),mktime(&trasaA.struct_czas)) << "\n";
    }
    trasaA = trasaB;
  }
  file_stream.close();

  cout << "wczytane";

  // zapis do pliku
  file_stream.open(nazwa_pliku_do_zapisu);
  cout << file_stream.is_open();
  for (unsigned int i=0; i<przystanki.size(); ++i) {
    for (unsigned int j=0; j<przystanki.size(); ++j) {
        file_stream << fun_macierz_sasiedzctw[i][j] << ",";
        if (j == przystanki.size()-1)
          file_stream << fun_macierz_sasiedzctw[i][j];
    }
    file_stream <<"\n";
  }
  file_stream.close();

  for (unsigned int i=0; i<przystanki.size(); ++i)
    delete fun_macierz_sasiedzctw[i];
  delete fun_macierz_sasiedzctw;

}

void Graf::wczytajMacierzZPliku(string nazwa_pliku_zrodlowgo) {
  fstream file_stream;
  string buffer;
  std::string::size_type str_size_type;     // alias of size_t dla getline

  // zajęcie miejsca - zakladam ze wskaźnik na fun_macierz_sasiedzctw jest wolna
  macierz_sasiedzctw = new short*[przystanki.size()];
  for (unsigned int i=0; i<przystanki.size(); ++i)
    macierz_sasiedzctw[i] = new short[przystanki.size()];

  // wypelniam zerami
  for (unsigned int i=0; i<przystanki.size(); ++i)
    for (unsigned int j=0; j<przystanki.size(); ++j)
      macierz_sasiedzctw[i][j] = 0;

  file_stream.open(nazwa_pliku_zrodlowgo);
  for (unsigned int i=0; i<przystanki.size(); ++i) {
    for (unsigned int j=0; j<przystanki.size(); ++j) {
      getline(file_stream, buffer,',');
      macierz_sasiedzctw[i][j] = stoi(buffer, &str_size_type);
    }
  }
  file_stream.close();

  cout << macierz_sasiedzctw[1][1] << "\n";
}

Graf::Graf() {
  //wczytajPrzystanki();
  //wczytajMacierzZPliku("macierz.txt");
}
Graf::~Graf() {}

int Graf::znajdz_index_po_stop_id(int fun_stop_id) {
  unsigned int i;
  for (i=0; i<przystanki.size(); ++i)
    if (przystanki[i]->stop_id == fun_stop_id)
      return i;
  return -1;
}

istream& Graf::wczytajTrase(istream& is, Trasa& trasa) {
  string buffer = "0";
  std::string::size_type str_size_type;     // alias of size_t dla getline

  if(!is.eof()) { // zeby uniknac warningow "brak return'a na koncu" przy make'u - i tak nie wykona sie jesli nie ma na czym
    getline(is, buffer,',');
    if(is.eof()) // przerywa jeśli eof, tylko przy pierwszym getline'ie
      return is; //bo dane są sformatowane i jeśli nie złapie pierwszej zmiennej,
                 //nie czyta całego wiersza
    trasa.trip_id = buffer;

    getline(is, trasa.arrival_time,',');
    trasa.struct_czas.tm_hour = stoi(trasa.arrival_time.substr(0,2),  &str_size_type);// hh:mm:ss
    trasa.struct_czas.tm_min = stoi(trasa.arrival_time.substr(3,2),  &str_size_type); // 01234567
    trasa.struct_czas.tm_sec = stoi(trasa.arrival_time.substr(6,2),  &str_size_type);
    // substr(pozycja, ileznakow)

    if (trasa.struct_czas.tm_hour >= 24) {
      trasa.struct_czas.tm_hour = trasa.struct_czas.tm_hour%23 - 1;
      //cout << "zmiana godziny 23+" << trasa.struct_czas.tm_hour << " " << trasa.struct_czas.tm_min << " " <<  trasa.struct_czas.tm_sec << "\n";
    }


    getline(is, buffer,','); // ignorujemy departure_time

    getline(is, buffer,',');
    trasa.stop_id = stoi(buffer, &str_size_type);

    getline(is, buffer,','); // ignorujemy stop_sequence
    getline(is, buffer,','); // ignorujemy pickup_type
    getline(is, buffer);     // ignorujemy drop_off_type
  }
  return is;
}
