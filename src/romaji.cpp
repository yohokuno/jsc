#include "jsc.h"

namespace jsc {

string Romaji::Convert(string input) {
  string output;
  for (int i = 0; i < (int)input.size(); i++) {
    bool flag = false;
    for (int j = 2; j >= 0; j--) {
      string query = input.substr(i, j+1);
      iterator result = this->find(query);
      if (result != this->end()) {
        output.append(result->second);
        i += j;
        flag = true;
        break;
      }
    }
    if (!flag) {
      output += input.at(i);
    }
  }
  return output;
}

bool Romaji::Load(string filename) {
  fstream fs(filename.c_str(), ios_base::in);
  if (!fs.is_open())
    return false;

  string line;
  while (getline(fs, line)) {
    vector<string> row;
    split(line, '\t', row);
    pair<string, string> p;
    p.first = row.at(0);
    p.second = row.at(1);
    insert(p);
  }
  return true;
}
}
