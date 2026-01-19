# 3x3_Keyboard
# Macropad 3x3 (QMK + VIA)

A custom 3×3 mechanical macropad based on the Arduino Pro Micro (ATmega32U4),
running QMK firmware with VIA support for live macro configuration.

## Features
- 3×3 key matrix (9 keys)
- QMK firmware
- VIA / WebHID configuration
- Per-key macros
- No background software required

## Hardware
- MCU: Arduino Pro Micro (ATmega32U4)
- Switches: MX-compatible
- Diodes: 1N4148
- Matrix: COL2ROW

## Pin Mapping

| Function | Pins |
|--------|------|
| Columns | 2, 3, 4 |
| Rows    | 5, 6, 7 |

## Firmware
Firmware source is located in `firmware/qmk`.

### Build
```bash
qmk compile -kb neil/macropad3x3 -km via
