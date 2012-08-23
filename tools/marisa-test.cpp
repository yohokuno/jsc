#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sstream>
#include <marisa.h>
#include "marisa-assert.h"

// Assert marisa::Key object
#define ASSERT_KEY(key, str) ASSERT(strncmp((key).ptr(), (str), (key).length()) == 0)

// Test small alphabet dictionary
void AlphabetTest() {
  TEST_START();
  
  // build trie from test data
  marisa::Trie trie;
  marisa::Keyset keyset;
  keyset.push_back("trie");
  keyset.push_back("tree");
  keyset.push_back("try");
  keyset.push_back("tries");
  trie.build(keyset);
  
  // test lookup
  marisa::Agent agent;
  agent.set_query("trie");
  ASSERT( trie.lookup(agent) == true );
  ASSERT( strcmp( agent.key().ptr(), "trie" ) == 0 );

  size_t id = agent.key().id();
  agent.clear();
  agent.set_query(id);
  trie.reverse_lookup(agent);
  ASSERT( strncmp( agent.key().ptr(), "trie", agent.key().length() ) == 0 );

  agent.clear();
  agent.set_query("triton");
  ASSERT( trie.lookup(agent) == false );
  

  // test common prefix search
  agent.clear();
  agent.set_query("tries");
  ASSERT( trie.common_prefix_search(agent) == true );
  ASSERT( strncmp(agent.key().ptr(), "trie", agent.key().length()) == 0 );
  ASSERT( trie.common_prefix_search(agent) == true );
  ASSERT( strncmp(agent.key().ptr(), "tries", agent.key().length()) == 0 );
  ASSERT( trie.common_prefix_search(agent) == false );
  
  // test predictive search
  agent.clear();
  agent.set_query("tr");
  ASSERT( trie.predictive_search(agent) == true );
  ASSERT( strncmp(agent.key().ptr(), "trie", agent.key().length()) == 0 );
  ASSERT( trie.predictive_search(agent) == true );  
  ASSERT( strncmp(agent.key().ptr(), "tries", agent.key().length()) == 0 );
  ASSERT( trie.predictive_search(agent) == true );
  ASSERT( strncmp(agent.key().ptr(), "tree", agent.key().length()) == 0 );
  ASSERT( trie.predictive_search(agent) == true );
  ASSERT( strncmp(agent.key().ptr(), "try", agent.key().length()) == 0 );
  ASSERT( trie.predictive_search(agent) == false );
 
  // test reverse_lookup
  agent.clear();
  agent.set_query((size_t)0);
  trie.reverse_lookup(agent);
  ASSERT( strncmp(agent.key().ptr(), "trie", agent.key().length()) == 0 );
  agent.set_query(1);
  trie.reverse_lookup(agent);
  ASSERT( strncmp(agent.key().ptr(), "tree", agent.key().length()) == 0 );
  agent.set_query(2);
  trie.reverse_lookup(agent);
  ASSERT( strncmp(agent.key().ptr(), "try", agent.key().length()) == 0 );
  agent.set_query(3);
  trie.reverse_lookup(agent);
  ASSERT( strncmp(agent.key().ptr(), "tries", agent.key().length()) == 0 );
  agent.set_query(4);
  EXCEPT(trie.reverse_lookup(agent), MARISA_BOUND_ERROR);
  
  // test reverse_lookup -> lookup combination
  for (size_t i = 0; i < trie.size(); i++) {
    // reverse lookup
    agent.clear();
    agent.set_query(i);
    trie.reverse_lookup(agent);
    std::string key = agent.key().ptr();
    key.resize(agent.key().length());

    // lookup
    agent.clear();
    agent.set_query(key.c_str());
    trie.lookup(agent);
    ASSERT( strncmp( agent.key().ptr(), key.c_str(), agent.key().length() ) == 0 );
  }

  TEST_END();
}

// Test small Japanese dictionary
// Literals of Japanese string are described as hex number.
// Comments must be surrounded by double quote.
// For example, "あ" is described as "\xe3\x81\x82" in a literal.
void JapaneseTest() {
  TEST_START();
  
  // build trie from test data
  marisa::Trie trie;
  marisa::Keyset keyset;
  marisa::Agent agent;
  keyset.push_back("\xe3\x81\x82");               // "あ"
  keyset.push_back("\xe3\x81\x82\xe3\x81\x84");   // "あい"
  keyset.push_back("\xe3\x81\x82\xe3\x81\x86");   // "あう"
  trie.build(keyset);
  
  // test lookup
  agent.clear();
  agent.set_query("\xe3\x81\x82");                // "あ"
  ASSERT( trie.lookup(agent) == true );
  ASSERT_KEY(agent.key(), "\xe3\x81\x82");        // "あ"
 
  agent.clear();
  agent.set_query("\xe3\x81\x86");                // "う"
  ASSERT( trie.lookup(agent) == false );
  

  // test common prefix search
  agent.clear();
  agent.set_query("\xe3\x81\x82\xe3\x81\x84\xe3\x81\x86");  // "あいう"
  ASSERT( trie.common_prefix_search(agent) == true );
  ASSERT_KEY( agent.key(), "\xe3\x81\x82");                 // "あ"
  ASSERT( trie.common_prefix_search(agent) == true );
  ASSERT_KEY( agent.key(), "\xe3\x81\x82\xe3\x81\x84");     // "あい"
  ASSERT( trie.common_prefix_search(agent) == false );
  
  // test predictive search
  agent.clear();
  agent.set_query("\xe3\x81\x82");                          // "あ"
  ASSERT( trie.predictive_search(agent) == true );  
  ASSERT_KEY( agent.key(), "\xe3\x81\x82");                 // "あ"
  ASSERT( trie.predictive_search(agent) == true );
  ASSERT_KEY( agent.key(), "\xe3\x81\x82\xe3\x81\x84");     // "あい"
  ASSERT( trie.predictive_search(agent) == true );
  ASSERT_KEY( agent.key(), "\xe3\x81\x82\xe3\x81\x86");     // "あう"
  ASSERT( trie.predictive_search(agent) == false );
 
  // test reverse_lookup
  agent.clear();
  agent.set_query((size_t)0);
  trie.reverse_lookup(agent);   // void cannot be tested.
  ASSERT_KEY( agent.key(), "\xe3\x81\x82");                 // "あ"

  agent.clear();
  agent.set_query(1);
  trie.reverse_lookup(agent);   // void cannot be tested.
  ASSERT_KEY( agent.key(), "\xe3\x81\x82\xe3\x81\x84");     // "あい"
  agent.set_query(3);
  EXCEPT(trie.reverse_lookup(agent), MARISA_BOUND_ERROR);
  
  
  // test reverse_lookup -> lookup combination
  for (size_t i = 0; i < trie.size(); i++) {
    // reverse lookup
    agent.clear();
    agent.set_query(i);
    trie.reverse_lookup(agent);
    std::string key = agent.key().ptr();
    key.resize(agent.key().length());

    // lookup
    agent.clear();
    agent.set_query(key.c_str());
    trie.lookup(agent);
    ASSERT( strncmp( agent.key().ptr(), key.c_str(), agent.key().length() ) == 0 );
  }
  TEST_END();
}

std::string generate(int i) {
  std::string result;
  for (int j = 0; j < 4; j++) {
    result += (char)(i >> j % 256);
  }
  return result;
}
// Create large dicitonary automatically
// Automatically generated test cases are like:
//  {0x00, 0x00, 0x00, 0x00},
//  {0x01, 0x01, 0x01, 0x01},
//  {0x02, 0x02, 0x02, 0x02},
//  ...
void AutomaticTest() {
  TEST_START();
  
  // build trie from test data
  marisa::Trie trie;
  marisa::Keyset keyset;
  const size_t size = 1000;
  for (size_t i = 0; i < size; i++) {
    std::string key = generate(i);
    keyset.push_back(key.c_str(), key.length());
  }
  trie.build(keyset);
  ASSERT( trie.size() == size );
  
  // test lookup
  for (size_t i = 0; i < size; i++) {
    std::string key = generate(i);

    marisa::Agent agent;
    agent.set_query(key.c_str(), key.length());
    ASSERT( trie.lookup(agent) == true );
    ASSERT( strncmp( agent.key().ptr(), key.c_str(), agent.key().length() ) == 0 );
    size_t id = agent.key().id();
    
    // reverse lookup
    agent.clear();
    agent.set_query(id);
    trie.reverse_lookup(agent);
    ASSERT( strncmp( agent.key().ptr(), key.c_str(), agent.key().length() ) == 0 );
  }
  {
    marisa::Agent agent;
    agent.set_query("-1");
    ASSERT( trie.lookup(agent) == false );
  }

  // test common prefix search
  for (size_t i = 0; i < size; i++) {
    std::string key = generate(i);
    marisa::Agent agent;
    agent.set_query(key.c_str(), key.length());
    ASSERT( trie.common_prefix_search(agent) == true );
  }
  
  // test predictive search
  for (size_t i = 0; i < size; i++) {
    std::string key = generate(i);
    marisa::Agent agent;
    agent.set_query(key.c_str(), key.length());
    ASSERT( trie.predictive_search(agent) == true );
  }
 
  // test reverse_lookup -> lookup combination
  for (size_t i = 0; i < trie.size(); i++) {
    // reverse lookup
    marisa::Agent agent;
    agent.set_query(i);
    trie.reverse_lookup(agent);
    std::string key = agent.key().ptr();
    key.resize(agent.key().length());

    // lookup
    agent.clear();
    agent.set_query(key.c_str());
    trie.lookup(agent);
    ASSERT( strncmp( agent.key().ptr(), key.c_str(), agent.key().length() ) == 0 );
  }
  {
    marisa::Agent agent;
    agent.set_query(size+1);
    EXCEPT(trie.reverse_lookup(agent), MARISA_BOUND_ERROR);
  }
  

  TEST_END();
}

// Run all test case
int main() {
  AlphabetTest();
  JapaneseTest();
  AutomaticTest();
  return 0;
}

