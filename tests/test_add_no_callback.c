#include "test.h"


void test_impl()
{
  struct EventEmitter *event_emitter = eventemitter_new();

  unsigned int        id = eventemitter_add_listener(event_emitter, 1, NULL, NULL);

  assert_num_equal(id, 0);
  id = eventemitter_on(event_emitter, 1, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_prepend_listener(event_emitter, 1, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_add_once_listener(event_emitter, 1, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_once(event_emitter, 1, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_prepend_once_listener(event_emitter, 1, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_add_unhandled_listener(event_emitter, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_else(event_emitter, NULL, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_prepend_unhandled_listener(event_emitter, NULL, NULL);
  assert_num_equal(id, 0);

  eventemitter_release(event_emitter);
}


int main()
{
  test_run(test_impl);
}

