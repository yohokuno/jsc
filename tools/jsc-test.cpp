#include "jsc.h"
using namespace jsc;

// Build ngram binary from text file
void TestBuildNgram() {
  TEST_START();

  // build ngram
  Builder builder;
  if (!builder.Build("test-data/ngram", "test-data/")) {
    cout << "File test-data/ngram is not found." << endl;
    return;
  }
  
  TEST_END();
}

// Load model from binary file after build process
void TestModel() {
  TEST_START();

  Model &model = Model::GetModel();
  if (!model.LoadFromBinary("test-data/")) {
    cout << "Directory test-data/ or binary files under the directory is not found." << endl;
    return;
  }
  string input("\xe3\x82\x8f\xe3\x81\x9f\xe3\x81\x97"); // "わたし"
  vector<Ngram> result;
  model.ExactSearch(input, result);
  ASSERT(result.size() == 1);
  ASSERT(result[0].source == "\xe3\x82\x8f\xe3\x81\x9f\xe3\x81\x97"); // "わたし"
  ASSERT(result[0].ngram == "\xe7\xa7\x81/\xe3\x82\x8f\xe3\x81\x9f\xe3\x81\x97"); // "私/わたし"
  ASSERT(result[0].cost == (int) (-500. * -1.021189));
  ASSERT(result[0].backoff == (int) (-500. * -0.3853509));

  model.Clear();
  TEST_END();
}

// Test decoder
void TestDecoder() {
  TEST_START();

  Decoder &decoder = Decoder::GetDecoder();
  Model &model = Model::GetModel();
  model.LoadFromBinary("test-data/");
  vector<Node> result;
  string input("\xe3\x82\x8f\xe3\x81\x9f\xe3\x81\x97"); // "わたし"
  ASSERT(decoder.Decode(input, result, false) == true);
  ASSERT(result.size() == 1);
  ASSERT(result[0].source.size() == 1);
  ASSERT(result[0].target.size() == 1);
  ASSERT(result[0].source[0] == "\xe3\x82\x8f\xe3\x81\x9f\xe3\x81\x97"); // "わたし"
  ASSERT(result[0].target[0] == "\xe7\xa7\x81");  // "私"
  ASSERT(result[0].cost == (int) (-500. * -1.021189));
  ASSERT(result[0].backoff == (int) (-500. * -0.3853509));
  ASSERT(result[0].total_cost == result[0].total_cost);
  ASSERT(result[0].back_index == 0);
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
