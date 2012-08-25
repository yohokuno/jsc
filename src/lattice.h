#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include "namespace.h"
#include "type.h"
#include "node.h"

namespace jsc {

// Lattice is list of list for Viterbi algorithm.
// Outer list is indexed by the end position in the input string.

class Lattice {
 public:
  Lattice();
  virtual ~Lattice();
  bool CreateLattice(vector<vector<Ngram> > &ngrams, string &key, bool reverse=false);
  bool AddBosNodes();
  // Currently, unknown characters are recognized as 1 byte words.
  // TODO: use UTF-8 character (3 byte) as unknown node.
  bool AddUnknownNodes();
  vector<Node> &GetList(uint32 position);
  uint32 GetListSize(uint32 position);
  Node &GetNode(uint32 position, uint32 index);
  uint32 GetSize();

 private:
  vector<vector<Node> > end_nodes_;
  string key_;
};

}
#endif

