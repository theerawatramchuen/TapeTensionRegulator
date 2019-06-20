#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display

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
  m = 0.1;  // y = m*x + c;          
  c = 0.0;   // y = m*x + c;

  lcd.init(); // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello world");
  lcd.setCursor(1,0);
  lcd.print("ESP32 I2C LCD");
}

void loop() {
  int sensorValue;
  sensorValue = analogRead(A0);
  y = m*sensorValue + c;   // y = m*x + c;  , sensorValue is x
  if ( y > 4095) y = 4095;
  if ( y < 0) y = 0;
  ledcAnalogWrite(VOUT_CHANNEL_0, y);
  Serial.print(sensorValue);Serial.print("   ");Serial.println(y,3);
  delay(1);  
}
