#include "exchange.h"

#include "board_pins.h"

namespace {
constexpr uint32_t kSeizeDelayMs = 300;
constexpr uint32_t kReleaseGuardMs = 500;
constexpr uint32_t kRingCadenceOnMs = 1000;
constexpr uint32_t kRingCadencePeriodMs = 3000;
}  // namespace

void Exchange::begin() {
  pinMode(board::kOffHookA, INPUT_PULLUP);
  pinMode(board::kOffHookB, INPUT_PULLUP);
  pinMode(board::kLineRelayA, OUTPUT);
  pinMode(board::kLineRelayB, OUTPUT);
  pinMode(board::kRingEnable, OUTPUT);
  pinMode(board::kRingPolarity, OUTPUT);
  transition(ExchangeState::Idle, millis());
}

bool Exchange::offHookA() const { return digitalRead(board::kOffHookA) == LOW; }
bool Exchange::offHookB() const { return digitalRead(board::kOffHookB) == LOW; }

void Exchange::transition(ExchangeState next, uint32_t nowMs) {
  state_ = next;
  enteredAtMs_ = nowMs;
  Serial.printf("state=%u\n", static_cast<unsigned>(state_));
}

void Exchange::update(uint32_t nowMs) {
  const bool a = offHookA();
  const bool b = offHookB();
  const uint32_t elapsed = nowMs - enteredAtMs_;

  switch (state_) {
    case ExchangeState::Idle:
      if (a && !b) transition(ExchangeState::SeizeA, nowMs);
      else if (b && !a) transition(ExchangeState::SeizeB, nowMs);
      break;
    case ExchangeState::SeizeA:
      if (!a) transition(ExchangeState::ReleaseGuard, nowMs);
      else if (elapsed >= kSeizeDelayMs) transition(ExchangeState::RingB, nowMs);
      break;
    case ExchangeState::SeizeB:
      if (!b) transition(ExchangeState::ReleaseGuard, nowMs);
      else if (elapsed >= kSeizeDelayMs) transition(ExchangeState::RingA, nowMs);
      break;
    case ExchangeState::RingA:
    case ExchangeState::RingB:
      if (!a && !b) transition(ExchangeState::ReleaseGuard, nowMs);
      else if (a && b) transition(ExchangeState::Connected, nowMs);
      break;
    case ExchangeState::Connected:
      if (!a || !b) transition(ExchangeState::ReleaseGuard, nowMs);
      break;
    case ExchangeState::ReleaseGuard:
      if (elapsed >= kReleaseGuardMs && !a && !b)
        transition(ExchangeState::Idle, nowMs);
      break;
    case ExchangeState::Fault:
      break;
  }

  applyOutputs(nowMs);
}

void Exchange::applyOutputs(uint32_t nowMs) {
  const bool connected = state_ == ExchangeState::Connected;
  const bool ringing = state_ == ExchangeState::RingA ||
                       state_ == ExchangeState::RingB;
  const uint32_t phase = (nowMs - enteredAtMs_) % kRingCadencePeriodMs;
  const bool cadenceOn = ringing && phase < kRingCadenceOnMs;

  digitalWrite(board::kLineRelayA, connected ? HIGH : LOW);
  digitalWrite(board::kLineRelayB, connected ? HIGH : LOW);
  digitalWrite(board::kRingEnable, cadenceOn ? HIGH : LOW);

  // Slow polarity reversal placeholder. The final hardware driver will create
  // a current-limited 16-20 Hz ringing waveform.
  digitalWrite(board::kRingPolarity, ((nowMs / 25U) & 1U) ? HIGH : LOW);
}

