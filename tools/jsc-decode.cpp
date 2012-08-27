#include "jsc.h"
using namespace jsc;

void Run(const char *prefix, string format, bool label) {
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
    decoder.Decode(line, result, label);
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
  int c;
  while ((c = getopt (argc, argv, "d:f:l")) != -1) {
    switch (c) {
      case 'l':
        label = false;
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

  Run(prefix.c_str(), format, label);

  return 0;
}

