#include "test.h"

int _test_global_counter = 0;


void _test_cb11(void *event_data, void *context)
{
  assert_string_equal((char *)event_data, "event1");
  assert_string_equal((char *)context, "test1");

  _test_global_counter++;
}


void _test_cb12(void *event_data, void *context)
{
  assert_string_equal((char *)event_data, "event1");
  assert_string_equal((char *)context, "test2");

  _test_global_counter++;
}


void test_impl()
{
  struct EventEmitter *event_emitter = eventemitter_new();

  unsigned int        id = eventemitter_add_listener(event_emitter, -1, _test_cb11, "test1");

  assert_num_equal(id, 1);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 1);

  id = eventemitter_on(event_emitter, -1, _test_cb12, "test2");
  assert_num_equal(id, 2);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 2);

  id = eventemitter_add_listener(event_emitter, -2, _test_cb12, "test3");
  assert_num_equal(id, 3);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 2);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -2), 1);

  int count = eventemitter_emit(event_emitter, -1, "event1");
  assert_num_equal(count, 2);
  assert_num_equal(_test_global_counter, 2);

  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 2);
  int removed = eventemitter_remove_listener(event_emitter, -1, 1);
  assert_num_equal(removed, 1);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 1);

  count = eventemitter_emit(event_emitter, -1, "event1");
  assert_num_equal(count, 1);
  assert_num_equal(_test_global_counter, 3);

  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 1);
  removed = eventemitter_off(event_emitter, -1, 2);
  assert_num_equal(removed, 1);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 0);

  count = eventemitter_emit(event_emitter, -1, "event1");
  assert_num_equal(count, 0);
  assert_num_equal(_test_global_counter, 3);

  eventemitter_release(event_emitter);
} /* test_impl */


int main()
{
  test_run(test_impl);
}

