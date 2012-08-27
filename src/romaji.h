#ifndef ROMAJI_H
#define ROMAJI_H

namespace jsc {

class Romaji : private map<string, string> {
public:
  bool Load(string filename);
  string Convert(string input);
};

}

#endif

