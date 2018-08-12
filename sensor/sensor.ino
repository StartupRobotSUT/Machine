/*
ตัวอย่างโคด ArduinoAll LJ18A3-8-Z
สายสีน้ำเงิน - GND
สายสีน้ำตาล - 5V
สายสีดำ - 7
*/
void setup() {
  Serial.begin(9600);
  Serial.println("ArduinoAll Example LJ18A3-8-Z");
}

void loop() {
  int detect = digitalRead(22);
  if (detect == 1) {
    Serial.println("Can");
  } else Serial.println("Bottle");
  
}
