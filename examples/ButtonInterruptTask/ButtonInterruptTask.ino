// A button interrupt that defers its work to a FreeRTOS task.
//
// Rules this follows (the ones AI-generated firmware breaks most):
// - ISRs are tiny and marked ARDUINO_ISR_ATTR (kept in IRAM).
// - Tasks wait with vTaskDelay / blocking takes, never long delay() loops,
//   so the idle task runs and the task watchdog stays happy.
// - Big buffers go on the heap, not the task stack.

const int BUTTON_PIN = 27;  // safe input with internal pull-up
const int LED_PIN = 5;

SemaphoreHandle_t buttonSemaphore;

void ARDUINO_ISR_ATTR onButton() {
  BaseType_t woken = pdFALSE;
  xSemaphoreGiveFromISR(buttonSemaphore, &woken);
  if (woken) portYIELD_FROM_ISR();
}

void buttonTask(void *) {
  // 4 KB work buffer on the heap: a local array this size could overflow the stack.
  uint8_t *buffer = (uint8_t *)malloc(4096);
  bool led = false;
  for (;;) {
    if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) == pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(30));  // debounce without blocking other tasks
      if (digitalRead(BUTTON_PIN) == LOW) {
        led = !led;
        digitalWrite(LED_PIN, led);
        if (buffer) buffer[0] = led;  // stand-in for real work
        Serial.printf("button: LED %s\n", led ? "on" : "off");
      }
      xSemaphoreTake(buttonSemaphore, 0);  // drop bounces queued meanwhile
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  buttonSemaphore = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(buttonTask, "button", 4096, nullptr, 1, nullptr, 1);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButton, FALLING);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));  // nothing to do here; yield the CPU
}
