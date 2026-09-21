#pragma once
#include <cstdint>

namespace revb {
// SHK High=オフフック。1ms程度で呼ぶ純粋ロジック。GPIOや呼状態は操作しない。
// 設計上の受付窓であり、電話機規格への適合・全機種対応の保証ではない。
struct DialEvent {
  int digit = -1;
  bool seized = false;
  bool hung_up = false;
  bool invalid = false;
};
class PulseDial {
 public:
  DialEvent update(uint32_t now, bool raw_off_hook) {
    DialEvent event;
    if (!initialized_) {
      initialized_ = true;
      candidate_ = raw_off_hook;
      candidate_at_ = now;
    }
    if (candidate_ != raw_off_hook) {
      candidate_ = raw_off_hook;
      candidate_at_ = now;
    }
    if (stable_ != candidate_ && uint32_t(now - candidate_at_) >= 10) {
      stable_ = candidate_;
      if (stable_) {
        if (!active_) {
          active_ = true;
          event.seized = true;
        } else {
          const auto width = uint32_t(now - edge_at_);
          if (width >= 20 && width <= 100 && count_ < 10 && !bad_) {
            ++count_;
          } else {
            bad_ = true;
            count_ = 0;
            event.invalid = true;
          }
        }
      }
      edge_at_ = now;
    }
    if (active_ && !stable_ && uint32_t(now - edge_at_) >= 500) {
      active_ = false;
      count_ = 0;
      bad_ = false;
      event.hung_up = true;
    }
    if (active_ && stable_ && uint32_t(now - edge_at_) >= 250) {
      if (count_ && !bad_) event.digit = count_ == 10 ? 0 : count_;
      count_ = 0;
      bad_ = false;
    }
    return event;
  }
 private:
  uint32_t candidate_at_ = 0, edge_at_ = 0;
  unsigned count_ = 0;
  bool initialized_ = false, candidate_ = false, stable_ = false;
  bool active_ = false, bad_ = false;
};
}  // namespace revb
