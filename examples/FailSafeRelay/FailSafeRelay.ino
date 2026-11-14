// A heater relay controller that fails closed (safe = relay OFF).
//
// WARNING: if this switches mains voltage, use a properly rated, isolated
// relay module, and if you are not qualified to work with mains, don't.
// No code makes unsafe wiring safe.
//
// Safety rules, and where each is enforced:
//   1. Relay OFF before anything else at boot         -> first lines of setup()
//   2. Sensor read fails or is out of range: relay OFF -> controlStep()
//   3. Never ON above MAX_SAFE_C                       -> controlStep()
//   4. If the control loop stalls, the board resets
//      (and rule 1 turns the relay off again)          -> loop watchdog

const int RELAY_PIN = 26;          // drives a relay MODULE (transistor + diode), never a bare coil
const int THERMISTOR_PIN = 35;     // ADC1 input
const bool RELAY_ON = HIGH;        // check your module: many are active-LOW
const float TARGET_C = 45.0;
const float HYSTERESIS_C = 2.0;
const float MAX_SAFE_C = 60.0;

void relayOff() { digitalWrite(RELAY_PIN, !RELAY_ON); }
void relayOn() { digitalWrite(RELAY_PIN, RELAY_ON); }

// Replace with your sensor. Returns NAN on failure.
// Here: 10k NTC (Beta 3950) to GND, 10k resistor to 3.3 V.
float readTemperatureC() {
  uint32_t mv = analogReadMilliVolts(THERMISTOR_PIN);
  if (mv < 50 || mv > 3250) return NAN;  // open or shorted sensor
  float r = 10000.0f * mv / (3300.0f - mv);
  float kelvin = 1.0f / (1.0f / 298.15f + log(r / 10000.0f) / 3950.0f);
  return kelvin - 273.15f;
}

void controlStep() {
  static bool heating = false;
  float t = readTemperatureC();

  if (isnan(t) || t < -20 || t > 150) {  // rule 2: bad reading
    heating = false;
    relayOff();
    Serial.println("sensor fault: relay OFF");
    return;
  }
  if (t >= MAX_SAFE_C) {                 // rule 3: hard limit
    heating = false;
  } else if (t < TARGET_C - HYSTERESIS_C) {
    heating = true;
  } else if (t > TARGET_C + HYSTERESIS_C) {
    heating = false;
  }
  heating ? relayOn() : relayOff();
  Serial.printf("%.1f C, relay %s\n", t, heating ? "ON" : "OFF");
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);  // rule 1: safe state before anything else
  relayOff();

  Serial.begin(115200);
  analogReadResolution(12);
  enableLoopWDT();             // rule 4: reset if loop() stops being fed
}

void loop() {
  controlStep();
  feedLoopWDT();
  vTaskDelay(pdMS_TO_TICKS(500));
}
