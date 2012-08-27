#ifndef UTIL_H
#define UTIL_H

namespace jsc {

using namespace std;

typedef signed char       int8_t;
typedef short             int16_t;
typedef int               int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;

#define INT_MAX   0x7FFFFFFF
#define INT_MIN   0x80000000
#define SHRT_MAX  0x7FFF
#define SHRT_MIN  0x8000
#define UINT_MAX  0xFFFFFFFFUL
#define USHRT_MAX 0xFFFFU
#define SCALE_COST      500.

#define PAIR_SEPARATOR  '/'
#define NGRAM_SEPARATOR ' '
#define LABEL_BEGIN     "<s>"
#define LABEL_END       "</s>"
#define LABEL_UNK       "unk"
#define SOURCE_BEGIN    "\x01"
#define SOURCE_END      "\x02"
#define SOURCE_UNK      "\x03"
#define MAX_COST        "-99"
#define SOURCE_TRIE     "source.trie"
#define NGRAM_TRIE      "pair.trie"
#define OFFSET_FILE     "offset.bin"
#define ENTRY_FILE      "entry.bin"
#define MODEL_FILE      "ngram"
#define DATA_DIRECTORY  "data/"

#define ASSERT(condition) \
  ((condition) || \
  ((cerr << __LINE__ << ": Assertion `" << #condition << "' failed." \
      << endl), exit(-1), 0))

#define TEST_START() \
  (cout << __FILE__ << ":" << __LINE__ << ": " << __FUNCTION__ << "(): ")

#define TEST_END() \
  (cout << "ok" << endl)

#define CHECK(condition) \
  ((condition) || \
  cerr << "CHECK " << __FILE__ << ":" << __LINE__ << ":" << #condition << endl)

inline void split(const string &input, char delim, vector<string> &result){
  istringstream iss(input);
  string temp;
  while(getline(iss, temp, delim)) {
    result.push_back(temp);
  }
}
}

#endif

