#include "jsc.h"
using namespace jsc;

void Run(string prefix, string format, bool label, string table_mode) {
  Model &model = Model::GetModel();

  if (!model.LoadFromBinary(prefix.c_str())) {
    cerr << "Model " << prefix << "is not found." << endl;
    return;
  }

  Decoder &decoder = Decoder::GetDecoder();
  Table table = Table();
  if (table_mode == "both" || table_mode == "on")
    table.Load(prefix + "table.txt");

  string line;
  while (getline(cin, line)) {
    vector<Node> result;
    if (table_mode == "off") {
      decoder.Decode(line, result, label);
    } else if (table_mode == "on") {
      line = table.Convert(line);
      decoder.Decode(line, result, label);
    } else if (table_mode == "both") {
      vector<Node> result1, result2;
      decoder.Decode(line, result1, label);
      line = table.Convert(line);
      decoder.Decode(line, result2, label);
      if (result1.size() == 0 || result2.size() == 0) continue;
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
    } else if (format == "segment") {
      cout << ToStringSegment(result) << endl;
    }
  }

  model.Clear();
}

int main(int argc, char **argv) {
  string prefix = "./";
  string format = "segment";
  string table_mode = "both";
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
        table_mode = optarg;
        break;
      case '?':
        cerr << "Unknown option -" << optopt << endl;
        return 1;
    }
  }

  Run(prefix, format, label, table_mode);

  return 0;
}

