# Arduino-ESP32 2.x → 3.x: the calls AI tools still get wrong

Core 3.x (built on ESP-IDF 5.x) changed several peripheral APIs. Models trained mostly on 2.x examples keep generating the old calls, which no longer compile. If you see any left-column call in AI output, it's 2.x code.

## LEDC (PWM)

| 2.x (removed) | 3.x |
|---|---|
| `ledcSetup(channel, freq, bits);` + `ledcAttachPin(pin, channel);` | `ledcAttach(pin, freq, bits);` (channel picked automatically) |
| — | `ledcAttachChannel(pin, freq, bits, channel);` (pick the channel yourself) |
| `ledcWrite(channel, duty);` | `ledcWrite(pin, duty);` |
| `ledcWriteTone(channel, freq);` | `ledcWriteTone(pin, freq);` |
| `ledcRead(channel);` | `ledcRead(pin);` |
| `ledcDetachPin(pin);` | `ledcDetach(pin);` |

Example: [examples/PwmFade](../examples/PwmFade/PwmFade.ino)

## Hardware timers

| 2.x (removed) | 3.x |
|---|---|
| `timerBegin(num, divider, countUp);` | `timerBegin(frequencyHz);` e.g. `timerBegin(1000000)` for 1 µs ticks |
| `timerAttachInterrupt(timer, isr, edge);` | `timerAttachInterrupt(timer, isr);` |
| `timerAlarmWrite(timer, ticks, autoreload);` + `timerAlarmEnable(timer);` | `timerAlarm(timer, ticks, autoreload, reloadCount);` (`0` = forever) |

Example: [examples/HardwareTimer](../examples/HardwareTimer/HardwareTimer.ino)

## Other changes to watch for

- **ESP-IDF I2C driver (if you call IDF directly):** the legacy `i2c_config_t` / `i2c_driver_install` / `i2c_master_write_to_device` driver is replaced by the bus/device model: `i2c_new_master_bus`, `i2c_master_bus_add_device`, `i2c_master_transmit`. Arduino's `Wire` library still works as before.
- **Task watchdog (ESP-IDF 5):** `esp_task_wdt_init()` now takes a `esp_task_wdt_config_t*` instead of `(timeout, panic)`. In Arduino, `enableLoopWDT()` / `feedLoopWDT()` is the simplest way to watch `loop()`.

The full official list: [Arduino-ESP32 migration guide, 2.x to 3.0](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html).

## Make the AI check itself

```text
I'm on Arduino-ESP32 core 3.3.x (ESP-IDF 5.5). Before writing code,
list any APIs you plan to use that CHANGED between core 2.x and 3.x
(e.g., LEDC, timer, I2C). For each, show the OLD call and the NEW 3.x call.
Then write the sketch using ONLY the 3.x API.
```

More in [prompts/](../prompts/README.md).
