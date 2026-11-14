// Hardware timer interrupt with the Arduino-ESP32 3.x timer API.
//
// 3.x: timerBegin(frequencyHz), timerAttachInterrupt(timer, isr),
//      timerAlarm(timer, alarmValue, autoReload, reloadCount).
// 2.x (no longer compiles on 3.x): timerBegin(num, divider, countUp),
//      timerAttachInterrupt(timer, isr, edge), timerAlarmWrite + timerAlarmEnable.
//
// The ISR only signals; the real work happens in loop(). Long ISRs trip the
// interrupt watchdog.

const int LED_PIN = 2;  // on-board LED on many DevKits (a strapping pin, fine as an output after boot)

hw_timer_t *timer = nullptr;
volatile SemaphoreHandle_t timerSemaphore;

void ARDUINO_ISR_ATTR onTimer() {
  xSemaphoreGiveFromISR(timerSemaphore, nullptr);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  timerSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(1000000);              // 1 MHz tick = 1 microsecond
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 500000, true, 0);       // every 0.5 s, auto-reload, forever
}

void loop() {
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.println("tick");
  }
}
