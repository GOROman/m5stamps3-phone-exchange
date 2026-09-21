#pragma once
#include <cstdint>

// Ag1171呼出の参照モデル。GPIOドライバ・独立安全回路ではない。
// requestは呼出期間中true、tripは外部で10msデバウンス済みのSHK。
namespace revb {
struct RingPins { bool rm = false; bool fr = true; };
class RingSequencer {
 public:
  RingPins update(uint32_t now, bool request, bool trip) {
    if (!request) tripped_ = false;
    if (trip) tripped_ = true;
    switch (state_) {
      case State::Idle:
        if (request && !tripped_ &&
            (!started_ || uint32_t(now - start_) >= 4000)) {
          started_ = true;
          start_ = now;
          state_ = State::Ringing;
          pins_ = {true, true};
        }
        break;
      case State::Ringing:
        if (!request || tripped_ || uint32_t(now - start_) >= 1000) {
          // 実際にFRをHighへ変更した時刻を保存。開始時刻から逆算しない。
          pins_.fr = true;
          stop_ = now;
          state_ = State::Settling;
        } else {
          pins_.fr = (uint32_t(now - start_) / 25) % 2 == 0;
        }
        break;
      case State::Settling:
        if (uint32_t(now - stop_) >= 20) {
          pins_ = {false, true};
          state_ = State::Idle;
        }
        break;
    }
    return pins_;
  }
  bool stopped() const { return state_ == State::Idle && !pins_.rm; }
 private:
  enum class State { Idle, Ringing, Settling };
  State state_ = State::Idle;
  RingPins pins_{};
  uint32_t start_ = 0, stop_ = 0;
  bool started_ = false, tripped_ = false;
};
}  // namespace revb
