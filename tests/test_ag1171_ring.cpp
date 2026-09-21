#include "../firmware/reference/ag1171_ring.h"
#include <cassert>
#include <cstdio>

int main() {
  using revb::RingSequencer;
  {
    RingSequencer r;
    auto p = r.update(0, false, false);
    assert(!p.rm && p.fr && r.stopped());
    p = r.update(1, true, false);
    assert(p.rm && p.fr);
    assert(!r.update(26, true, false).fr);
    assert(r.update(51, true, false).fr);
    p = r.update(1001, true, false);
    assert(p.rm && p.fr);
    assert(r.update(1020, true, false).rm);
    assert(!r.update(1021, true, false).rm);
    assert(!r.update(4000, true, false).rm);
    assert(r.update(4001, true, false).rm);
  }
  {
    RingSequencer r;
    r.update(0, true, false);
    assert(!r.update(25, true, false).fr);
    auto p = r.update(30, true, true);
    assert(p.rm && p.fr);
    assert(r.update(49, true, true).rm);
    assert(!r.update(50, true, true).rm);
    // 一瞬のtripでも要求を解除するまで再呼出しない。
    assert(!r.update(5000, true, false).rm);
    r.update(5001, false, false);
    assert(r.update(5002, true, false).rm);
  }
  {
    RingSequencer r;
    r.update(0, true, false);
    auto p = r.update(25, false, false);
    assert(p.rm && p.fr);  // キャンセルも停止順序を守る。
    assert(!r.update(45, false, false).rm);
    assert(!r.update(46, true, false).rm);  // 高速再要求による占有率超過防止。
  }
  {
    RingSequencer r;
    const uint32_t t = UINT32_MAX - 9;
    r.update(t, true, false);
    assert(!r.update(t + 25, true, false).fr);
    auto p = r.update(t + 30, true, true);
    assert(p.rm && p.fr);
    assert(!r.update(t + 50, true, true).rm);
  }
  {
    RingSequencer r;
    r.update(0, true, false);
    // 遅延復帰でも、FRをHighにした実時刻から20ms待つ。
    assert(r.update(1500, true, false).rm);
    assert(r.update(1519, true, false).rm);
    assert(!r.update(1520, true, false).rm);
    // ここでは過長鳴動そのものは防げない。ハードウェア停止が必要。
  }
  {
    RingSequencer r;
    unsigned high = 0;
    for (uint32_t t = 0; t < 40000; ++t) {
      if (r.update(t, true, false).rm) ++high;
    }
    assert(high == 10200);  // 1ms周期で25.5%。
  }
  std::puts("6項目合格: 初期値・周期・応答停止・キャンセル・時刻周回・占有率等");
}
