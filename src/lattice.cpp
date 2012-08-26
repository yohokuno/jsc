#include "lattice.h"

namespace jsc {

Lattice::Lattice() {
}

Lattice::~Lattice() {
}

bool Lattice::CreateLattice(vector<vector<Ngram> > &ngrams, string &key, bool reverse) {
  key_ = key;
  end_nodes_.resize(ngrams.size() + 2);

  for (uint32 i = 0; i < ngrams.size(); i++) {
    for (uint32 j = 0; j < ngrams[i].size(); j++) {
      Node node(ngrams[i][j], reverse);
      uint32 position = i + node.GetTotalLength();
      end_nodes_[position].push_back(node);
    }
  }
  AddBosNodes();
  AddUnknownNodes();
  return true;
}

bool Lattice::AddBosNodes() {
  Node bos;
  bos.source.push_back("_");
  bos.target.push_back("BOS");
  end_nodes_.front().push_back(bos);  // BOS node
  end_nodes_.back().push_back(bos);   // EOS node
  return true;
}

bool Lattice::AddUnknownNodes() {
  for (uint32 i = 0; i < end_nodes_.size(); i++) {
    if (end_nodes_[i].size() == 0) {
      Node unknown;
      string s(1, key_[i-1]);
      unknown.source.push_back(s);
      unknown.target.push_back(s);
      end_nodes_[i].push_back(unknown);
    }
  }
  return true;
}

vector<Node> &Lattice::GetList(uint32 position) {
  return end_nodes_[position];
}
uint32 Lattice::GetListSize(uint32 position) {
  return end_nodes_[position].size();
}
Node &Lattice::GetNode(uint32 position, uint32 index) {
  return end_nodes_[position][index];
}
uint32 Lattice::GetSize() {
  return (uint32) end_nodes_.size();
}

}

