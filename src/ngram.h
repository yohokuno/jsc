#ifndef NGRAM_H
#define NGRAM_H

#include <string>
#include "namespace.h"
#include "type.h"

namespace jsc {

// An interface of joint ngram.
// Search functions of Ngram return vector<Ngram> as result.

struct Ngram {
  string source;
  string ngram;
  uint16 cost;
  uint16 backoff;
  uint32 source_id;
  uint32 ngram_id;
};

// for sorting
inline bool CompareNgram (const Ngram &a, const Ngram &b) {
  return a.source_id < b.source_id;
}

// Information to lookup from source_id to cost, backoff and ngram_id.
struct Entry {
  uint16 cost;
  uint16 backoff;
  uint32 ngram_id;
};

}

#endif
