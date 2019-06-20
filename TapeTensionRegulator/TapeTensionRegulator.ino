#include <Arduino.h>
#include <EEPROM.h>

#ifndef ESP32
#pragma message(THIS CODE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

float y,m,c;  // y = m*x + c;

/*
 */

#define VOUT_CHANNEL_0     0   // use first channel of 16 channels (started from zero)
#define LEDC_TIMER_12_BIT  12   // use 12 bit precission for LEDC timer
#define LEDC_BASE_FREQ     5000  // use 5000 Hz as a LEDC base frequency
#define LED_PIN            5 //Voutput --> GPIO5 // fade LED PIN (replace with LED_BUILTIN constant for built-in LED)

// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 4095) {
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);   // calculate duty, 4095 from 2 ^ 12 - 1
  ledcWrite(channel, duty);    // write duty to LEDC
}

/*
*/

void setup() {
  Serial.begin(115200);
  ledcSetup(VOUT_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttachPin(LED_PIN, VOUT_CHANNEL_0);
}

void loop() {
  float sensorValue;
  m = m_Gain_Knob(100);   //m = 0-2  // y = m*x + c;          
  c = c_OffSet_Knob(100); //c = +/-5 ;   // y = m*x + c;
  sensorValue = TapeRollDia(100);  // ADC0
  y = m*sensorValue + c;   // y = m*x + c;  , sensorValue is x
  if ( y > 4095) y = 4095;
  if ( y < 0) y = 0;
  y = y/4096 * 100.0;
  ledcAnalogWrite(VOUT_CHANNEL_0, y);
  Serial.print("ADC0 : ");Serial.print(sensorValue);
  Serial.print("  Vout(0-100%): ");Serial.print(y,3);
  Serial.print("  Gain(0-2): ");Serial.print(m,3);
  Serial.print("  Offse(+/-5): ");Serial.println(c,3);
  delay(1);  
}

float TapeRollDia (int sampl) {
  int value = 0;
  int sum_value = 0;
  float avg_sampl = 0;
  for (int i = 0; i <= sampl; i++) {
    value = analogRead(A0);          //ADC0
    sum_value = sum_value + value;
  }
  avg_sampl = sum_value/sampl;
  return avg_sampl;
}

float m_Gain_Knob (int sampl) {
  int value = 0;
  int sum_value = 0;
  float avg_sampl = 0;
  for (int i = 0; i <= sampl; i++) {
    value = analogRead(A0);          //ADC3
    sum_value = sum_value + value;
  }
  avg_sampl = sum_value/sampl;
  return avg_sampl/4096.0 * 2.0;   //Gain = 0-2
}

float c_OffSet_Knob (int sampl) {
  int value = 0;
  int sum_value = 0;
  float avg_sampl = 0;
  for (int i = 0; i <= sampl; i++) {
    value = analogRead(A0);          //ADC6
    sum_value = sum_value + value;
  }
  avg_sampl = sum_value/sampl;
  return (avg_sampl/4096.0 * 10.0) - 5;  //Offset = +/- 5
}
