// Deep sleep with a timer wake source.
// Configure a wake source BEFORE esp_deep_sleep_start(), or the board sleeps
// forever. For a button wake, use an RTC-capable GPIO, e.g.
// esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);

#define uS_PER_S 1000000ULL

RTC_DATA_ATTR int bootCount = 0;  // survives deep sleep

void setup() {
  Serial.begin(115200);
  delay(200);
  bootCount++;
  Serial.printf("Boot #%d, wake cause %d\n", bootCount, (int)esp_sleep_get_wakeup_cause());

  esp_sleep_enable_timer_wakeup(30 * uS_PER_S);  // wake in 30 s
  Serial.println("Sleeping...");
  Serial.flush();
  esp_deep_sleep_start();  // nothing after this runs
}

void loop() {}
