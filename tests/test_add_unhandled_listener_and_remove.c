#include "test.h"

int _test_global_counter           = 0;
int _test_global_unhandled_counter = 0;


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


void _test_unhandled_cb(int event_id, void *event_data, void *context)
{
  assert_true(event_id == 200 || event_id == -2);
  assert_string_equal((char *)event_data, "else");
  assert_string_equal((char *)context, "unhandled");

  _test_global_unhandled_counter++;
}


void _test_unhandled_first_cb(int event_id, void *event_data, void *context)
{
  assert_true(event_id == 200 || event_id == -2);
  assert_string_equal((char *)event_data, "else");
  assert_string_equal((char *)context, "unhandled_first");

  _test_global_unhandled_counter++;
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

  id = eventemitter_once(event_emitter, -2, _test_cb12, "test2");
  assert_num_equal(id, 3);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -2), 1);

  id = eventemitter_add_unhandled_listener(event_emitter, _test_unhandled_cb, "unhandled");
  assert_num_equal(id, 4);
  id = eventemitter_prepend_unhandled_listener(event_emitter, _test_unhandled_first_cb, "unhandled_first");
  assert_num_equal(id, 5);
  id = eventemitter_else(event_emitter, _test_unhandled_cb, "unhandled");
  assert_num_equal(id, 6);

  int count = eventemitter_emit(event_emitter, -1, "event1");
  assert_num_equal(count, 2);
  assert_num_equal(_test_global_counter, 2);
  assert_num_equal(_test_global_unhandled_counter, 0);

  count = eventemitter_emit(event_emitter, 200, "else");
  assert_num_equal(count, 3);
  assert_num_equal(_test_global_counter, 2);
  assert_num_equal(_test_global_unhandled_counter, 3);

  count = eventemitter_emit(event_emitter, 200, "else");
  assert_num_equal(count, 3);
  assert_num_equal(_test_global_counter, 2);
  assert_num_equal(_test_global_unhandled_counter, 6);

  int removed = eventemitter_remove_unhandled_listener(event_emitter, 5);
  assert_num_equal(removed, 1);

  count = eventemitter_emit(event_emitter, 200, "else");
  assert_num_equal(count, 2);
  assert_num_equal(_test_global_counter, 2);
  assert_num_equal(_test_global_unhandled_counter, 8);

  assert_num_equal(eventemitter_listeners_count(event_emitter, -2), 1);
  count = eventemitter_emit(event_emitter, -2, "event1");
  assert_num_equal(count, 1);
  assert_num_equal(_test_global_counter, 3);
  assert_num_equal(_test_global_unhandled_counter, 8);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -2), 0);
  count = eventemitter_emit(event_emitter, -2, "else");
  assert_num_equal(count, 2);
  assert_num_equal(_test_global_counter, 3);
  assert_num_equal(_test_global_unhandled_counter, 10);

  eventemitter_release(event_emitter);
} /* test_impl */


int main()
{
  test_run(test_impl);
}

