#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads

char hexaKeys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

int in3 = 7;
int in4 = 8;
int pwmB = 9;


int in1 = 4;
int in2 = 5;
int pwmA = 6;

byte rowPins[ROWS] = {28, 26, 24, 22}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {36, 34, 32, 30}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

bool limit;

float calibration_factor = -295876.00;  // cell 2
#define zero_factor - 64900              // cell 2

#define DOUT  A3
#define CLK   A2
#define DEC_POINT  3
float offset = 0;
float get_units_kg();
#include "HX711.h"
HX711 scale(DOUT, CLK);
float get_units_kg()
{
  return (scale.get_units(
          ) * 0.453592);
}
void setup()
{
  Serial1.begin(9600);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(pwmA,OUTPUT);
  pinMode(pwmB,OUTPUT);
  

  Serial.begin(115200);
  // initialize the LCD
  lcd.begin();
  lcd.print("RECAN");
  lcd.setCursor(0, 1);
  pinMode(40, INPUT_PULLUP);
  pinMode(42, OUTPUT);
  digitalWrite(42, 0);



  scale.set_scale(calibration_factor);
  scale.set_offset(zero_factor);

  lcd.print("Student id :");
  keypad.addEventListener(keypadEvent);

}
int countcan;
int countbottle;
String uid = "";
int cur;


const uint8_t can_sensor = 38;
void  motorcan(){
  
  digitalWrite(in1,1);
  digitalWrite(in2,0);
  analogWrite(pwmA,255);
  
}

void  motorbottle(){
  
  digitalWrite(in3,1);
  digitalWrite(in4,0);
  analogWrite(pwmB,255);
  
}
void loop()
{
  bool detect = digitalRead(can_sensor);
  limit = digitalRead(40);
  if (limit == 0 && detect == 1) {
    delay(1000);
    countcan++;
    motorcan();
    // can
  }
  if (limit == 0 && detect == 0) {
    delay(1000);
    countbottle++;
    motorbottle();
  }

  char key = keypad.getKey();

  lcd.setCursor(20, 3);-
  lcd.print("Coutcan :");
  lcd.setCursor(29, 3);
  lcd.print(countcan);
  lcd.setCursor(31, 3);
  lcd.print("CoutB :");
  lcd.setCursor(38, 3);
  lcd.print(countbottle);



}

void keypadEvent(KeypadEvent key) {

  switch (keypad.getState()) {
    case PRESSED:
      if (key == '#') {
        Serial.println("#");

      }
      break;

    case RELEASED:
      if (cur < 0) {
        cur = 0;
      }
      if (key == '*') {

      } else if (key == '#') {
        lcd.setCursor(0, 2);
        lcd.print("Start Insert ...");
        Serial.print(uid);
      } else if (key == 'C') {

        uid.remove(cur - 1);
        cur--;
        lcd.clear();

        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(12, 1);
        lcd.print(uid);

        lcd.setCursor(0, 0);
        lcd.print("RECAN");
        lcd.setCursor(0, 1);
        lcd.print("Student id :");
        Serial.println(uid);
      }
      else {
        uid += key;
        lcd.setCursor(12, 1);
        lcd.print(uid);
        Serial.println(uid);
        Serial.println(uid);
        cur  = uid.length();
      }
      break;

    case HOLD:
      if (key == '*') {
        //  blink = true;    // Blink the LED when holding the * key.
        Serial.println("*");
      } else if (key == '#') {
       String str  = "uid : " + uid + ',' + "count Can : " + String(countcan) + ',' + "count bottle : " + String(countbottle);
      Serial1.println(str);
      Serial.println(str); 
      }
      break;
  }
}



