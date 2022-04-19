#include "test.h"


void test_impl()
{
  eventemitter_release(NULL);
}


int main()
{
  test_run(test_impl);
}

