#include "jsc.h"

namespace jsc {

Decoder::Decoder()
    : model_(Model::GetModel()) {
}

Decoder::~Decoder() {
}

bool Decoder::Decode(string input, vector<Node> &nodes, bool label) {
  vector<vector<Ngram> > ngrams;

  if (!SearchSubString(input, ngrams, label)) {
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

bool Decoder::SearchSubString(string &input, vector<vector<Ngram> > &result, bool label) {
  if (label) {
    input.insert(0, SOURCE_BEGIN);  // sentence beginning
    input.append(SOURCE_END);       // sentence ending
  }

  result.resize(input.size());

  for (uint32_t i = 0; i < input.size(); i++) {
    string query = input.substr(i);
    model_.CommonPrefixSearch(query, result[i]);
  }
  return true;
}
bool Decoder::Viterbi(Lattice &lattice, vector<Node> &result) {

  // Forward step
  for (uint32_t i = 1; i < lattice.GetSize(); i++) {
    for (uint32_t j = 0; j < lattice.GetListSize(i); j++) {
      Node &current = lattice.GetNode(i, j);

      uint32_t index = i - current.GetLength();
      if (lattice.GetListSize(index) == 0)
        continue;

      uint32_t best_cost = UINT_MAX;
      uint32_t best_index = 0;

      // search best cost
      for (uint32_t k = 0; k < lattice.GetListSize(index); k++) {
        Node &previous = lattice.GetNode(index, k);

        // check consistency
        size_t result = previous.GetNgram().find(current.GetNgramContext());
        if (result == string::npos)
          continue;

        uint32_t total_cost = previous.total_cost;

        // penalize if previous node does not overlapped to current node
        if (result == 0)
          total_cost += previous.backoff;

        if (total_cost < best_cost) {
          best_index = k;
          best_cost = total_cost;
        }
      }

      current.back_index = best_index;
      current.total_cost = best_cost + current.cost;
    }
  }

  // Backward step
  int32_t current = lattice.GetSize()-2;
  uint32_t position = lattice.GetNode(lattice.GetSize()-1, 0).back_index;

  while (current > 0 && lattice.GetListSize(current) > 0) {
    Node &node = lattice.GetNode(current, position);
    if (node.target.back() != LABEL_BEGIN && node.target.back() != LABEL_END)
      result.insert(result.begin(), node);
    position = node.back_index;
    current -= node.GetLength();
  }
  return true;
}

string ToStringPlain(vector<Node> &nodes) {
  ostringstream oss;
  for (int i = 0; i < (int) nodes.size(); i++) {
    oss << nodes[i].target.back();
  }
  return oss.str();
}

string ToStringSegment(vector<Node> &nodes) {
  ostringstream oss;
  for (int i = 0; i < (int) nodes.size(); i++) {
    oss << nodes[i].target.back() << " ";
  }
  return oss.str().substr();
}

string ToStringDebug(vector<Node> &nodes) {
  ostringstream oss;
  for (int i = 0; i < (int) nodes.size(); i++) {
    oss << "(";
    for (int j = 0; j < (int) nodes[i].source.size(); j++)
      oss << nodes[i].source[j] << " ";
    oss << ", ";
    for (int j = 0; j < (int) nodes[i].target.size(); j++)
      oss << nodes[i].target[j] << " ";
    oss  << ", "
      << nodes[i].cost << ", "
      << nodes[i].backoff << ", "
      << nodes[i].total_cost << ", "
      << nodes[i].back_index
      << ")\n";
  }
  return oss.str();
}


// static
static Decoder g_decoder;
Decoder &Decoder::GetDecoder() {
  return g_decoder;
}

}

