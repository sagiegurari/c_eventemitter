#include "eventemitter.h"
#include "vector.h"
#include <stdlib.h>

struct EventEmitter
{
  unsigned int  next_callback_id;
  struct Vector *event_listeners;
  struct Vector *unhandled_listeners;
};

struct EventEmitterEventListeners
{
  int           event_id;
  struct Vector *listeners;
};

struct EventEmitterEventListener
{
  unsigned int id;
  void         (*callback)(void *event_data, void *context);
  void         *context;
  bool         once;
};

struct EventEmitterUnhandledListener
{
  unsigned int id;
  void         (*callback)(int event_id, void *event_data, void *context);
  void         *context;
};

// private functions
static struct EventEmitterEventListeners *_eventemitter_get_listeners_for_event_id(struct EventEmitter *, int);
static unsigned int _eventemitter_add_listener(struct EventEmitter *, int, void (*callback)(void *, void *), void *, bool, bool);
static unsigned int _eventemitter_add_unhandled_listener(struct EventEmitter *, void (*callback)(int, void *, void *), void *, bool);

struct EventEmitter *eventemitter_new(void)
{
  struct EventEmitter *event_emitter = malloc(sizeof(struct EventEmitter));

  event_emitter->next_callback_id    = 1;
  event_emitter->event_listeners     = vector_new();
  event_emitter->unhandled_listeners = vector_new();

  return(event_emitter);
}


void eventemitter_release(struct EventEmitter *event_emitter)
{
  if (event_emitter == NULL)
  {
    return;
  }

  eventemitter_remove_all_listeners(event_emitter);
  vector_release(event_emitter->event_listeners);
  vector_release(event_emitter->unhandled_listeners);
  free(event_emitter);
}


unsigned int eventemitter_add_listener(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context)
{
  return(_eventemitter_add_listener(event_emitter, event_id, callback, context, false, false));
}


unsigned int eventemitter_on(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context)
{
  return(_eventemitter_add_listener(event_emitter, event_id, callback, context, false, false));
}


unsigned int eventemitter_prepend_listener(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context)
{
  return(_eventemitter_add_listener(event_emitter, event_id, callback, context, false, true));
}


unsigned int eventemitter_add_once_listener(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context)
{
  return(_eventemitter_add_listener(event_emitter, event_id, callback, context, true, false));
}


unsigned int eventemitter_once(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context)
{
  return(_eventemitter_add_listener(event_emitter, event_id, callback, context, true, false));
}


unsigned int eventemitter_prepend_once_listener(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context)
{
  return(_eventemitter_add_listener(event_emitter, event_id, callback, context, true, true));
}


unsigned int eventemitter_add_unhandled_listener(struct EventEmitter *event_emitter, void (*callback)(int event_id, void *event_data, void *context), void *context)
{
  return(_eventemitter_add_unhandled_listener(event_emitter, callback, context, false));
}


unsigned int eventemitter_else(struct EventEmitter *event_emitter, void (*callback)(int event_id, void *event_data, void *context), void *context)
{
  return(_eventemitter_add_unhandled_listener(event_emitter, callback, context, false));
}


unsigned int eventemitter_prepend_unhandled_listener(struct EventEmitter *event_emitter, void (*callback)(int event_id, void *event_data, void *context), void *context)
{
  return(_eventemitter_add_unhandled_listener(event_emitter, callback, context, true));
}


int eventemitter_remove_listener(struct EventEmitter *event_emitter, int event_id, unsigned int callback_id)
{
  if (event_emitter == NULL || !callback_id)
  {
    return(-1);
  }

  struct EventEmitterEventListeners *listeners = _eventemitter_get_listeners_for_event_id(event_emitter, event_id);
  if (listeners == NULL)
  {
    return(0);
  }

  size_t count  = vector_size(listeners->listeners);
  int    output = 0;
  for (size_t index = 0; index < count; index++)
  {
    struct EventEmitterEventListener *listener = (struct EventEmitterEventListener *)vector_get(listeners->listeners, index);

    if (listener != NULL && listener->id == callback_id)
    {
      free(listener);
      vector_remove(listeners->listeners, index);
      output = 1;
      break;
    }
  }

  if (vector_is_empty(listeners->listeners))
  {
    eventemitter_remove_all_event_listeners(event_emitter, event_id);
  }

  return(output);
}


int eventemitter_off(struct EventEmitter *event_emitter, int event_id, unsigned int callback_id)
{
  return(eventemitter_remove_listener(event_emitter, event_id, callback_id));
}


int eventemitter_remove_unhandled_listener(struct EventEmitter *event_emitter, unsigned int callback_id)
{
  if (event_emitter == NULL || !callback_id)
  {
    return(-1);
  }

  size_t count  = vector_size(event_emitter->unhandled_listeners);
  int    output = 0;
  for (size_t index = 0; index < count; index++)
  {
    struct EventEmitterUnhandledListener *listener = (struct EventEmitterUnhandledListener *)vector_get(event_emitter->unhandled_listeners, index);

    if (listener != NULL && listener->id == callback_id)
    {
      free(listener);
      vector_remove(event_emitter->unhandled_listeners, index);
      output = 1;
      break;
    }
  }

  return(output);
}


bool eventemitter_remove_all_event_listeners(struct EventEmitter *event_emitter, int event_id)
{
  if (event_emitter == NULL)
  {
    return(false);
  }

  struct EventEmitterEventListeners *listeners = _eventemitter_get_listeners_for_event_id(event_emitter, event_id);
  if (listeners == NULL)
  {
    return(true);
  }

  size_t count = vector_size(listeners->listeners);
  for (size_t index = 0; index < count; index++)
  {
    struct EventEmitterEventListener *listener = (struct EventEmitterEventListener *)vector_get(listeners->listeners, index);

    if (listener != NULL)
    {
      free(listener);
    }
  }

  count = vector_size(event_emitter->event_listeners);
  for (size_t index = 0; index < count; index++)
  {
    listeners = (struct EventEmitterEventListeners *)vector_get(event_emitter->event_listeners, index);

    if (listeners != NULL && listeners->event_id == event_id)
    {
      vector_remove(event_emitter->event_listeners, index);
      vector_release(listeners->listeners);
      free(listeners);
    }
  }

  return(true);
} /* eventemitter_remove_all_event_listeners */


bool eventemitter_remove_all_unhandled_listeners(struct EventEmitter *event_emitter)
{
  if (event_emitter == NULL)
  {
    return(false);
  }

  size_t count = vector_size(event_emitter->unhandled_listeners);
  for (size_t index = 0; index < count; index++)
  {
    struct EventEmitterUnhandledListener *listener = (struct EventEmitterUnhandledListener *)vector_get(event_emitter->unhandled_listeners, index);

    if (listener != NULL)
    {
      free(listener);
    }
  }
  vector_clear(event_emitter->unhandled_listeners);

  return(true);
}


bool eventemitter_remove_all_listeners(struct EventEmitter *event_emitter)
{
  if (event_emitter == NULL)
  {
    return(false);
  }

  while (!vector_is_empty(event_emitter->event_listeners))
  {
    struct EventEmitterEventListeners *listeners = (struct EventEmitterEventListeners *)vector_get(event_emitter->event_listeners, 0);

    if (listeners != NULL)
    {
      eventemitter_remove_all_event_listeners(event_emitter, listeners->event_id);
    }
    else
    {
      vector_remove(event_emitter->event_listeners, 0);
    }
  }

  eventemitter_remove_all_unhandled_listeners(event_emitter);

  return(true);
}


int eventemitter_listeners_count(struct EventEmitter *event_emitter, int event_id)
{
  if (event_emitter == NULL)
  {
    return(-1);
  }

  struct EventEmitterEventListeners *listeners = _eventemitter_get_listeners_for_event_id(event_emitter, event_id);
  if (listeners == NULL)
  {
    return(0);
  }

  return((int)vector_size(listeners->listeners));
}


int eventemitter_emit(struct EventEmitter *event_emitter, int event_id, void *event_data)
{
  if (event_emitter == NULL)
  {
    return(-1);
  }

  int                               callback_counter = 0;
  struct EventEmitterEventListeners *listeners       = _eventemitter_get_listeners_for_event_id(event_emitter, event_id);
  if (listeners != NULL)
  {
    size_t count = vector_size(listeners->listeners);
    for (size_t index = 0; index < count; index++)
    {
      struct EventEmitterEventListener *listener = (struct EventEmitterEventListener *)vector_get(listeners->listeners, index);

      if (listener != NULL)
      {
        listener->callback(event_data, listener->context);
        callback_counter++;
      }
    }

    // remove 'once' listeners
    for (size_t index = 0; index < count; index++)
    {
      size_t                           end_index = count - 1 - index;
      struct EventEmitterEventListener *listener = (struct EventEmitterEventListener *)vector_get(listeners->listeners, end_index);

      if (listener != NULL && listener->once)
      {
        free(listener);
        vector_remove(listeners->listeners, end_index);
      }
    }
    if (vector_is_empty(listeners->listeners))
    {
      eventemitter_remove_all_event_listeners(event_emitter, event_id);
    }
  }
  else
  {
    size_t count = vector_size(event_emitter->unhandled_listeners);
    for (size_t index = 0; index < count; index++)
    {
      struct EventEmitterUnhandledListener *listener = (struct EventEmitterUnhandledListener *)vector_get(event_emitter->unhandled_listeners, index);

      if (listener != NULL)
      {
        listener->callback(event_id, event_data, listener->context);
        callback_counter++;
      }
    }
  }

  return(callback_counter);
} /* eventemitter_emit */

static struct EventEmitterEventListeners *_eventemitter_get_listeners_for_event_id(struct EventEmitter *event_emitter, int event_id)
{
  if (event_emitter == NULL)
  {
    return(NULL);
  }

  size_t count = vector_size(event_emitter->event_listeners);
  for (size_t index = 0; index < count; index++)
  {
    struct EventEmitterEventListeners *listeners = (struct EventEmitterEventListeners *)vector_get(event_emitter->event_listeners, index);

    if (listeners != NULL && listeners->event_id == event_id)
    {
      return(listeners);
    }
  }

  return(NULL);
}


static unsigned int _eventemitter_add_listener(struct EventEmitter *event_emitter, int event_id, void (*callback)(void *event_data, void *context), void *context, bool once, bool prepend)
{
  if (event_emitter == NULL || callback == NULL)
  {
    return(0);
  }

  struct EventEmitterEventListeners *listeners = _eventemitter_get_listeners_for_event_id(event_emitter, event_id);
  if (listeners == NULL)
  {
    listeners            = malloc(sizeof(struct EventEmitterEventListeners));
    listeners->event_id  = event_id;
    listeners->listeners = vector_new();
    vector_push(event_emitter->event_listeners, listeners);
  }

  // allocate next id for listener
  unsigned int callback_id = event_emitter->next_callback_id;
  event_emitter->next_callback_id++;

  // create listener wrapper
  struct EventEmitterEventListener *listener = malloc(sizeof(struct EventEmitterEventListener));
  listener->id       = callback_id;
  listener->callback = callback;
  listener->context  = context;
  listener->once     = once;

  // keep in event listeners list
  if (prepend)
  {
    vector_prepend(listeners->listeners, listener);
  }
  else
  {
    vector_push(listeners->listeners, listener);
  }

  return(callback_id);
}


static unsigned int _eventemitter_add_unhandled_listener(struct EventEmitter *event_emitter, void (*callback)(int, void *, void *), void *context, bool prepend)
{
  if (event_emitter == NULL || callback == NULL)
  {
    return(0);
  }

  // allocate next id for listener
  unsigned int callback_id = event_emitter->next_callback_id;
  event_emitter->next_callback_id++;

  // create listener wrapper
  struct EventEmitterUnhandledListener *listener = malloc(sizeof(struct EventEmitterUnhandledListener));
  listener->id       = callback_id;
  listener->callback = callback;
  listener->context  = context;

  // keep in event listeners list
  if (prepend)
  {
    vector_prepend(event_emitter->unhandled_listeners, listener);
  }
  else
  {
    vector_push(event_emitter->unhandled_listeners, listener);
  }

  return(callback_id);
}

