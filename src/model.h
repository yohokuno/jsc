#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <marisa.h>
#include "namespace.h"
#include "ngram.h"

namespace jsc {

// Model consists of four data structures as below:
//   Source trie: compressed trie of source side string
//   Ngram trie: compressed trie of joint n-gram string
//   Offsets array: array of offset in entry array
//   Entry array: ngram cost, backoff cost and ngram ID in Ngram Trie).
//
// Actual search works like this:
//   1) Search from source trie and get IDs in the trie.
//   2) Get offset from array like: offset[ID]
//   3) Get entries from array from entries[offset] to entries[offset+size-1].
//   4) Get ngram by reverse lookup from ngram trie, using ngram ID in the entry.
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
  uint32 GetSize() { return (uint32)entries_.size(); }

  // Get reference from global object.
  static Model &GetModel();

private:
  void PushResult(uint32 source_id, string &query, vector<Ngram> &result);
  marisa::Trie source_trie_;
  marisa::Trie ngram_trie_;
  vector<Entry> entries_;
  vector<uint32> offsets_;
};

}

#endif

