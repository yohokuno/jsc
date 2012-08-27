#include "jsc.h"
using namespace jsc;

void Run(string prefix, string format, bool label, string romaji) {
  Model &model = Model::GetModel();
  cerr << "Now loading model...\n";

  if (!model.LoadFromBinary(prefix.c_str())) {
    cerr << "Model " << prefix << "is not found." << endl;
    return;
  }

  Decoder &decoder = Decoder::GetDecoder();
  Romaji table = Romaji();
  if (romaji == "both" || romaji == "on")
    table.Load(prefix + "romaji.txt");

  cerr << "Input:\n";
  string line;

  while (getline(cin, line)) {
    vector<Node> result;
    if (romaji == "off") {
      decoder.Decode(line, result, label);
    } else if (romaji == "on") {
      line = table.Convert(line);
      decoder.Decode(line, result, label);
    } else if (romaji == "both") {
      vector<Node> result1, result2;
      decoder.Decode(line, result1, label);
      line = table.Convert(line);
      decoder.Decode(line, result2, label);
      if (result1.back().total_cost < result2.back().total_cost) {
        result = result1;
      } else {
        result = result2;
      }
      if (format == "debug") {
        cout << ToStringDebug(result1) << endl;
        cout << ToStringDebug(result2) << endl;
        cout << ToStringPlain(result) << endl;
        continue;
      }
    }
    if (format == "debug") {
      cout << ToStringDebug(result) << endl;
      cout << ToStringPlain(result) << endl;
    } else if (format == "plain") {
      cout << ToStringPlain(result) << endl;
    }
  }

  model.Clear();
}

int main(int argc, char **argv) {
  string prefix = "data/";
  string format = "plain";
  string romaji = "both";
  bool label = true;
  int c;
  while ((c = getopt (argc, argv, "d:f:r:l")) != -1) {
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
      case 'r':
        romaji = optarg;
        break;
      case '?':
        cerr << "Unknown option -" << optopt << endl;
        return 1;
    }
  }

  Run(prefix, format, label, romaji);

  return 0;
}

