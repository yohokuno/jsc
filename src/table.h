#ifndef TABLE_H
#define TABLE_H

namespace jsc {

// Table implements rule-based conversion such as
// romaji-to-kana conversion etc.

class Table : private map<string, string> {
public:
  bool Load(string filename);
  string Convert(string input);
};

}

#endif

