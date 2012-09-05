#ifndef TAbLE_H
#define TAbLE_H

namespace jsc {

class Table : private map<string, string> {
public:
  bool Load(string filename);
  string Convert(string input);
};

}

#endif

