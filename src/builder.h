#ifndef BUILDER_H
#define BUILDER_H

namespace jsc {

// To build and save model file, use this class.
// At present, this class has no member variables and so we can change this pure static.
class Builder {
public:
  Builder() {}
  virtual ~Builder() {}
  
  // Building binary model works as below:
  //   1) Load n-gram model from text file in SRILM(ARPA) format
  //   2) Build source trie and save
  //   3) Build n-gram trie and save
  //   4) Lookup and add source_id and ngram_id for all ngram
  //   5) Sort source array by source_id
  //   6) Extract offset of source_id
  //   7) Save offset and token array as binary
  bool Build(const char *filename, const char *prefix, bool reverse=false);
};

}

#endif

