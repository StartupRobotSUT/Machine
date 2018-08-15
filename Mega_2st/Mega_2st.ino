#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
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
// driver motor
#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
#define CS_THRESHOLD 100
int inApin[2] = {7, 4};  // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {5, 6}; // PWM input
int cspin[2] = {2, 3}; // CS: Current sense ANALOG input
int enpin[2] = {0, 1}; // EN: Status of switches output (Analog pin)

int statpin = 13;


byte rowPins[ROWS] = {28, 26, 24, 22}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {36, 34, 32, 30}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// limit switch
bool limit_sensor;
int in_limit = 38;
int gr_limit = 40;

// inductive sensor // can sensor
bool can_sensor;
int in_can = 42;
int pr_can = 44;
int gr_can = 46;

bool phr_sensor;
int in_phr = 48;
int pr_phr = 50;
int gr_phr = 52;

void setup()
{

  pinMode(statpin, OUTPUT);
  Serial1.begin(9600);

  for (int i = 0; i < 2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  // Initialize braked
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }

  pinMode(38, INPUT_PULLUP);
  pinMode(gr_limit, OUTPUT);
  digitalWrite(gr_limit, LOW);

  pinMode(in_can, INPUT);
  pinMode(pr_can, OUTPUT);
  pinMode(gr_can, OUTPUT);
  digitalWrite(pr_can, HIGH);
  digitalWrite(gr_can, LOW);

  pinMode(in_phr, INPUT_PULLUP);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);
  digitalWrite(50, HIGH);
  digitalWrite(52, LOW);




  Serial.begin(115200);
  // initialize the LCD
  lcd.begin();

  lcd.setCursor(0, 0);
  lcd.print("RECAN");
  lcd.setCursor(0, 1);
  lcd.print("Student id :");
  keypad.addEventListener(keypadEvent);

}
int countcan;
int countbottle;
String uid = "";
int cur;
String str;

void motorOff(int motor)
{
  // Initialize braked
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
  analogWrite(pwmpin[motor], 0);
}

/* motorGo() will set a motor going in a specific direction
  the motor will continue going in that direction, at that speed
  until told to do otherwise.

  motor: this should be either 0 or 1, will selet which of the two
  motors to be controlled

  direct: Should be between 0 and 3, with the following result
  0: Brake to VCC
  1: Clockwise
  2: CounterClockwise
  3: Brake to GND

  pwm: should be a value between ? and 1023, higher the number, the faster
  it'll go
*/
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  if (motor <= 1)
  {
    if (direct <= 4)
    {
      // Set inA[motor]
      if (direct <= 1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct == 0) || (direct == 2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], pwm);
    }
  }
}

void gogo() {
  motorGo(0, CW, 1023);
  motorGo(1, CCW, 1023);
  // delay(500);
}

void backback() {
  motorGo(0, CCW, 1023);
  motorGo(1, CW, 1023);
  //delay(500);
}

void left() {
  motorGo(0, CCW, 0);
  motorGo(1, CW, 1023);
  //delay(500);
}

void rigth() {
  motorGo(0, CW, 0);
  motorGo(1, CCW, 1023);
  //delay(500);
}

void stopm() {
  motorGo(0, CW, 0);
  motorGo(1, CCW, 0);
  //delay(500);
}


void loop()
{
 
  phr_sensor = digitalRead(48);//  มี  = 0 ไม่มี = 1
  limit_sensor = digitalRead(38); // หนัก  = 1 เบา = 0 
  can_sensor = digitalRead(42);  //ป๋อง  = 1 ขวด  = 0

  


  if (limit_sensor == 1) {  // หนักเกิ้น 
    while (1) {
      limit_sensor = digitalRead(38);
      if (limit_sensor == 0 ) {
        stopm();
        break;
      } else backback();
    }

  }

  if(phr_sensor == 0 && can_sensor == 1 ){
    while(1){
      can_sensor = digitalRead(42);  //ป๋อง  = 1 ขวด  = 0
      phr_sensor = digitalRead(48);//  มี  = 0 ไม่มี = 1
      if(phr_sensor == 1){
        stopm();
        countCan++;
        break;
      }else left();

    }
  }
    if(phr_sensor == 0 && can_sensor == 0 ){
    while(1){
      can_sensor = digitalRead(42);  //ป๋อง  = 1 ขวด  = 0
      phr_sensor = digitalRead(48);//  มี  = 0 ไม่มี = 1
      if(phr_sensor == 1){
        stopm();
        countbottle++;
        break;
      }else rigth();

    }
  }

/*
  if (phr_sensor == 0) {
    while (1) {
      phr_sensor = digitalRead(48);
      if (phr_sensor == 1) {
        stopm();
        countbottle++;
        break;
      }
      else     rigth();
    }
  }
  */


  char key = keypad.getKey();
  lcd.setCursor(0, 3);
  lcd.print("Coutcan");
  lcd.setCursor(8, 3);
  lcd.print(countcan);
  lcd.setCursor(11, 3);
  lcd.print("CoutB");
  lcd.setCursor(16, 3);
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
        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("RECAN");
        lcd.setCursor(0, 1);
        lcd.print("SendComplete");
        delay(2000);
        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("RECAN");
        lcd.setCursor(0, 1);
        lcd.print("Student id :");
        lcd.setCursor(12, 1);
        uid ="";
        countcan = 0;
        countbottle = 0;
        lcd.print(uid);
        String str  = "uid : " + uid + ',' + "count Can : " + String(countcan) + ',' + "count bottle : " + String(countbottle);
        Serial1.println(str);
        Serial.println(str);
      }
      break;
  }
}



