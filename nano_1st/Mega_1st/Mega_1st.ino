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

byte rowPins[ROWS] = {28, 26, 24, 22}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {36, 34, 32, 30}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
void setup()
{
  Serial.begin(9600);
  // initialize the LCD
  lcd.begin();
  lcd.print("RECAN");
  lcd.setCursor(0, 1);
  lcd.print("Student id :");
  keypad.addEventListener(keypadEvent);

}
int countCan;
int countBottle;
String uid = "";
int cur;


const uint8_t can_sensor = 38;
void loop()
{
  char key = keypad.getKey();

  bool detect = digitalRead(can_sensor);
  lcd.setCursor(20, 3);
  lcd.print("Coutcan :");
   lcd.setCursor(29, 3);
   lcd.print(8);
  lcd.setCursor(31, 3);
  lcd.print("CoutB :");
  lcd.setCursor(38, 3);
   lcd.print(7);
  if (detect == 1) {
  
  } else {

  }
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

      }
      break;
  }
}



