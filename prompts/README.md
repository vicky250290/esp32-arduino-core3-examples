# Prompts for AI-assisted ESP32 firmware

## Project rules file (AGENTS.md / CLAUDE.md / .cursor/rules)

Put this in the repo so every session starts with your board, core version and pin rules. Edit the first line for your board.

```markdown
# Firmware project rules
- Board: ESP32-S3-DevKitC-1. Framework: Arduino-ESP32 core 3.3.x (ESP-IDF 5.5).
- Use ONLY the 3.x API. Never use ledcSetup/ledcAttachPin or the old
  timerBegin(num, divider, countUp).
- Safe output pins on this board only. Never assign GPIO 6–11 (SPI flash).
- Analog sensors must use ADC1 pins because Wi-Fi is enabled.
- After any change, run `pio run`. Paste the full error output before editing.
- Never use delay() inside a FreeRTOS task longer than 50 ms; use vTaskDelay.
- Cite the doc page for any peripheral API you use.
```

## Surface the version trap

```text
I'm on Arduino-ESP32 core 3.3.x (ESP-IDF 5.5). Before writing code,
list any APIs you plan to use that CHANGED between core 2.x and 3.x
(e.g., LEDC, timer, I2C). For each, show the OLD call and the NEW 3.x call.
Then write the sketch using ONLY the 3.x API.
```

## A peripheral request with guardrails

```text
Write an Arduino-ESP32 3.x sketch for an ESP32-WROOM-32 that:
- reads a potentiometer on an ADC1 pin (Wi-Fi is ON, so NOT ADC2),
- drives an LED with PWM using ledcAttach()/ledcWrite() (NOT ledcSetup),
- avoids GPIO 6–11 and never uses 34–39 for the LED.
State which pins you chose and why each is safe.
```

## Force citations (ESP-IDF)

```text
For each ESP-IDF function you used, cite the exact v5.x API-reference page
and confirm it is NOT from the legacy driver. Flag anything you are unsure about.
```

## Review a task for FreeRTOS hazards

```text
Review this task for FreeRTOS hazards: any delay() over 50 ms, blocking
calls, ISR work that should be deferred, or large stack allocations.
Rewrite using vTaskDelay and heap allocation where needed.
```

## Fail-closed control

```text
This controls a heater relay. Safety rules, enforce them:
- On boot, relay OFF before anything else.
- If the temperature read fails or the sensor times out, force relay OFF.
- Never allow relay ON above 60 °C. Add a watchdog that turns it OFF if
  the control loop stalls. Show where each rule is enforced in code.
```

## Decode a crash

```text
Here is the panic and backtrace from the serial monitor, plus my sketch.
Panic type: LoadProhibited. Decoded backtrace points to line 42.
Explain in plain English what LoadProhibited means here and the most
likely cause, then propose a minimal fix.
```

---

From **[Vibe-Coding Your ESP32: AI-Assisted Firmware Without the Magic Smoke](https://www.techbrosebooks.co.in/books/vibe-coding-your-esp32-ai-assisted-firmware-without-the-magic-smoke?utm_source=github&utm_medium=referral&utm_campaign=esp32-arduino-core3-examples)**.
