#include "jsc.h"

namespace jsc {

Lattice::Lattice() {
}

Lattice::~Lattice() {
}

bool Lattice::CreateLattice(vector<vector<Ngram> > &ngrams, string &key) {
  end_nodes_.resize(ngrams.size() + 2);

  for (uint32_t i = 0; i < ngrams.size(); i++) {
    for (uint32_t j = 0; j < ngrams[i].size(); j++) {
      Node node(ngrams[i][j]);
      uint32_t position = i + node.GetTotalLength();
      end_nodes_[position].push_back(node);
    }
  }
  AddBosNodes();
  AddUnknownNodes(key);
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

bool Lattice::AddUnknownNodes(string key) {
  for (uint32_t i = 0; i < end_nodes_.size(); i++) {
    if (end_nodes_[i].size() == 0) {
      Node unknown;
      string s(1, key[i-1]);
      unknown.source.push_back(s);
      unknown.target.push_back(s);
      unknown.cost = USHRT_MAX;
      end_nodes_[i].push_back(unknown);
    }
  }
  return true;
}

vector<Node> &Lattice::GetList(uint32_t position) {
  return end_nodes_[position];
}
uint32_t Lattice::GetListSize(uint32_t position) {
  return end_nodes_[position].size();
}
Node &Lattice::GetNode(uint32_t position, uint32_t index) {
  return end_nodes_[position][index];
}
uint32_t Lattice::GetSize() {
  return (uint32_t) end_nodes_.size();
}

}

