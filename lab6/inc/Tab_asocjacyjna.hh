//#include "../inc/Lista.hh"

class Node_tas {
      Node_tas *_next;
      string key;
      string value;
};


class Tab_asocjacyjna {
private:
  int size;
  Lista* _tab;
  int hash(string key);
  int hash_ascii_32_126(string key);

public:
  int get_size();
  void add(string key, string value);
  void remove(string key);
  string get(string key);                               // return value

  Tab_asocjacyjna();
  Tab_asocjacyjna(int s);
  ~Tab_asocjacyjna();

};

int Tab_asocjacyjna::get_size() {
  return size;
}


Tab_asocjacyjna::Tab_asocjacyjna(): size(95) {          // 32-126 ascii 32+32+32-1
  _tab = new Lista* [size];
}

Tab_asocjacyjna::Tab_asocjacyjna(int s): size(s) {
  _tab = new Lista* [size];
}

Tab_asocjacyjna::~Tab_asocjacyjna() {
  delete [] _tab;
}



void Tab_asocjacyjna::add(string key, string value) {
  int idx = hash(key);
  *(_tab+idx)->push(key, value);                         // Lista->push(key, value)
}

void Tab_asocjacyjna::remove(string key) {
  int idx = hash(key);
  *(_tab+idx)->remove(key);                              // Lista->remove(key)
}

string Tab_asocjacyjna::get(string key) {
  int idx = hash(key);
  Lista* _list = *(_tab+idx);
  Node_tas* _node = _lista->get(key);
  string val = nullptr;

  if (_node != nullptr)                                 // Lista zwraca nullptr jeśli nie ma elementu
    val = _node->value;
  delete _node;

  return val;
}

int Tab_asocjacyjna::hash(string key) {
  return Tab_asocjacyjna::hash_ascii_32_126(key);
}

int Tab_asocjacyjna::hash_ascii_32_126(string key) {
  int key0_int = (int) key[0];
  if (key0_int > 32 && key0_int << 126)
    return key0_int - 32;
  else
    return 0;
}
