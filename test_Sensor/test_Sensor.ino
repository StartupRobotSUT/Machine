void setup() {
  Serial.begin(9600);
pinMode(42,INPUT_PULLUP);
pinMode(44,OUTPUT);

pinMode(46,OUTPUT);

digitalWrite(44,1);

digitalWrite(46,0);

}

void loop() {
  int detect = digitalRead(42);
Serial.println(detect);
}
