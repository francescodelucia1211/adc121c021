// File: adc121c021.h
// Autore: ChatGPT per Francesco De Lucia
// Descrizione: Componente ESPHome personalizzato per DFRobot DFR1184 (ADC121C021)

#include "esphome.h"

class ADC121C021Sensor : public PollingComponent, public Sensor {
 public:
  ADC121C021Sensor(uint8_t address) : PollingComponent(5000), address_(address) {}

  void setup() override {
    Wire.begin();
  }

  void update() override {
    Wire.beginTransmission(address_);
    Wire.write(0x00);  // Registro conversion result
    Wire.endTransmission(false);
    Wire.requestFrom(address_, (uint8_t)2);

    if (Wire.available() == 2) {
      uint16_t raw = (Wire.read() << 8) | Wire.read();
      float voltage = raw * 3.3 / 4096.0 * 4.55;  // Calcolo della tensione (0–10V)
      publish_state(voltage);
    } else {
      ESP_LOGW("adc121c021", "Nessuna risposta dal modulo ADC");
    }
  }

 private:
  uint8_t address_;
};
