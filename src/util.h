#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>
#include <vector>
#include "namespace.h"

namespace jsc {

inline void split(const string &input, char delim, vector<string> &result){
  istringstream iss(input);
  string temp;
  while(getline(iss, temp, delim)) {
    result.push_back(temp);
  }
}
}

#endif

