# eventemitter

[![CI](https://github.com/sagiegurari/c_eventemitter/workflows/CI/badge.svg?branch=master)](https://github.com/sagiegurari/c_eventemitter/actions)
[![Release](https://img.shields.io/github/v/release/sagiegurari/c_eventemitter)](https://github.com/sagiegurari/c_eventemitter/releases)
[![license](https://img.shields.io/github/license/sagiegurari/c_eventemitter)](https://github.com/sagiegurari/c_eventemitter/blob/master/LICENSE)

> Simple Event Emitter C API

* [Overview](#overview)
* [Usage](#usage)
* [Contributing](.github/CONTRIBUTING.md)
* [Release History](CHANGELOG.md)
* [License](#license)

<a name="overview"></a>
## Overview
This library enables to register event listeners and emit events.

<a name="usage"></a>
## Usage

```c
#include "eventemitter.h"
#include <stdio.h>

enum ExampleEvents
{
  EVENT_START  = 100,
  EVENT_MIDDLE = 200,
  EVENT_END    = 300
};


static void on_start(void *event_data, void *context)
{
  printf("OnStart, data: %s context: %s\n", (char *)event_data, (char *)context);
}


static void on_end(void *event_data, void *context)
{
  printf("OnEnd, data: %s context: %s\n", (char *)event_data, (char *)context);
}


static void on_unhandled(int event_id, void *event_data, void *context)
{
  printf("OnUnhandled, did not set listener for event: %d data: %s context: %s\n", event_id, (char *)event_data, (char *)context);
}


int main()
{
  // first create a new event emitter which will host the listeners and emit the events
  struct EventEmitter *event_emitter = eventemitter_new();

  // lets add few listeners.
  // we can add as many as we want
  // this listener will be triggered for the 'start' event
  // we are adding the same callback multiple times so for each event triggered
  // the callback will be called several times
  // The last param is a context that the specific listener will get each time its triggered
  // Each add function returns a unique ID which can be used to remove the listener
  unsigned int listener_id = eventemitter_add_listener(event_emitter, EVENT_START, on_start, "some context for start 1");

  eventemitter_add_listener(event_emitter, EVENT_START, on_start, "some context for start 2");
  eventemitter_on(event_emitter, EVENT_START, on_start, "some context for shorthand start"); // shorthand version

  // we can add listeners that are triggered once and are removed afterwards
  eventemitter_add_once_listener(event_emitter, EVENT_END, on_end, "end context 1");
  eventemitter_once(event_emitter, EVENT_END, on_end, "end context 2"); // shorthand

  // we can add listeners to any event triggered but no listeners were registered to it
  eventemitter_add_unhandled_listener(event_emitter, on_unhandled, "unhandled context 1");
  eventemitter_else(event_emitter, on_unhandled, "unhandled context 2"); // shorthand

  // trigger the 'start' event and pass some optional event data (any type) which
  // listeners will get in the callback
  eventemitter_emit(event_emitter, EVENT_START, "my start event data");

  // lets remove one of the 'start' listeners and emit it again
  eventemitter_remove_listener(event_emitter, EVENT_START, listener_id);
  eventemitter_emit(event_emitter, EVENT_START, "my second start event data");

  // lets trigger an event that no one listens to and see how the unhandled listeners are triggered
  eventemitter_emit(event_emitter, EVENT_MIDDLE, "middle event data");

  // lets trigger the 'end' event
  eventemitter_emit(event_emitter, EVENT_END, "end event data");
  // since all 'end' event listeners were added as 'once' they are no longer registered
  // so we will trigger it again and see how the unhandled listeners are invoked
  eventemitter_emit(event_emitter, EVENT_END, "end event data again");
  eventemitter_emit(event_emitter, EVENT_END, "end event data again 2");

  // once done with the emitter, we should release it
  // no need to release the listeners manually
  printf("Releasing Event Emitter\n");
  eventemitter_release(event_emitter);

  return(0);
} /* main */
```

## Contributing
See [contributing guide](.github/CONTRIBUTING.md)

<a name="history"></a>
## Release History

See [Changelog](CHANGELOG.md)

<a name="license"></a>
## License
Developed by Sagie Gur-Ari and licensed under the Apache 2 open source license.
