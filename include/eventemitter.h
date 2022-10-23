#ifndef EVENTEMITTER_H
#define EVENTEMITTER_H

#include <stdbool.h>

struct EventEmitter;

/**
 * Creates and returns a new event emitter.
 * Once no longer needed, it must be released.
 */
struct EventEmitter *eventemitter_new(void);

/**
 * Frees the memory of the provided emitter.
 */
void eventemitter_release(struct EventEmitter *);

/**
 * Adds a new event listener for the given event ID.
 * The provided optional context will be provided to the callback when the event is triggered.
 * The returned ID can be later used to remove the listener.
 * In case of invalid input, 0 will be returned.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_add_listener(struct EventEmitter *, int /* event ID */, void (*callback)(void * /* event data */, void * /* context */), void * /* context */);

/**
 * Shorthand for add_listener.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_on(struct EventEmitter *, int /* event ID */, void (*callback)(void * /* event data */, void * /* context */), void * /* context */);

/**
 * Same as the add listener, but it adds it to the start of the listener list.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_prepend_listener(struct EventEmitter *, int /* event ID */, void (*callback)(void * /* event data */, void * /* context */), void * /* context */);

/**
 * Same as the add listener, but once an event is handled by the callback,
 * the callback would automatically be removed.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_add_once_listener(struct EventEmitter *, int /* event ID */, void (*callback)(void * /* event data */, void * /* context */), void * /* context */);

/**
 * Shorthand for add once listener.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_once(struct EventEmitter *, int /* event ID */, void (*callback)(void * /* event data */, void * /* context */), void * /* context */);

/**
 * Same as the add once listener, but it adds it to the start of the listener list.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_prepend_once_listener(struct EventEmitter *, int /* event ID */, void (*callback)(void * /* event data */, void * /* context */), void * /* context */);

/**
 * Adds a new event listener which will be invoked for any event ID without registered listeners.
 * The provided optional context will be provided to the callback when the event is triggered.
 * The returned ID can be later used to remove the listener.
 * In case of invalid input, 0 will be returned.
 *
 * @param event emitter - The emitter struct
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_add_unhandled_listener(struct EventEmitter *, void (*callback)(int /* event ID */, void * /* event data */, void * /* context */), void * /* context */);

/**
 * Shorthand for add unhandled listener.
 *
 * @param event emitter - The emitter struct
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_else(struct EventEmitter *, void (*callback)(int /* event ID */, void * /* event data */, void * /* context */), void * /* context */);

/**
 * Same as the add unhandled listener, but it adds it to the start of the listener list.
 *
 * @param event emitter - The emitter struct
 * @param callback - Will be called when the event ID is triggered via emit
 * @param context - Will be passed to this specific callback when an event is triggered
 * @returns 0 in case of error or the callback ID which can be used to remove the listener
 */
unsigned int eventemitter_prepend_unhandled_listener(struct EventEmitter *, void (*callback)(int /* event ID */, void * /* event data */, void * /* context */), void * /* context */);

/**
 * Removes the listener for the given event ID and callback ID if exists.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback ID - The callback ID returned from the add listener function
 * @returns -1 for invalid input, 0 for callback not found, 1 for removed
 */
int eventemitter_remove_listener(struct EventEmitter *, int /* event ID */, unsigned int /* callback ID */);

/**
 * Shorthand for remove listener.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @param callback ID - The callback ID returned from the add listener function
 * @returns -1 for invalid input, 0 for callback not found, 1 for removed
 */
int eventemitter_off(struct EventEmitter *, int /* event ID */, unsigned int /* callback ID */);

/**
 * Removes the unhandled events listener for the given callback ID if exists.
 *
 * @param event emitter - The emitter struct
 * @param callback ID - The callback ID returned from the add listener function
 * @returns -1 for invalid input, 0 for callback not found, 1 for removed
 */
int eventemitter_remove_unhandled_listener(struct EventEmitter *, unsigned int /* callback ID */);

/**
 * Removes all listeners for the given event ID.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @returns true in case of valid input
 */
bool eventemitter_remove_all_event_listeners(struct EventEmitter *, int /* event ID */);

/**
 * Removes all unhandled event listeners.
 *
 * @param event emitter - The emitter struct
 * @returns true in case of valid input
 */
bool eventemitter_remove_all_unhandled_listeners(struct EventEmitter *);

/**
 * Removes all listeners for all events.
 *
 * @param event emitter - The emitter struct
 * @returns true in case of valid input
 */
bool eventemitter_remove_all_listeners(struct EventEmitter *);

/**
 * Returns the listeners count for the given event ID.
 * In case of invalid input, -1 will be returned.
 *
 * @param event emitter - The emitter struct
 * @param event ID - The event ID that listeners have registered on
 * @returns the amount of callbacks registered for the given event ID or returns -1 in case of invalid input
 */
int eventemitter_listeners_count(struct EventEmitter *, int /* event ID */);

/**
 * Triggers an event for the given event ID.
 * The event data would be given to all relevant event listeners.
 *
 * @param event emitter - The emitter struct
 * @param event data - The event data passed to all relevant listeners
 * @returns the amount of callbacks invoked (including unhandled) or returns -1 in case of invalid input
 */
int eventemitter_emit(struct EventEmitter *, int /* event ID */, void * /* event data */);

#endif

