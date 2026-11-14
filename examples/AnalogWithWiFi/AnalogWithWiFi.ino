// Reading an analog sensor while Wi-Fi is on.
//
// The Wi-Fi driver takes over the ADC2 controller, so analogRead() on ADC2
// pins (GPIO 0, 2, 4, 12-15, 25-27) fails while Wi-Fi runs. Use ADC1
// (GPIO 32-39). AI tools often pick an ADC2 pin; check every analog pin.

#include <WiFi.h>
#include "arduino_secrets.h"

const int POT_PIN = 34;  // ADC1, input-only pin: fine for analog in

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 0..4095

  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
  Serial.print("Connecting");
  for (int i = 0; i < 40 && WiFi.status() != WL_CONNECTED; i++) {
    vTaskDelay(pdMS_TO_TICKS(250));
    Serial.print('.');
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? " connected" : " not connected (reading anyway)");
}

void loop() {
  int raw = analogRead(POT_PIN);
  uint32_t mv = analogReadMilliVolts(POT_PIN);  // calibrated millivolts
  Serial.printf("raw=%d  %lu mV\n", raw, (unsigned long)mv);
  vTaskDelay(pdMS_TO_TICKS(500));
}
