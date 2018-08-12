#include "HX711.h"
#include <Servo.h>

const uint8_t phr_sensor = 26;
int countCan;
int countBottle;


Servo myservo;  // create servo object to control a servo
Servo to;

//float calibration_factor = -293562.00;  // cell 1
//#define zero_factor -75762              // cell 1


float calibration_factor = -295876.00;  // cell 2
#define zero_factor - 64900              // cell 2
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  3

float offset = 0;
float get_units_kg();

HX711 scale(DOUT, CLK);

void setup()
{

  Serial.begin(115200);
//  Serial.println("Load Cell");
  scale.set_scale(calibration_factor);
  scale.set_offset(zero_factor);
  myservo.attach(9);
  to.attach(10);
  pinMode(phr_sensor, INPUT_PULLUP); // NPN
}
void loop()
{

  static bool last_phr_state = HIGH;
  static unsigned int can_count = 1;
  static unsigned long last_press_time;
  int phr_state = digitalRead(phr_sensor);
  unsigned long now = millis();


  float unit = get_units_kg() + offset;

  if (unit < 0.011) {
    Serial.println(unit);
   // Serial.println("kg");
    motorpop();
  }
  else
  {
   // Serial.print(unit);
   // Serial.println("reject");
    reject();
  }

  bool detect = digitalRead(22);
  if (detect == 1) {
   // Serial.println("Can");
    tocan();
    // Record when the phr is pressed.
    if (last_phr_state == HIGH && phr_state == LOW) {
      can_count++;
      last_press_time = now;
    }

    // Report when the phr is released.
    if (last_phr_state == LOW && phr_state == HIGH) {
      unsigned long phase_duration = now - last_press_time;
      if (phase_duration < 10) {  // discard bounce
        can_count--;
      }
      else {
        Serial.print(F("pushes: "));
        Serial.print(can_count);
        Serial.print(F(", duration: "));
        Serial.print(phase_duration);
        Serial.println(F(" ms"));
      }
    }



  } else {
   // Serial.println("Bottle");
    tobottle();
  }



  last_phr_state = phr_state;










}
float get_units_kg()
{
  return (scale.get_units() * 0.453592);
}

void tocan() {
  to.write(0);


}
void tobottle() {
  to.write(100);
}
void motorpop() {
  myservo.write(0);              // tell servo to go to position in variable 'pos'

}

void reject() {
  myservo.write(100);              // tell servo to go to position in variable 'pos'

}

