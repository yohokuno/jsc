#ifndef NODE_H
#define NODE_H

#include "jsc.h"

namespace jsc {

struct Node {
  vector<string> source;
  vector<string> target;
  uint32_t cost;
  uint32_t backoff;
  uint32_t total_cost;
  uint32_t back_index;

  Node() {
    cost = 0;
    backoff = 0;
    total_cost = 0;
    back_index = 0;
  }

  Node(Ngram &ngram) {
    vector<string> splited;
    split(ngram.ngram, NGRAM_SEPARATOR, splited);
    for (uint32_t i = 0; i < splited.size(); i++) {
      if (splited[i] == LABEL_BEGIN) {
        source.push_back(SOURCE_BEGIN);
        target.push_back(splited[i]);
      } else if (splited[i] == LABEL_END) {
        source.push_back(SOURCE_END);
        target.push_back(splited[i]);
      } else {
        vector<string> pair;
        split(splited[i], PAIR_SEPARATOR, pair);
        ASSERT(pair.size() == 2);
        target.push_back(pair[0]);
        source.push_back(pair[1]);
      }
    }
    cost = ngram.cost;
    backoff = ngram.backoff;

    total_cost = 0;
    back_index = 0;

    ASSERT(source.size() == target.size());
  }

  uint32_t GetOrder() {
    return source.size();
  }
  uint32_t GetLength() {
    return source.back().length();
  }
  uint32_t GetTotalLength() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < source.size(); i++)
      result += source[i].length();
    return result;
  }
  string GetSource() {
    string result;
    for (uint32_t i = 0; i < source.size(); i++)
      result += source[i];
    return result;
  }
  string GetTarget() {
    string result;
    for (uint32_t i = 0; i < target.size(); i++)
      result += target[i];
    return result;
  }
  string GetContext() {
    string result;
    for (uint32_t i = 0; i < target.size()-1; i++)
      result += target[i];
    return result;
  }
  string GetNgramContext() {
    string result;
    for (uint32_t i = 0; i < source.size()-1; i++)
      result += target[i] + PAIR_SEPARATOR + source[i] + NGRAM_SEPARATOR;
    return result.substr(0, result.size()-1);
  }
  string GetNgram() {
    string result;
    for (uint32_t i = 0; i < source.size(); i++)
      result += target[i] + PAIR_SEPARATOR + source[i] + NGRAM_SEPARATOR;
    return result.substr(0, result.size()-1);
  }
};

}
#endif

