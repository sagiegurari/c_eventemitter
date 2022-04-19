#include "test.h"


void test_impl()
{
  struct EventEmitter *event_emitter = eventemitter_new();

  eventemitter_release(event_emitter);
}


int main()
{
  test_run(test_impl);
}

