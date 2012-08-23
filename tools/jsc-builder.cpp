#include "namespace.h"
#include "builder.h"
#include <iostream>

int main(int argc, char **argv) {
  string directory = "data";
  if (argc > 1) {
    directory = argv[1];
  }
  if (*directory.end() != '/' && *directory.end() != '\\') {
    directory += '/';
  }
  cerr << "Directory: " << directory << endl;
  // build ngram binary file
  {
    jsc::Builder builder;
    const string model = "model.txt";
    cerr << "Building model..." << endl;
    bool result = builder.Build((directory+model).c_str(), directory.c_str());
    if (result) {
      cerr << "Succeeded to build model." << endl;
    } else {
      cerr << "Failed to build model." << endl;
    }
    cerr << endl;
  }
  return 0;
}
