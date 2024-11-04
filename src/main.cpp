#include <HX711_ADC.h>
#include <Arduino.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// Pins:
const int HX711_dout = 3; // MCU > HX711 dout pin
const int HX711_sck = 2;  // MCU > HX711 sck pin
const int led_pin = 4;

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAddress = 0;

void setup() {
  Serial.begin(9600);
  LoadCell.begin();
  pinMode(led_pin, OUTPUT);
  float calValue;
  EEPROM.get(calVal_eepromAddress, calValue);
  if (calValue == -55) { 
    calValue = 1.0; 
  }
  LoadCell.setCalFactor(calValue);

  unsigned long stabilizingTime = 2000; 
  boolean _tare = true; 
  LoadCell.start(stabilizingTime, _tare);

  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    Serial.println("Startup is complete");
  }
}

void loop() {
  if (LoadCell.update()) {
    float weight = LoadCell.getData();
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    if (weight < 100)
    {
      digitalWrite(led_pin, HIGH);
    }
    else{
      digitalWrite(led_pin, LOW);
    }
    
  }

  delay(250); 
}
