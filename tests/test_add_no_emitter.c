#include "test.h"


void _test_cb(void *event_data, void *context)
{
  assert_true(event_data == NULL && context == NULL);
}


void _test_unhandled(int event_id, void *event_data, void *context)
{
  assert_true(event_id != 0 && event_data == NULL && context == NULL);
}


void test_impl()
{
  unsigned int id = eventemitter_add_listener(NULL, 1, _test_cb, NULL);

  assert_num_equal(id, 0);
  id = eventemitter_on(NULL, 1, _test_cb, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_prepend_listener(NULL, 1, _test_cb, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_add_once_listener(NULL, 1, _test_cb, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_once(NULL, 1, _test_cb, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_prepend_once_listener(NULL, 1, _test_cb, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_add_unhandled_listener(NULL, _test_unhandled, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_else(NULL, _test_unhandled, NULL);
  assert_num_equal(id, 0);
  id = eventemitter_prepend_unhandled_listener(NULL, _test_unhandled, NULL);
  assert_num_equal(id, 0);
}


int main()
{
  test_run(test_impl);
}

