#include <sstream>
#include <string.h>
#include <unistd.h>
#include "decoder.h"
#include "assert.h"
using namespace jsc;

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
      << nodes[i].back_index << ", "
      << ")\n";
  }
  oss << "\n";
  return oss.str();
}
string ToStringPlain(vector<Node> &nodes) {
  ostringstream oss;
  for (uint32 i = 0; i < nodes.size(); i++) {
    oss << nodes[i].target.back();
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
      cout << ToStringDebug(result);
    } else if (format == "plain") {
      cout << ToStringPlain(result) << endl;
    }
  }

  model.Clear();
}

int main(int argc, char **argv) {
  string directory = "data";
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
        directory = optarg;
        break;
      case 'f':
        format = optarg;
        break;
      case '?':
        cerr << "Unknown option -" << optopt << endl;
        return 1;
    }
  }

  if (directory[directory.size()-1] != '/' && directory[directory.size()-1] != '\\') {
    directory += '/';
  }

  Run(directory.c_str(), format, label, reverse);

  return 0;
}

