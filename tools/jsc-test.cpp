#include "jsc.h"
using namespace jsc;

#define TEST_DATA "data/test/"
#define TEST_MODEL "data/test/ngram"

// Build ngram binary from text file
void TestBuildNgram() {
  TEST_START();

  // build ngram
  Builder builder;
  if (!builder.Build(TEST_MODEL, TEST_DATA)) {
    cerr << "missing " << TEST_MODEL << endl;
    return;
  }
  
  TEST_END();
}

// Load model from binary file after build process
void TestModel() {
  TEST_START();

  Model &model = Model::GetModel();
  if (!model.LoadFromBinary(TEST_DATA)) {
    cerr << "missing " << TEST_DATA << endl;
    return;
  }
  {
    string input("a");
    vector<Ngram> result;
    model.ExactSearch(input, result);
    ASSERT(result.size() == 2);
    ASSERT(result[0].source == "a");
    ASSERT(result[0].ngram == "A/a");
    ASSERT(result[0].cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[0].backoff == (int) (-SCALE_COST * -1.));
    ASSERT(result[1].source == "a");
    ASSERT(result[1].ngram == "a/a");
    ASSERT(result[1].cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[1].backoff == (int) (-SCALE_COST * -2.));
  }

  model.Clear();
  TEST_END();
}

// Test decoder
void TestDecoder() {
  TEST_START();

  Decoder &decoder = Decoder::GetDecoder();
  Model &model = Model::GetModel();
  model.LoadFromBinary(TEST_DATA);

  {
    vector<Node> result;
    string input("a");
    ASSERT(decoder.Decode(input, result, false) == true);
    ASSERT(result.size() == 1);
    ASSERT(result[0].source.size() == 1);
    ASSERT(result[0].target.size() == 1);
    ASSERT(result[0].source[0] == "a");
    ASSERT(result[0].target[0] == "A");
    ASSERT(result[0].cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[0].backoff == (int) (-SCALE_COST * -1.));
    ASSERT(result[0].total_cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[0].back_index == 0);
  }
  {
    vector<Node> result;
    string input("ab");
    ASSERT(decoder.Decode(input, result, false) == true);
    ASSERT(result.size() == 2);
    ASSERT(result[0].source.size() == 1);
    ASSERT(result[0].target.size() == 1);
    ASSERT(result[0].source[0] == "a");
    ASSERT(result[0].target[0] == "a");
    ASSERT(result[0].cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[0].backoff == (int) (-SCALE_COST * -2.));
    ASSERT(result[0].total_cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[0].back_index == 0);

    ASSERT(result[1].source.size() == 2);
    ASSERT(result[1].target.size() == 2);
    ASSERT(result[1].source[0] == "a");
    ASSERT(result[1].target[0] == "a");
    ASSERT(result[1].source[1] == "b");
    ASSERT(result[1].target[1] == "B");
    ASSERT(result[1].cost == (int) (-SCALE_COST * -1.));
    ASSERT(result[1].total_cost == (int) (-SCALE_COST * -4.));
    ASSERT(result[1].backoff == 0);
    ASSERT(result[1].back_index == 1);
  }

  model.Clear();
  TEST_END();
}

// Test all case
int main() {
  TestBuildNgram();
  TestModel();
  TestDecoder();

  return 0;
}
