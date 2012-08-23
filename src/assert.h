#ifndef ASSERT_H_
#define ASSERT_H_

#include <iostream>
#include <cstdlib>
#include "namespace.h"

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

#endif
