#ifndef NGRAM_H
#define NGRAM_H

#include "jsc.h"

namespace jsc {

// An interface of joint ngram.
// Search functions of Ngram return vector<Ngram> as result.

struct Ngram {
  string source;
  string ngram;
  uint16_t cost;
  uint16_t backoff;
  uint32_t source_id;
  uint32_t ngram_id;
};

// for sorting
inline bool CompareNgram (const Ngram &a, const Ngram &b) {
  return a.source_id < b.source_id;
}

// Information to lookup from source_id to cost, backoff and ngram_id.
struct Entry {
  uint16_t cost;
  uint16_t backoff;
  uint32_t ngram_id;
};

}

#endif
