#ifndef MODEL_H
#define MODEL_H

#include "jsc.h"

namespace jsc {

// Model consists of four data structures as below:
//   Source trie: compressed trie of source side string
//   Ngram trie: compressed trie of joint n-gram string
//   Offsets array: array of offset in entry array
//   Entries array: ngram cost, backoff cost and ngram ID in Ngram Trie
//
// Actual search works like this:
//   1) Search query from source trie and get source_id
//   2) Get start and end index by offsets[source_id], offsets[source_id+1]-1
//   3) Get entries from array from entries[start] to entries[end]
//   4) Get ngram by reverse lookup from ngram trie,by ngram_id
//   5) Return the dataset
//
class Model {
public:
  Model() {}
  virtual ~Model() {}

  // Input source string as query, and return ngram in various ways.
  // These functions delegates actual search to trie data structure.
  virtual void CommonPrefixSearch(string &query, vector<Ngram> &result);
  virtual void PredictiveSearch(string &query, vector<Ngram> &result);
  virtual void ExactSearch(string &query, vector<Ngram> &result);

  // Load dictionary from binary files under the prefix.
  // Binary dictionary consists of files source.trie, ngram.trie, offset.bin and entry.bin.
  bool LoadFromBinary(const char *prefix);
  void Clear();
  uint32_t GetSize() { return (uint32_t)entries_.size(); }

private:
  void PushResult(uint32_t source_id, string &query, vector<Ngram> &result);
  marisa::Trie source_trie_;
  marisa::Trie ngram_trie_;
  vector<Entry> entries_;
  vector<uint32_t> offsets_;
};

}

#endif

