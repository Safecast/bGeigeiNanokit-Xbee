#include "debugger.h"
#include "sm_context.h"

Context::Context() : _current_state(nullptr), _event_queue() {
}

Context::~Context() {
  if(_current_state) {
    _current_state->exit_action();
    delete _current_state;
  }
}

void Context::set_state(BaseState* state) {
  if(_current_state) {
    _current_state->exit_action();
    delete _current_state;
  }
  _current_state = state;
  if(_current_state) {
    _current_state->entry_action();
  }
}

void Context::run() {
  if(!_current_state) {
    DEBUG_PRINTLN("Trying to run state machine with no active state");
    return;
  }
  _current_state->do_activity();
  handle_events();
}

void Context::schedule_event(Event_enum event_id) {
  _event_queue.add(event_id);
}

BaseState* Context::get_current_state() const {
  return _current_state;
}

void Context::clear_events() {
  _event_queue.clear();
}

void Context::handle_events() {
  while(!_event_queue.empty()) {
    Event_enum event_id = _event_queue.get();
    if(_current_state) { _current_state->handle_event(event_id); }
  }
}
