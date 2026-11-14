// PWM fade with the Arduino-ESP32 3.x LEDC API.
//
// 3.x: ledcAttach(pin, freq, resolution) then ledcWrite(pin, duty).
// Channels are assigned automatically.
// 2.x (no longer compiles on 3.x): ledcSetup(channel, freq, res) +
// ledcAttachPin(pin, channel) + ledcWrite(channel, duty).

const int LED_PIN = 5;           // safe output pin on ESP32 DevKit boards
const uint32_t PWM_FREQ = 5000;  // 5 kHz
const uint8_t PWM_BITS = 8;      // duty 0..255

void setup() {
  Serial.begin(115200);
  if (!ledcAttach(LED_PIN, PWM_FREQ, PWM_BITS)) {
    Serial.println("ledcAttach failed");
  }
}

void loop() {
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_PIN, duty);
    delay(5);
  }
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(LED_PIN, duty);
    delay(5);
  }
}
