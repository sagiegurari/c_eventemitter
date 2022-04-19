#include "test.h"

int _test_global_counter       = 0;
int _test_global_order_counter = 0;


void _test_cb0(void *event_data, void *context)
{
  assert_string_equal((char *)event_data, "event");
  assert_string_equal((char *)context, "test0");
  assert_num_equal(_test_global_order_counter, 0);

  _test_global_counter++;
  _test_global_order_counter++;
}


void _test_cb1(void *event_data, void *context)
{
  assert_string_equal((char *)event_data, "event");
  assert_string_equal((char *)context, "test1");
  assert_num_equal(_test_global_order_counter, 1);

  _test_global_counter++;
  _test_global_order_counter++;
}


void _test_cb2(void *event_data, void *context)
{
  assert_string_equal((char *)event_data, "event");
  assert_string_equal((char *)context, "test2");
  assert_num_equal(_test_global_order_counter, 2);

  _test_global_counter++;
  _test_global_order_counter++;
}


void test_impl()
{
  struct EventEmitter *event_emitter = eventemitter_new();

  unsigned int        id = eventemitter_add_listener(event_emitter, -1, _test_cb1, "test1");

  assert_num_equal(id, 1);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 1);

  id = eventemitter_prepend_listener(event_emitter, -1, _test_cb0, "test0");
  assert_num_equal(id, 2);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 2);

  id = eventemitter_on(event_emitter, -1, _test_cb2, "test2");
  assert_num_equal(id, 3);
  assert_num_equal(eventemitter_listeners_count(event_emitter, -1), 3);

  _test_global_order_counter = 0;
  int count = eventemitter_emit(event_emitter, -1, "event");
  assert_num_equal(count, 3);
  assert_num_equal(_test_global_counter, 3);
  assert_num_equal(_test_global_order_counter, 3);

  _test_global_order_counter = 0;
  count                      = eventemitter_emit(event_emitter, -1, "event");
  assert_num_equal(count, 3);
  assert_num_equal(_test_global_counter, 6);
  assert_num_equal(_test_global_order_counter, 3);

  eventemitter_release(event_emitter);
}


int main()
{
  test_run(test_impl);
}

