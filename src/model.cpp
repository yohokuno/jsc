#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>
#include "namespace.h"
#include "util.h"
#include "assert.h"
#include "model.h"

namespace jsc {

void Model::CommonPrefixSearch(string &query, vector<Ngram> &result) {
  marisa::Agent agent;
  agent.set_query(query.c_str());
  while (source_trie_.common_prefix_search(agent)) {
    uint32 source_id = agent.key().id();
    string source(agent.key().ptr(), agent.key().length());
    PushResult(source_id, source, result);
  }
}
void Model::PredictiveSearch(string &query, vector<Ngram> &result) {
  if (query.size()==0)
    return;
  marisa::Agent agent;
  agent.set_query(query.c_str());
  while (source_trie_.predictive_search(agent)) {
    uint32 source_id = agent.key().id();
    string source(agent.key().ptr(), agent.key().length());
    PushResult(source_id, source, result);
  }
}
void Model::ExactSearch(string &query, vector<Ngram> &result) {
  marisa::Agent agent;
  agent.set_query(query.c_str());

  if (!source_trie_.lookup(agent)) {
    return;
  }

  uint32 source_id = agent.key().id();
  PushResult(source_id, query, result);
}
void Model::PushResult(uint32 source_id, string &source, vector<Ngram> &result) { 
  ASSERT(source_id+1 < offsets_.size());

  uint32 offset = offsets_.at(source_id);
  uint32 next = offsets_.at(source_id+1);

  for (uint32 i = offset; i < next; i++) {
    Entry &entry = entries_.at(i);
    Ngram ngram;

    ngram.source = source;
    ngram.source_id = source_id;
    ngram.cost = entry.cost;
    ngram.backoff = entry.backoff;
    ngram.ngram_id = entry.ngram_id;

    {
      marisa::Agent agent;
      agent.set_query((size_t)entry.ngram_id);
      ngram_trie_.reverse_lookup(agent);
      ngram.ngram = string(agent.key().ptr()).substr(0,agent.key().length());
    }

    result.push_back(ngram);
  }
}
bool Model::LoadFromBinary(const char *prefix) {
  // load source trie
  try {
    string filename(prefix);
    filename.append("source.trie");
    source_trie_.load(filename.c_str());
  } catch (marisa::Exception) {
    return false;
  }
  // load ngram trie
  try {
    string filename(prefix);
    filename.append("ngram.trie");
    ngram_trie_.load(filename.c_str());
  } catch (marisa::Exception) {
    return false;
  }
  // load offset array
  {
    string filename(prefix);
    filename.append("offset.bin");
    ifstream ifs(filename.c_str(), ios_base::in | ios_base::binary);
    if (!ifs.is_open())
      return false;

    // prepare memory buffer
    uint32 size = source_trie_.size() + 1;
    offsets_.reserve(size);

    for (uint32 i = 0; i < size; i++) {
      uint32 offset;
      ifs.read((char*)&offset, sizeof(offset));
      offsets_.push_back(offset);
    }
    ifs.close();
  }
  // load entry array
  {
    string filename(prefix);
    filename.append("entry.bin");
    ifstream ifs(filename.c_str(), ios_base::in | ios_base::binary);
    if (!ifs.is_open())
      return false;

    // prepare memory buffer
    ifs.seekg(0, ios::end ); 
    streampos test = ifs.tellg();
    uint32 size = (uint32) ifs.tellg() / sizeof(Entry);
    ifs.seekg(0, ios::beg );
    entries_.reserve(size);

    for (uint32 i = 0; i < size; i++) {
      Entry entry;
      ifs.read((char*)&entry, sizeof(entry));
      entries_.push_back(entry);
    }
    ifs.close();
  }
  return true;
}

void Model::Clear() {
  source_trie_.clear();
  ngram_trie_.clear();
  entries_.clear();
  offsets_.clear();
}
// static
static Model g_model;
Model &Model::GetModel() {
  return g_model;
}

}

