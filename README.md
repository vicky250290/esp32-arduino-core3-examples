# ESP32 Arduino Core 3.x Examples

[![compile](../../actions/workflows/compile.yml/badge.svg)](../../actions/workflows/compile.yml)

Small, correct sketches for **Arduino-ESP32 core 3.x**, plus the pin rules and prompts that stop ChatGPT, Claude, Copilot and Cursor from handing you firmware that won't compile or that crashes the board.

## Why this exists

Arduino-ESP32 went from 2.x to 3.x (on ESP-IDF 5.x) and changed the PWM and timer APIs. Most AI tools learned from years of 2.x examples, so they keep writing:

```cpp
ledcSetup(0, 5000, 8);      // removed in 3.x
ledcAttachPin(LED_PIN, 0);  // removed in 3.x
```

instead of:

```cpp
ledcAttach(LED_PIN, 5000, 8);
ledcWrite(LED_PIN, duty);
```

They also pick pins that crash the chip (GPIO 6–11), read analog pins that stop working when Wi-Fi turns on (ADC2), and put long work inside interrupts. Every sketch here avoids those, and **CI compiles all of them against the latest Arduino-ESP32 3.x on every push**.

## Examples

| Sketch | Shows |
|---|---|
| [PwmFade](examples/PwmFade/PwmFade.ino) | 3.x LEDC: `ledcAttach` + `ledcWrite(pin, duty)` |
| [HardwareTimer](examples/HardwareTimer/HardwareTimer.ino) | 3.x timers: `timerBegin(freq)`, `timerAlarm`, an ISR that only signals |
| [AnalogWithWiFi](examples/AnalogWithWiFi/AnalogWithWiFi.ino) | Analog input on ADC1 while Wi-Fi is on, calibrated millivolts |
| [ButtonInterruptTask](examples/ButtonInterruptTask/ButtonInterruptTask.ino) | Tiny ISR → semaphore → FreeRTOS task; heap buffer instead of stack |
| [FailSafeRelay](examples/FailSafeRelay/FailSafeRelay.ino) | Heater relay that fails closed: off at boot, off on sensor fault, hard temperature limit, loop watchdog |
| [DeepSleepTimer](examples/DeepSleepTimer/DeepSleepTimer.ino) | Deep sleep with a configured wake source and a counter that survives sleep |

Written for a classic ESP32 DevKit (ESP32-WROOM-32). On S3, C3 or C6 boards, change the pin numbers to suit your board.

## Reference

- [docs/migration-2x-to-3x.md](docs/migration-2x-to-3x.md): old call → new call, for LEDC and timers
- [docs/pin-rules.md](docs/pin-rules.md): which pins to never use, input-only pins, strapping pins, ADC2 and Wi-Fi
- [prompts/](prompts/README.md): a project rules file for AGENTS.md / CLAUDE.md, plus prompts that make the AI check versions, pins and FreeRTOS hazards itself

## Build

Arduino IDE: install **esp32 by Espressif Systems** version 3.x from the Boards Manager, open a sketch, select your board.

arduino-cli:

```bash
arduino-cli core install esp32:esp32 \
  --additional-urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli compile --fqbn esp32:esp32:esp32 examples/PwmFade
```

For `AnalogWithWiFi`, put your Wi-Fi details in `arduino_secrets.h` locally and don't commit them.

## Safety

GPIOs are 3.3 V and source only a few milliamps: level-shift 5 V signals and drive loads through a transistor or driver module. **If a project touches mains voltage, use properly rated, isolated modules, and if you aren't qualified, don't.** No code makes unsafe wiring safe.

## Learn more

These sketches and prompts come from **[Vibe-Coding Your ESP32: AI-Assisted Firmware Without the Magic Smoke](https://www.techbrosebooks.co.in/books/vibe-coding-your-esp32-ai-assisted-firmware-without-the-magic-smoke?utm_source=github&utm_medium=referral&utm_campaign=esp32-arduino-core3-examples)**, which covers giving the AI a compile-and-fix feedback loop, simulating in Wokwi before flashing, memory and power pitfalls, OTA rollback, and a 12-point pre-flash review checklist.

Free article: [Why AI Writes Broken ESP32 Code (and How to Fix It for Arduino-ESP32 3.x)](https://www.techbrosebooks.co.in/blog/esp32-ai-code-arduino-core-3?utm_source=github&utm_medium=referral&utm_campaign=esp32-arduino-core3-examples)

## License

[MIT](LICENSE)
