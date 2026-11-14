# ESP32 pin rules (classic ESP32 / ESP32-WROOM-32)

AI tools assign pins as if every GPIO were the same. They aren't. Check every pin in generated code against this list. (ESP32-S3, -C3 and -C6 have different pin maps; check your board's datasheet.)

| Pins | Rule | What goes wrong |
|---|---|---|
| **GPIO 6–11** | Never use | Wired to the internal SPI flash. Using them crashes the chip. |
| **GPIO 34, 35, 36, 39** | Input only | No output drivers and no internal pull-ups. Fine for analog or digital input with an external pull-up. |
| **GPIO 0, 2, 12, 15** | Strapping pins: careful at boot | Sampled at reset to choose the boot mode; the wrong level from an attached circuit can stop the board booting or flashing. |
| **ADC2: GPIO 0, 2, 4, 12–15, 25–27** | No analog while Wi-Fi is on | The Wi-Fi driver takes the ADC2 controller; `analogRead()` fails. Use ADC1. |
| **ADC1: GPIO 32–39** | Use for analog | Works with Wi-Fi on. |
| **I2C** | Needs pull-ups | External pull-ups (typically 4.7 kΩ) on SDA and SCL unless your module has them. |

## Electrical rules

- **3.3 V logic.** Feeding a 5 V signal straight into a GPIO is the most common way people kill the chip. Level-shift first.
- **A few milliamps per pin.** Drive motors, relays and heaters through a transistor or driver module, never directly.
- **Safe state at boot.** Outputs float or go LOW during reset; wire loads so that "off" is the default, and set output pins first thing in `setup()`. See [examples/FailSafeRelay](../examples/FailSafeRelay/FailSafeRelay.ino).
