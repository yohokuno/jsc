#include <sstream>
#include <string.h>
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
  oss << "\n";
  return oss.str();
}

void Run(const char *prefix, char *format) {
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
    decoder.Decode(line, result);
    if (strcmp(format, "debug") == 0) {
      cout << ToStringDebug(result);
    } else if (strcmp(format, "plain") == 0) {
      cout << ToStringPlain(result);
    } else if (strcmp(format, "pair") == 0) {
      cout << line << "\t" << ToStringPlain(result);
    }
  }

  model.Clear();
}

int main(int argc, char **argv) {
  string directory = "data";
  char *format = (char*)"plain";

  if (argc > 1)
    directory = argv[1];
  if (argc > 2)
    format = argv[2];

  if (*directory.end() != '/' && *directory.end() != '\\') {
    directory += '/';
  }

  Run(directory.c_str(), format);

  return 0;
}

