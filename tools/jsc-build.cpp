#include "jsc.h"
using namespace jsc;

int main(int argc, char **argv) {
  string prefix = "data/";
  string model = "ngram";

  bool reverse = false;
  int c;
  while ((c = getopt (argc, argv, "d:m:r")) != -1) {
    switch (c) {
      case 'd':
        prefix = optarg;
        break;
      case 'm':
        model = optarg;
        break;
      case 'r':
        reverse = true;
        break;
      case '?':
        cerr << "Unknown option -" << optopt << endl;
        return 1;
    }
  }

  // build ngram binary file
  {
    Builder builder;
    cerr << "Building model..." << endl;
    bool result = builder.Build((prefix+model).c_str(), prefix.c_str(), reverse);
    if (result) {
      cerr << "Succeeded to build model." << endl;
    } else {
      cerr << "Failed to build model." << endl;
    }
    cerr << endl;
  }
  return 0;
}
