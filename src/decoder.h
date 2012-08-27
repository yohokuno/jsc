#ifndef DECODER_H
#define DECODER_H

#include "jsc.h"

namespace jsc {

// Decodes source string into target string
//
// Decode() method works as:
//  1) Search substring using trie
//  2) Create lattice from search result
//  3) Find the lowest cost path by Viterbi algorithm.

class Decoder {
 public:
  Decoder();
  virtual ~Decoder();

  bool Decode(string input, vector<Node> &nodes, bool label=true);

  // Internal functions for Decode() method.
  bool SearchSubString(string &input, vector<vector<Ngram> > &result, bool label);

  // Viterbi algorithm has 2 steps:
  //  1) Forward: calculate cost of nodes in lattice from left to righ
  //  2) Backward: search the best path using the total cost
  bool Viterbi(Lattice &lattice, vector<Node> &result);

  static Decoder &GetDecoder();

 private:
  Model &model_;
};

string ToStringPlain(vector<Node> &nodes);
string ToStringSegment(vector<Node> &nodes);
string ToStringDebug(vector<Node> &nodes);

}
#endif

