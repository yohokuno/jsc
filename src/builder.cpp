#include "jsc.h"

namespace jsc {

bool Builder::Build(const char *filename, const char *prefix, bool reverse) {

  // open text file
  fstream fs(filename);
  if (!fs.is_open())
    return false;

  string line;
  marisa::Keyset source_keyset;
  marisa::Keyset ngram_keyset;
  vector<Ngram> ngrams;

  // load ngram file
  while (getline(fs, line)) {
    // parse line
    vector<string> result;
    split(line, '\t', result);
    ASSERT(result.size() <= 3);
    
    // skip unnecessary line
    if (result.size() <= 1)
      continue;

    // parse n-gram
    Ngram ngram;
    ngram.ngram = result.at(1);
    vector<string> splited;
    bool flag = false;

    split(ngram.ngram, NGRAM_SEPARATOR, splited);
    for (uint32_t i = 0; i < splited.size(); i++) {
      if (splited[i] == LABEL_BEGIN) {
        ngram.source += SOURCE_BEGIN;
      } else if (splited[i] == LABEL_END) {
        ngram.source += SOURCE_END;
      } else if (splited[i] == LABEL_UNK) {
        ngram.source += SOURCE_UNK;
      } else {
        vector<string> pair;
        split(splited[i], PAIR_SEPARATOR, pair);
        if (pair.size() == 2) {
          if (!reverse)
            ngram.source += pair[1];
          else
            ngram.source += pair[0];
        } else
          flag = true;
      }
    }
    if (flag) continue;

    // parse cost
    if (result[0] == MAX_COST) {
      ngram.cost = USHRT_MAX;
    } else {
      double cost;
      stringstream(result[0]) >> cost;
      ngram.cost = (uint16_t) (-SCALE_COST * cost);
    }

    // parse backoff
    if (result.size() == 3) {
      if (result[0] == MAX_COST) {
        ngram.backoff = USHRT_MAX;
      } else {
        double backoff;
        stringstream(result[2]) >> backoff;
        ngram.backoff = (uint16_t) (-SCALE_COST * backoff);
      }
    } else {
      ngram.backoff = 0;
    }

    // add temporary data
    source_keyset.push_back(ngram.source.c_str(), ngram.source.length());
    ngram_keyset.push_back(ngram.ngram.c_str(), ngram.ngram.length());
    ngrams.push_back(ngram);
  }
  // build and save source trie
  marisa::Trie source_trie;
  source_trie.build(source_keyset);
  {
    string file(prefix);
    file.append(SOURCE_TRIE);
    source_trie.save(file.c_str());
  }

  // build and save ngram trie
  // Notice: ngram trie is used later for building entry array.
  marisa::Trie ngram_trie;
  ngram_trie.build(ngram_keyset);
  {
    string file(prefix);
    file.append(NGRAM_TRIE);
    ngram_trie.save(file.c_str());
  }

  // Each ngram needs ID in source trie for sort,
  // and needs ID in ngram_trie for reverse lookup.
  // So, both IDs are appended to Ngram after building both source and ngram tries.
  for (uint32_t i = 0; i < ngrams.size(); i++) {
    // lookup source_id from source_trie_
    {
      marisa::Agent agent;
      agent.set_query(ngrams[i].source.c_str());

      source_trie.lookup(agent);
      ngrams[i].source_id = agent.key().id();
    }
    // Check reverse_lookup can find original query
    {
      marisa::Agent agent;
      agent.set_query((size_t)ngrams[i].source_id);
      source_trie.reverse_lookup(agent);
      ASSERT(strncmp(ngrams[i].source.c_str(), agent.key().ptr(), agent.key().length()) == 0);
    }
    // lookup ngram_id from ngram_trie_
    {
      marisa::Agent agent;
      agent.set_query(ngrams[i].ngram.c_str(), ngrams[i].ngram.length());
      ngram_trie.lookup(agent);
      ngrams[i].ngram_id = agent.key().id();
    }
    // Check reverse_lookup can find original query
    {
      marisa::Agent agent;
      agent.set_query((size_t)ngrams[i].ngram_id);
      ngram_trie.reverse_lookup(agent);
      ASSERT(strncmp(ngrams[i].ngram.c_str(), agent.key().ptr(), agent.key().length()) == 0);
    }
  }

  // sort ngrams by source_id
  sort(ngrams.begin(), ngrams.end(), CompareNgram);

  // build entries and offsets
  uint32_t length = 0;
  uint32_t source_id = ngrams.at(0).source_id;
  vector<Entry> entries;
  vector<uint32_t> offsets;
  for (uint32_t i = 0; i < ngrams.size(); i++) {

    // initialize Entry element
    Ngram &ngram = ngrams.at(i);
    Entry entry = {ngram.cost, ngram.backoff, ngram.ngram_id};

    // add entry
    entries.push_back(entry);

    // if current source is different from previous source, add offset and reflesh length
    if (source_id != ngrams.at(i).source_id) {
      uint32_t offset = i-length;
      offsets.push_back(offset);
      length = 0;
      source_id = ngrams.at(i).source_id;
    }
    length ++;
  }
  // add last offset
  {
    uint32_t offset = ngrams.size()-length;
    offsets.push_back(offset);

    // We have to add entire size finally to calculate size of last ngram
    offsets.push_back(ngrams.size());
  }
  // save offset array as binary file
  {
    string file_offset(prefix);
    file_offset.append(OFFSET_FILE);
    ofstream ofs_offset(file_offset.c_str(), ios_base::out | ios_base::binary);

    for (uint32_t i = 0; i < offsets.size(); i++) {
      ofs_offset.write((const char*)&offsets.at(i), sizeof(offsets.at(i)));
    }
    ofs_offset.close();
  }
  // save Entry array as binary file
  {
    string file_entry(prefix);
    file_entry.append(ENTRY_FILE);
    ofstream ofs_entry(file_entry.c_str(), ios_base::out | ios_base::binary);

    for (uint32_t i = 0; i < entries.size(); i++) {
      ofs_entry.write((const char*)&entries.at(i), sizeof(entries.at(i)));
    }
    ofs_entry.close();
  }
  return true;
}
}

