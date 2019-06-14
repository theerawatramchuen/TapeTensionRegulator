#include <Arduino.h>
#include <EEPROM.h>

//#ifndef ESP32
//#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
//#error Select ESP32 board.
//#endif


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  58        /* Time ESP32 will go to sleep (in seconds) */

#define LED1 17
#define SW1 5
#define SW_Press false
#define SW_Release true

float gain_a;
float offset_b;
int temp = 0;

//////////////////////////////////////////////////////
void setup() 
{ 
Serial.begin(115200);
digitalWrite(LED1, false);
EEPROM.get(0, gain_a);
EEPROM.get(sizeof(gain_a), offset_b);
}

void loop()
{Read_tape_diameter(); 
 y_Calculation();
 Output_pwm_y();
 if (digitalRead(SW1) == SW_Press)
    { 
     delay(500);
     if (digitalRead(SW1) == SW_Press)
        {digitalWrite(LED1, false);
         gain_a = gain_a+1;
         offset_b = gain_a;
         Save_current_tape_profile();
        }
     }
Shift2next_profile();

delay(1000); Serial.print("Gain:");Serial.print(gain_a);Serial.print("  Offset:");Serial.println(offset_b);
Serial.print("SW1 Status: ");Serial.println(digitalRead(SW1));
}

/////////////////////////////////////////////////////

void Initialize()
{ Serial.println("Clear motor PWM to 0%");
  pinMode(LED1, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  load();

}

void Read_last_tape_profile() 
{ //Serial.println("Read Last tape profile gain_a and offset_b");
  EEPROM.get(0, gain_a);
  EEPROM.get(sizeof(gain_a), offset_b);
}

void Read_tape_diameter()
{ //Serial.println("Read Tape roll Diameter");
}

void y_Calculation()
{ //Serial.println("Calculate y = ax+b ");
}

void Output_pwm_y()
{ //Serial.println("Output_pwm_y");
}

boolean Read_input_sw()
{ //Serial.println("Read Input Switch");
  digitalWrite(LED1, digitalRead(SW1));
  if (digitalRead(SW1) == false)
      { delay (800);
        if (digitalRead(SW1) == false)
           { 
             return SW_Press;
           }
      }
  return SW_Release;
}

void Shift2next_profile()
{ //Serial.println("Shift to Next Tape Profile");
}

void Save_current_tape_profile()
{ //Serial.println("Save current tape profile to EEPROM");
  EEPROM.put(0, gain_a);
  EEPROM.put(sizeof(gain_a), offset_b);
//  EEPROM.commit();

}

void Save()
{
  EEPROM.put(0, gain_a);
  EEPROM.put(36, offset_b);
//  EEPROM.commit();
 }

void load()
{
    EEPROM.get(0, gain_a);
    EEPROM.get(36, offset_b);
 }
