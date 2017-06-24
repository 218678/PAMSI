#ifndef _PRZYSTANEK_HH
#define _PRZYSTANEK_HH

#include "../inc/libs.hh"

class Przystanek {
public:
  unsigned short stop_id;
  string stop_name;
  float stop_lat;
  float stop_lon;

  Przystanek() {
    stop_id = 0;
    stop_name = "0000";
    stop_lat = 0;
    stop_lon = 0;
  }

  friend fstream& operator>>(fstream& is, Przystanek *p) {
    string buffer = "0";
    std::string::size_type str_size_type;     // alias of size_t dla getline
    if(!is.eof()) { // zeby uniknac warningow "brak return'a na koncu" przy make'u - i tak nie wykona sie jesli nie ma na czym
      getline(is, buffer,',');
      if(is.eof()) // przerywa jeśli eof, tylko przy pierwszym getline'ie
        return is; //bo dane są sformatowane i jeśli nie złapie pierwszej zmiennej,
                   //nie czyta całego wiersza
      p->stop_id = stoi(buffer, &str_size_type);
      getline(is, buffer,','); // ignorujemy stop_code
      getline(is, p->stop_name,',');
      //cout << p->stop_name << "\n";
      getline(is, buffer,',');
      p->stop_lat = stof(buffer, &str_size_type);
      getline(is, buffer);
      p->stop_lon = stof(buffer, &str_size_type);
    }
    return is;
  }
};

#endif //_PRZYSTANEK_HH
