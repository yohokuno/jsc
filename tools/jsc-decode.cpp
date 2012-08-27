#include <sstream>
#include <string.h>
#include <unistd.h>
#include "decoder.h"
#include "assert.h"
using namespace jsc;

string ToStringPlain(vector<Node> &nodes) {
  ostringstream oss;
  for (uint32 i = 0; i < nodes.size(); i++) {
    oss << nodes[i].target.back();
  }
  return oss.str();
}

string ToStringDebug(vector<Node> &nodes) {
  ostringstream oss;
  for (uint32 i = 0; i < nodes.size(); i++) {
    oss << "(";
    for (uint32 j = 0; j < nodes[i].source.size(); j++)
      oss << nodes[i].source[j] << " ";
    oss << ", ";
    for (uint32 j = 0; j < nodes[i].target.size(); j++)
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

void Run(const char *prefix, string format, bool label, bool reverse) {
  Model &model = Model::GetModel();
  cerr << "Now loading model...\n";

  if (!model.LoadFromBinary(prefix)) {
    cerr << "Model " << prefix << "is not found." << endl;
    return;
  }

  Decoder &decoder = Decoder::GetDecoder();

  cerr << "Input:\n";
  string line;

  while (getline(cin, line)) {
    vector<Node> result;
    decoder.Decode(line, result, label, reverse);
    if (format == "debug") {
      cout << ToStringPlain(result) << endl;
      cout << ToStringDebug(result) << endl;
    } else if (format == "plain") {
      cout << ToStringPlain(result) << endl;
    }
  }

  model.Clear();
}

int main(int argc, char **argv) {
  string prefix = "data/";
  string format = "plain";
  bool label = true;
  bool reverse = false;
  int c;
  while ((c = getopt (argc, argv, "d:f:lr")) != -1) {
    switch (c) {
      case 'l':
        label = false;
        break;
      case 'r':
        reverse = true;
        break;
      case 'd':
        prefix = optarg;
        break;
      case 'f':
        format = optarg;
        break;
      case '?':
        cerr << "Unknown option -" << optopt << endl;
        return 1;
    }
  }

  Run(prefix.c_str(), format, label, reverse);

  return 0;
}

