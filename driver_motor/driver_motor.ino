
// Motor
#define pwma 255
#define ENA 3
#define IN1 2
#define IN2 4

#define IN3 5
#define IN4 7
#define ENB 6
#define pwmb 255



void left() {
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  analogWrite(ENA, pwma);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  analogWrite(ENB, pwmb);

}
void right() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(ENA, pwma);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  analogWrite(ENB, pwmb);
}
void forward() {
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  analogWrite(ENA, pwma);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  analogWrite(ENB, 255);
}
void reward() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(ENA, 255);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  analogWrite(ENB, pwmb);
}
void stopm() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  analogWrite(ENA, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
  analogWrite(ENB, 0);
}



void setup() {
  // Arduino
  Serial.begin(9600);
  // Motor
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  


  //  Serial.print("No");

}
