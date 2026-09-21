# Architecture

## Power domains

1. `VIN`: isolated 12-24 VDC input.
2. `+5V`: buck-regulated supply for StampS3.
3. `LINE_24V`: current-limited telephone loop supply.
4. `RING_HV`: switched high-voltage ring supply, disabled by default.

Logic ground and telephone-line ground are initially common for a low-cost
prototype. The ring driver control must be galvanically or optically isolated
if its topology is not ground referenced.

## Functional blocks

```text
DC input -> protection -> 5 V buck -> StampS3
       |-> 24 V loop feed -> Port A/B hook sensing
       |-> ring boost/H-bridge -> Port A/B ring selection

Port A audio <-> protected AC coupling/speech path <-> Port B audio
```

## Design decisions pending validation

- Required ring voltage for the target Japanese mechanical telephone
- Boost topology and transformer/inductor selection
- Loop current target and feed resistor dissipation
- Speech-path insertion loss and sidetone behavior
- Relay versus solid-state line switching
- DTMF and rotary pulse requirements

## Firmware milestones

1. Validate hook inputs with switches and LEDs.
2. Validate call-state machine without high voltage.
3. Add guarded ring-enable interlock and watchdog fail-safe.
4. Add Wi-Fi captive configuration page.
5. Add rotary dial pulse and optional DTMF decoding.

