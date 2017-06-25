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

  rozmiar_macierzy = przystanki.size();
  cout << "wczytano przystanki\n";
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
      fun_macierz_sasiedzctw[i][j] = 32767;

  // wpisanie danych do macierzy
  while(!file_stream.eof()) {
  //for (int i=0; i<100; ++i) { // tymczasowa petla zeby nie ladowac wszystkiego
    wczytajTrase(file_stream, trasaB); // do B, bo pierwsza petla pusta, diff(B-A
    if (trasaA.stop_id != 0 && trasaB.stop_id != 0 && trasaA.trip_id == trasaB.trip_id) {
      fun_macierz_sasiedzctw[znajdz_index_po_stop_id(trasaA.stop_id)][znajdz_index_po_stop_id(trasaB.stop_id)] = difftime(mktime(&trasaB.struct_czas),mktime(&trasaA.struct_czas));
      // fun_macierz_sasiedzctw[znajdz_index_po_stop_id(trasaB.stop_id)][znajdz_index_po_stop_id(trasaA.stop_id)] = difftime(mktime(&trasaB.struct_czas),mktime(&trasaA.struct_czas));
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
      macierz_sasiedzctw[i][j] = 32767;

  file_stream.open(nazwa_pliku_zrodlowgo);
  for (unsigned int i=0; i<przystanki.size(); ++i) {
    for (unsigned int j=0; j<przystanki.size(); ++j) {
      getline(file_stream, buffer,',');
      macierz_sasiedzctw[i][j] = stoi(buffer, &str_size_type);
    }
  }
  file_stream.close();
  cout << "wczytano z pliku\n";
}

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

Graf::Graf() {
}
Graf::~Graf() {
}

bool Graf::czyPoloczone(int a_stop_id, int b_stop_id) {
  if (macierz_sasiedzctw == nullptr)
    return false;

  if ( macierz_sasiedzctw[znajdz_index_po_stop_id(a_stop_id), znajdz_index_po_stop_id(b_stop_id)] != 0 )
    return true;

  return false;
}

int Graf::zwrocRozmiar_macierzy() {
  return rozmiar_macierzy;
}

vector<Przystanek*> Graf::podajSasiadow(int a_stop_id) {
  vector<Przystanek*> sasiedzi;
  for (unsigned int i=0; i<przystanki.size(); ++i) {
    if (macierz_sasiedzctw[znajdz_index_po_stop_id(a_stop_id)][i] != 32767) {
      sasiedzi.push_back(przystanki[i]);
    }
  }
  return sasiedzi;
}


vector<Przystanek*> Graf::breadthFirst(int a_stop_id, int b_stop_id) {
  vector<int> odwiedzony(przystanki.size(), 0);
  vector<int> odleglosc(przystanki.size(), 0);
  vector<int> rodzic(przystanki.size(), 0);
  list<Przystanek*> lista;
  Przystanek* tester;
  int sasiad_indeks, tester_indeks;

  int indeks = znajdz_index_po_stop_id(a_stop_id);
  odwiedzony[indeks] = 1;
  odleglosc[indeks] = 0;
  rodzic[indeks] = -1;
  lista.push_back(przystanki[indeks]);

  while (!lista.empty()) {

    for (auto a : lista)
      cout << "l:" << a->stop_id << " " << a->stop_name << "\n";


    tester = lista.front();
    cout << "test1\n\n";

    lista.pop_front();
    tester_indeks = znajdz_index_po_stop_id(tester->stop_id);

    vector<Przystanek*> sasiedzi = podajSasiadow(tester_indeks);
    if (!sasiedzi.empty())  
    for (auto sasiad : sasiedzi) {
      cout << "test3\n\n";

      sasiad_indeks = znajdz_index_po_stop_id(sasiad->stop_id);
      if (odwiedzony[sasiad_indeks] == 0) {
        odwiedzony[sasiad_indeks] = 1;
        odleglosc[sasiad_indeks] = odleglosc[tester_indeks] + macierz_sasiedzctw[tester_indeks][sasiad_indeks];
        rodzic[sasiad_indeks] = tester_indeks;
        cout << "test2\n";
        lista.push_back(sasiad);
        cout << "if\n";
      }
      cout << "for\n";
    }
    cout << "while\n";
    odwiedzony[tester_indeks] = 2;
  }
  cout << "while end\n";

  // for (int i=0; i<rodzic.size(); ++i) {
  //   cout << i << ":" << rodzic[i] << "\n";
  // }

  int wskaznik = b_stop_id;
  vector<Przystanek*> nitka;
  while (wskaznik != -1 && wskaznik != 0) {
    cout << "w:" << wskaznik << ";" << rodzic[wskaznik] << "\n";
    nitka.push_back(przystanki[wskaznik]);
    wskaznik = rodzic[wskaznik];
    cout << "while2\n";
  }
  cout << "return\n---\n";
  return nitka;
}

vector<Przystanek*> Graf::astar(int a_stop_id, int b_stop_id) {

  Przystanek *a = przystanki[znajdz_index_po_stop_id(a_stop_id)];
  Przystanek *b = przystanki[znajdz_index_po_stop_id(b_stop_id)];

  vector<Przystanek*> zamkniete;
  list<Przystanek*> otwarte;
  std::list<Przystanek*>::iterator it, it_aktualny;
  // std::list<int>::iterator it;
  int odleglosc_a_aktualny = 0;
  int odleglosc_chwilowa = 1000000;
  int odleglosc_sprawdzana = 1000000;
  Przystanek *poprzedni = a;
  Przystanek *aktualny = a;
  // Przystanek *sukces = nullptr;
  vector<Przystanek*> sasiedzi = podajSasiadow(a->stop_id);
  bool sasiad_odwiedzony = false;
  int sasiad_indeks;
  int i;

  vector<Przystanek*> rodzice(przystanki.size(), new Przystanek());
  vector<int> odleglosci(przystanki.size(), 1000000);


  otwarte.push_back(a);
  while (!otwarte.empty()) {
    i = 0;
    for (it = otwarte.begin(); it!=otwarte.end(); ++it,++i) {
      odleglosc_chwilowa = odleglosc_a_aktualny + heuresticFun((*it)->stop_id, b->stop_id) + macierz_sasiedzctw[znajdz_index_po_stop_id(poprzedni->stop_id)][znajdz_index_po_stop_id((*it)->stop_id)];
      if (odleglosc_chwilowa <= odleglosc_sprawdzana) {
        aktualny = *it;
        it_aktualny = it;
        //odleglosc_sprawdzana = odleglosc_chwilowa;
        // cout << "A" << odleglosc_a_aktualny << " H" << heuresticFun((*it)->stop_id, b->stop_id) << " M" << macierz_sasiedzctw[znajdz_index_po_stop_id(poprzedni->stop_id)][znajdz_index_po_stop_id((*it)->stop_id)] << "\n";

      }
    }
    cout << "\n----------------------------------\nnajblizszy ch" << odleglosc_chwilowa <<  " akt" << aktualny->stop_id << "\n";
    cout << "sasiedzi";
    vector<Przystanek*> p;
    p = podajSasiadow(aktualny->stop_id);
    for (unsigned int i=0; i<p.size(); ++i)
      cout << "\n\t" << p[i]->stop_id << p[i]->stop_name << " & ";
    cout << "\n";
    cout << "otwarte";
    for (auto list_otwarte : otwarte)
      cout << "\n\t" << list_otwarte->stop_id << list_otwarte->stop_name << " & ";
    cout << "\n";

    // if (aktualny->stop_id == b->stop_id)
    //   break;

    zamkniete.push_back(aktualny);
    otwarte.erase(find(otwarte.begin(), otwarte.end(), aktualny));
    for (auto przystanek : podajSasiadow(aktualny->stop_id))
      sasiedzi.push_back(przystanek);

    for(unsigned int i=0; i<sasiedzi.size(); ++i) {
      for (unsigned int j=0; j<zamkniete.size(); ++j) {
        if ( sasiedzi[i] == zamkniete[j])
          sasiad_odwiedzony = true;
      }
      odleglosc_chwilowa = odleglosc_sprawdzana + macierz_sasiedzctw[znajdz_index_po_stop_id(poprzedni->stop_id)][znajdz_index_po_stop_id(sasiedzi[i]->stop_id)];
      sasiad_indeks = znajdz_index_po_stop_id(sasiedzi[i]->stop_id);

      if (!sasiad_odwiedzony) {
        otwarte.push_back(sasiedzi[i]);
    } else {
        if (odleglosc_chwilowa < odleglosci[sasiad_indeks + 1]) {
          odleglosci[sasiad_indeks] = odleglosc_chwilowa;
          rodzice[sasiad_indeks] = aktualny;
          sasiad_odwiedzony = false;
          odleglosc_a_aktualny += odleglosc_chwilowa;
          poprzedni = aktualny;
        }
      }

      // cout << "\nsasiad:\t" << i << "\t" << sasiedzi[i]->stop_id << "\n";
      // cout << "for2\n" << "aktualny" << aktualny->stop_id << "\n";
    }
    sasiad_odwiedzony = false;

    // cout << "zamkniete\n";
    // if (zamkniete.size() != 0)
    //   for (unsigned int i=0; i<zamkniete.size(); ++i)
    //     cout << zamkniete[i]->stop_id << " ";
    // cout << "otwarte\n";
    // for (unsigned int i=0; i<otwarte.size(); ++i)
    //   cout << ( otwarte.pop_front() )->stop_id << " ";
  }
  cout << "astar out\n";

  return rodzice;
}


/*vector<Przystanek*> Graf::astar(int a_stop_id, int b_stop_id) {

  Przystanek *a = przystanki[znajdz_index_po_stop_id(a_stop_id)];
  Przystanek *b = przystanki[znajdz_index_po_stop_id(b_stop_id)];

  vector<Przystanek*> zamkniete;
  list<Przystanek*> otwarte;
  std::list<Przystanek*>::iterator it, it_nieodw;
  // std::list<int>::iterator it;
  int odleglosc_a_aktualny = 0;
  int odleglosc_chwilowa = 1000000;
  int odleglosc_sprawdzana = 1000000;
  Przystanek *poprzedni = a;
  Przystanek *aktualny = a;
  vector<Przystanek*> sasiedzi = podajSasiadow(a->stop_id);
  bool sasiad_odwiedzony = false;
  int sasiad_indeks;
  int i;

  vector<Przystanek*> rodzice(przystanki.size(), new Przystanek());
  vector<int> odleglosci(przystanki.size(), 1000000);


  otwarte.push_back(a);
  while (otwarte.size() > 0 && aktualny->stop_id != b->stop_id) {
    i = 0;
    for (it = otwarte.begin(); it!=otwarte.end(); ++it,++i) {
      odleglosc_chwilowa = odleglosc_a_aktualny + heuresticFun((*it)->stop_id, b->stop_id) + macierz_sasiedzctw[znajdz_index_po_stop_id(poprzedni->stop_id)][znajdz_index_po_stop_id((*it)->stop_id)];
      if (odleglosc_chwilowa < odleglosc_sprawdzana) {
        aktualny = *it;
        odleglosc_sprawdzana = odleglosc_chwilowa;
        it_nieodw = it;
        break;
      }
    }

    zamkniete.push_back(aktualny);
    otwarte.erase(it_nieodw);
    sasiedzi = podajSasiadow(aktualny->stop_id);

    for(unsigned int i=0; i<sasiedzi.size(); ++i) {
      for (unsigned j=0; j<zamkniete.size(); ++j) {
        if ( sasiedzi[i] == zamkniete[j])
          sasiad_odwiedzony = true;
          //cout << "for3";
      }
      odleglosc_chwilowa = odleglosc_sprawdzana + macierz_sasiedzctw[znajdz_index_po_stop_id(poprzedni->stop_id)][znajdz_index_po_stop_id(sasiedzi[i]->stop_id)];
      sasiad_indeks = znajdz_index_po_stop_id(sasiedzi[i]->stop_id);
      if (!sasiad_odwiedzony) {
        otwarte.push_back(sasiedzi[i]);
        rodzice[sasiad_indeks] = aktualny;
        odleglosci[sasiad_indeks] = odleglosc_chwilowa;
    } else {
        if (odleglosc_chwilowa < odleglosci[sasiad_indeks + 1]) {
          odleglosci[sasiad_indeks] = odleglosc_sprawdzana + odleglosc_chwilowa;
          rodzice[sasiad_indeks] = aktualny;
        }
        sasiad_odwiedzony = false;
      }
      odleglosc_a_aktualny += macierz_sasiedzctw[znajdz_index_po_stop_id(aktualny->stop_id)][znajdz_index_po_stop_id(poprzedni->stop_id)];
      poprzedni = aktualny;
      cout << "\nsasiad:\t" << i << "\t" << sasiedzi[i]->stop_id << "\n";
      // cout << "for2\n" << "aktualny" << aktualny->stop_id << "\n";
    }
    // cout << "while\n";

    // cout << "zamkniete\n";
    // if (zamkniete.size() != 0)
    //   for (unsigned int i=0; i<zamkniete.size(); ++i)
    //     cout << zamkniete[i]->stop_id << " ";
    // cout << "otwarte\n";
    // for (unsigned int i=0; i<otwarte.size(); ++i)
    //   cout << ( otwarte.pop_front() )->stop_id << " ";
  }
  return rodzice;
}*/
