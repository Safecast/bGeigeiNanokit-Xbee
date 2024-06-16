#ifndef BGEIGIECAST_SM_C_CONCRETE_STATES_H
#define BGEIGIECAST_SM_C_CONCRETE_STATES_H

#include "sm_c_state.h"

class InitializeState : public ControllerState {
 public:
  explicit InitializeState(Controller& context);
  virtual ~InitializeState() = default;

  int8_t get_state_id() const override { return k_state_InitializeState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;
};

class InitReadingState : public ControllerState {
 public:
  explicit InitReadingState(Controller& context);
  virtual ~InitReadingState() = default;

  int8_t get_state_id() const override { return k_state_InitReadingState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;

 private:
  bool _button_pressed;
};

class PostInitializeState : public ControllerState {
 public:
  explicit PostInitializeState(Controller& context);
  virtual ~PostInitializeState() = default;

  int8_t get_state_id() const override { return k_state_PostInitializeState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;

 private:
  uint32_t timer;
};

class ConfigurationModeState : public ControllerState {
 public:
  explicit ConfigurationModeState(Controller& context);
  virtual ~ConfigurationModeState() = default;

  int8_t get_state_id() const override { return k_state_ConfigurationModeState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;
};

class MobileModeState : public ControllerState {
 public:
  explicit MobileModeState(Controller& context);
  virtual ~MobileModeState() = default;

  int8_t get_state_id() const override { return k_state_MobileModeState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;

};

class FixedModeState : public ControllerState {
 public:
  explicit FixedModeState(Controller& context);
  virtual ~FixedModeState() = default;

  int8_t get_state_id() const override { return k_state_FixedModeState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;

};

class ResetState : public ControllerState {
 public:
  explicit ResetState(Controller& context);
  virtual ~ResetState() = default;

  int8_t get_state_id() const override { return k_state_ResetState; }

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;
};


extern InitializeState InitializeState_i;
extern InitReadingState InitReadingState_i;
extern PostInitializeState PostInitializeState_i;
extern ConfigurationModeState ConfigurationModeState_i;
extern MobileModeState MobileModeState_i;
extern FixedModeState FixedModeState_i;
extern ResetState ResetState_i;

#endif //BGEIGIECAST_SM_C_CONCRETE_STATES_H
