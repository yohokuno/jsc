#include "decoder.h"

namespace jsc {

Decoder::Decoder()
    : model_(Model::GetModel()) {
}

Decoder::~Decoder() {
}

bool Decoder::Decode(string &input, vector<Node> &nodes) {
  vector<vector<Ngram> > ngrams;

  if (!SearchSubString(input, ngrams)) {
    return false;
  }

  Lattice lattice;

  if (!lattice.CreateLattice(ngrams, input)) {
    return false;
  }

  if (!Viterbi(lattice, nodes)) {
    return false;
  }

  return true;
}

bool Decoder::SearchSubString(string &input, vector<vector<Ngram> > &result) {
  input.insert(0,"");
  input.append("");
  result.resize(input.size());

  for (uint32 i = 0; i < input.size(); i++) {
    string query = input.substr(i);
    model_.CommonPrefixSearch(query, result[i]);
  }
  return true;
}
bool Decoder::Viterbi(Lattice &lattice, vector<Node> &result) {

  // Forward step
  for (uint32 i = 1; i < lattice.GetSize(); i++) {
    for (uint32 j = 0; j < lattice.GetListSize(i); j++) {
      Node &current = lattice.GetNode(i, j);

      uint32 index = i - current.GetLength();
      if (lattice.GetListSize(index) == 0)
        continue;

      uint32 best_cost = UINT_MAX;
      uint32 best_index = 0;

      for (uint32 k = 0; k < lattice.GetListSize(index); k++) {
        Node &previous = lattice.GetNode(index, k);

        // check consistency
        if (current.GetOrder() != 1 &&
            previous.GetTarget().find(current.GetContext()) == (size_t)-1)
          continue;

        // search best cost
        uint32 total_cost = previous.total_cost;

        if (best_cost == UINT_MAX || total_cost < best_cost) {
          best_index = k;
          best_cost = total_cost;
        }
      }

      lattice.GetNode(i, j).total_cost = best_cost + current.cost + current.backoff;
      lattice.GetNode(i, j).back_index = best_index;
    }
  }

  // Backward step
  int32 current = lattice.GetSize()-2;
  uint32 position = lattice.GetNode(lattice.GetSize()-1, 0).back_index;

  while (current > 0 && lattice.GetListSize(current) > 0) {
    Node &node = lattice.GetNode(current, position);
    if (node.target.back() != "<s>" && node.target.back() != "</s>")
      result.insert(result.begin(), node);
    position = node.back_index;
    current -= node.GetLength();
  }
  return true;
}

// static
static Decoder g_decoder;
Decoder &Decoder::GetDecoder() {
  return g_decoder;
}

}

