# M5StampS3 Phone Exchange

Two-line standalone analog telephone exchange based on the M5Stack StampS3.

The project is a modern reinterpretation of the Akizuki PIC simple telephone
exchange kit. It is intended only for isolated, private telephone instruments.
Never connect it to a public switched telephone network (PSTN).

## Initial target

- Two RJ11 telephone ports
- 12-24 VDC input
- Loop-current/off-hook detection per port
- Ring one telephone from the other
- Connect bidirectional voice after answer
- Detect hang-up and return to idle
- Wi-Fi configuration and OTA-ready firmware
- JLCPCB-compatible two-layer carrier PCB for M5StampS3

## Repository layout

- `firmware/`: PlatformIO/Arduino firmware
- `hardware/`: schematic, PCB and manufacturing notes
- `docs/`: architecture and safety requirements

## Status

The GPIO abstraction and call-state machine are implemented as a hardware-test
scaffold. Rev.Aのキャリア基板レイアウトを追加済みです。高電圧リンガ回路と
電話回線インターフェースの具体的な定数・部品は、現在設計中です。

## Build

```sh
cd firmware
pio run
```

## Safety

The ring generator may exceed SELV voltage. Use current limiting, creepage,
clearance, guarded test points and a covered enclosure. Do not probe the ring
section while energized.

## Reference

- [M5Stack StampS3 documentation](https://docs.m5stack.com/en/core/StampS3)
- [Akizuki PIC simple telephone exchange kit](https://akizukidenshi.com/img/contents/kairo/%E3%83%87%E3%83%BC%E3%82%BF/%E3%81%9D%E3%81%AE%E4%BB%96/L013_%E7%96%91%E4%BC%BC%E9%9B%BB%E8%A9%B1%E4%BA%A4%E6%8F%9B%E6%A9%9F.pdf)
