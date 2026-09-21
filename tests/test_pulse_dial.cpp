#include "../firmware/reference/pulse_dial.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <vector>

struct Fixture {
  revb::PulseDial decoder;
  uint32_t now = 0;
  std::vector<int> digits;
  unsigned seized = 0, hung = 0, invalid = 0;
  void feed(bool high, unsigned ms) {
    for (unsigned i = 0; i < ms; ++i, ++now) {
      auto e = decoder.update(now, high);
      if (e.digit >= 0) digits.push_back(e.digit);
      seized += e.seized;
      hung += e.hung_up;
      invalid += e.invalid;
    }
  }
  void pulse(unsigned count, unsigned low, unsigned high) {
    for (unsigned i = 0; i < count; ++i) {
      feed(false, low);
      feed(true, high);
    }
    feed(true, 300);
  }
};
int main() {
  // 10ppsと20pps、全数字。ブレーク率60%。
  for (auto low : {60U, 30U}) {
    for (unsigned n = 1; n <= 10; ++n) {
      Fixture f;
      f.feed(false, 100);
      f.feed(true, 300);
      f.pulse(n, low, low * 2 / 3);
      assert(f.digits == std::vector<int>{n == 10 ? 0 : int(n)});
      assert(f.seized == 1 && f.hung == 0 && f.invalid == 0);
    }
  }
  { // 接点バウンスは桁にしない。
    Fixture f;
    f.feed(true, 300);
    f.feed(false, 5);
    f.feed(true, 5);
    f.feed(false, 60);
    f.feed(true, 5);
    f.feed(false, 5);
    f.feed(true, 350);
    assert(f.digits == std::vector<int>{1});
  }
  { // 11パルスは不正。次の桁で回復する。
    Fixture f;
    f.feed(true, 300);
    f.pulse(11, 60, 40);
    assert(f.digits.empty() && f.invalid == 1);
    f.pulse(2, 30, 20);
    assert(f.digits == std::vector<int>{2});
  }
  { // 桁の途中で切断しても数字を通知しない。
    Fixture f;
    f.feed(true, 300);
    f.feed(false, 60);
    f.feed(true, 40);
    f.feed(false, 600);
    assert(f.digits.empty() && f.hung == 1);
    f.feed(false, 1000);
    assert(f.hung == 1);
    f.feed(true, 300);
    assert(f.seized == 2 && f.digits.empty());
  }
  { // パルスとして長すぎるフラッシュを拒否。
    Fixture f;
    f.feed(true, 300);
    f.feed(false, 150);
    f.feed(true, 300);
    assert(f.invalid == 1 && f.digits.empty() && f.hung == 0);
  }
  { // ミリ秒カウンタの32bit周回。
    Fixture f;
    f.now = UINT32_MAX - 350;
    f.feed(true, 300);
    f.pulse(3, 30, 20);
    assert(f.digits == std::vector<int>{3});
  }
  std::puts("パルス認識: 10/20pps全数字・バウンス・不正桁・切断・時刻周回 合格");
}
