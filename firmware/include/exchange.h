#pragma once

#include <Arduino.h>

enum class ExchangeState {
  Idle,
  SeizeA,
  SeizeB,
  RingA,
  RingB,
  Connected,
  ReleaseGuard,
  Fault,
};

class Exchange {
 public:
  void begin();
  void update(uint32_t nowMs);
  ExchangeState state() const { return state_; }

 private:
  bool offHookA() const;
  bool offHookB() const;
  void transition(ExchangeState next, uint32_t nowMs);
  void applyOutputs(uint32_t nowMs);

  ExchangeState state_{ExchangeState::Idle};
  uint32_t enteredAtMs_{0};
};

