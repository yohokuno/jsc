#include <iostream>
#include <unistd.h>
#include "namespace.h"
#include "builder.h"

int main(int argc, char **argv) {
  string prefix = "data/";
  bool reverse = false;
  int c;
  while ((c = getopt (argc, argv, "d:r")) != -1) {
    switch (c) {
      case 'd':
        prefix = optarg;
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
    jsc::Builder builder;
    const string model = "ngram";
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