#ifndef __TEST_H__
#define __TEST_H__

#include "eventemitter.h"
#include <stdlib.h>

void test_run(void (*fn)());
void test_fail();
void assert_true(bool);

void assert_num_equal(size_t, size_t);
void assert_string_equal(char *, char *);

#endif

