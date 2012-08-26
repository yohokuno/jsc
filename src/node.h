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
    total_cost = 0;
    back_index = 0;
  }

  Node(Ngram &ngram, bool reverse=false) {
    vector<string> splited;
    split(ngram.ngram, ' ', splited);
    for (uint32 i = 0; i < splited.size(); i++) {
      if (splited[i] == "<s>") {
        source.push_back("");
        target.push_back(splited[i]);
      } else if (splited[i] == "</s>") {
        source.push_back("");
        target.push_back(splited[i]);
      } else {
        vector<string> pair;
        split(splited[i], '/', pair);
        ASSERT(pair.size() == 2);
        if (!reverse) {
          target.push_back(pair[0]);
          source.push_back(pair[1]);
        } else {
          target.push_back(pair[1]);
          source.push_back(pair[0]);
        }
      }
    }
    cost = ngram.cost;
    backoff = ngram.backoff;

    total_cost = 0;
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
  string GetSource() {
    string result;
    for (uint32 i = 0; i < source.size(); i++)
      result += source[i];
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
  string GetNgramContext() {
    string result;
    for (uint32 i = 0; i < target.size()-1; i++)
      result += source[i] + "/" + target[i] + " ";
    return result.substr(0, result.size()-1);
  }
  string GetNgram() {
    string result;
    for (uint32 i = 0; i < source.size(); i++)
      result += source[i] + "/" + target[i] + " ";
    return result.substr(0, result.size()-1);
  }
};

}
#endif

