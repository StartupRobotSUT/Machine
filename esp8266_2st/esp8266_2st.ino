#include <Servo.h>
Servo S1;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
const uint8_t phr_sensor = D8;

const uint8_t inductive_sensor = D6;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  S1.attach(D7);
  pinMode(phr_sensor, INPUT_PULLUP); // NPN
  pinMode(inductive_sensor, INPUT);
  lcd.begin();
  lcd.print("Hello RECAN");

}

void loop() {

  // put your main code here, to run repeatedly:
  static bool last_phr_state = HIGH;
  static unsigned int can_count = 1;
  static unsigned long last_press_time;
  int phr_state = digitalRead(phr_sensor);
  unsigned long now = millis();

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
      lcd.setCursor(0, 1);
      lcd.print("can_count : ");
      lcd.setCursor(11,1);
      lcd.print(can_count);
      Serial.print(can_count);
      Serial.print(F(", duration: "));
      Serial.print(phase_duration);
      Serial.println(F(" ms"));
    }
  }


  last_phr_state = phr_state;




  bool detect = digitalRead(inductive_sensor);
  if (detect == 1) {
     lcd.setCursor(0, 2);
      lcd.print("Can");
    //  Serial.println("Can");
  }
  else {
    
      lcd.setCursor(0, 2);
      lcd.print("bottle");
  }

}

void can() {
  S1.write(0);


}
void bottle() {
  S1.write(100);
}
