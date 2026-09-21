#!/usr/bin/env python3
"""製造前の最低限の検査。KiCadのERC/DRCの代替ではない。"""
from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parents[1]
board = root / "hardware/m5stamps3_phone_exchange.kicad_pcb"
text = board.read_text()
errors = []
depth = 0
for token in re.findall(r'"(?:\\.|[^"\\])*"|[()]', text):
    if token == "(":
        depth += 1
    elif token == ")":
        depth -= 1
    if depth < 0:
        errors.append("PCB構文の括弧が不整合")
        break
if depth != 0:
    errors.append("PCB構文の括弧が不整合")
if not re.search(r'\(net\s+[1-9]', text):
    errors.append("電気ネットが未定義")
if not re.search(r'\(segment\s', text):
    errors.append("配線が存在しない")
if not list((root / "hardware").rglob("*.kicad_sch")):
    errors.append("KiCad回路図が存在しない")
if "DO NOT FABRICATE" in text:
    errors.append("PCBに製造禁止表示がある")
for error in dict.fromkeys(errors):
    print("不合格: " + error)
print("この検査に合格しても、ERC/DRC・部品照合・回路レビューは別途必要です。")
sys.exit(1 if errors else 0)
