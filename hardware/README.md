# Hardware

Target CAD: EasyEDA Pro. Target fabrication: JLCPCB, two-layer FR-4.

The first board is a StampS3 carrier. It supports removable 2.54 mm headers
and duplicate SMT pads so the module can later be soldered directly.

## Planned sheets

1. Power input, protection and 5 V buck
2. StampS3 carrier and programming access
3. Port A line feed and hook detector
4. Port B line feed and hook detector
5. Speech path and line switching
6. Ring boost, H-bridge and port selection

## Rev.A PCB

`m5stamps3_phone_exchange.kicad_pcb` contains the initial 80 mm × 60 mm
carrier layout with the StampS3 area, DC input, two RJ11 connectors and an
expansion header for the ring/audio section. See `REV_A.md` for the current
manufacturing assumptions and validation items.

## Manufacturing outputs

- Gerber/NC drill ZIP
- BOM CSV with LCSC part numbers
- CPL CSV
- Assembly drawing
- Schematic PDF

No public telephone network connector is provided. RJ11 ports are labeled
`PRIVATE PHONE A` and `PRIVATE PHONE B`.
