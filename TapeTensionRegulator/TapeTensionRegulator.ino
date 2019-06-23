#include <Arduino.h>
#include <EEPROM.h>

#ifndef ESP32
#pragma message(THIS CODE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

float y,m,c,Vin;  // y = m*x + c;

/*
 */
// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 5; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

/*
*/

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.begin(115200);

}

void loop() {
  float sensorValue;
  m = m_Gain_Knob(100);   //m = 0-2  // y = m*x + c;          
  c = c_OffSet_Knob(100); //c = +/-5 ;   // y = m*x + c;
  sensorValue = TapeRollDia(100);  // ADC0
  y = m*sensorValue + c;   // y = m*x + c;  , sensorValue is x
  if ( y > 4095) y = 4095;
  if ( y < 0) y = 0;
  dutyCycle = y/4095 * 256;
  ledcWrite(pwmChannel, dutyCycle);
  Vin = sensorValue/409.6;
  Serial.print("ADC0 : ");Serial.print(sensorValue);
  Serial.print("  Vin(0-10V): ");Serial.print(Vin);  //Vin (ADC0/4096)*10
  Serial.print("  Vout(0-100%): ");Serial.print(round(dutyCycle/2.56));
  Serial.print("  Gain(0-2): ");Serial.print(m,3);
  Serial.print("  Offse(+/-250): ");Serial.println(round(c));
  delay(1);  
  digitalWrite(motor1Pin1, HIGH);
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
    value = analogRead(A3);          //ADC3
    sum_value = sum_value + value;
  }
  avg_sampl = sum_value/sampl;
//  return 1;
  return avg_sampl/4096.0 * 2.0;   //Gain = 0-2
}

float c_OffSet_Knob (int sampl) {
  int value = 0;
  int sum_value = 0;
  float avg_sampl = 0;
  for (int i = 0; i <= sampl; i++) {
    value = analogRead(A6);          //ADC6
    sum_value = sum_value + value;
  }
  avg_sampl = sum_value/sampl;
//  return 0;
  return (avg_sampl/4096.0 * 500.0) - 250;  //Offset = +/- 250
}
