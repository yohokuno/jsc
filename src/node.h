#ifndef NODE_H
#define NODE_H

#include <limits.h>
#include "namespace.h"
#include "util.h"
#include "assert.h"
#include "ngram.h"

namespace jsc {

struct Node {
  vector<string> source;
  vector<string> target;
  uint32 cost;
  uint32 backoff;
  uint32 total_cost;
  uint32 back_index;

  Node() {
    cost = 0;
    backoff = 0;
    total_cost = INT_MAX / 2 - 1;
    back_index = 0;
  }

  Node(Ngram &ngram) {
    vector<string> splited;
    split(ngram.ngram, ' ', splited);
    for (uint32 i = 0; i < splited.size(); i++) {
      if (splited[i] == "<s>" || splited[i] == "</s>" || splited[i] == "unk") {
        source.push_back("");
        target.push_back(splited[i]);
      } else {
        vector<string> pair;
        split(splited[i], '/', pair);
        ASSERT(pair.size() == 2);
        target.push_back(pair[0]);
        source.push_back(pair[1]);
      }
    }
    cost = ngram.cost;
    backoff = ngram.backoff;

    total_cost = INT_MAX / 2 - 1;
    back_index = 0;

    ASSERT(source.size() == target.size());
  }

  uint32 GetOrder() {
    return source.size();
  }
  uint32 GetLength() {
    return source.back().length();
  }
  uint32 GetTotalLength() {
    uint32 result = 0;
    for (uint32 i = 0; i < source.size(); i++)
      result += source[i].length();
    return result;
  }
  string GetTarget() {
    string result;
    for (uint32 i = 0; i < target.size(); i++)
      result += target[i];
    return result;
  }
  string GetContext() {
    string result;
    for (uint32 i = 0; i < target.size()-1; i++)
      result += target[i];
    return result;
  }
};

}
#endif

