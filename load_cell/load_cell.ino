#include "HX711.h"

//float calibration_factor = -293562.00;  // cell 1
//#define zero_factor -75762              // cell 1


float calibration_factor = -295876.00;  // cell 2
#define zero_factor  -64900              // cell 2
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  3

float offset=0;
float get_units_kg();

HX711 scale(DOUT, CLK);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Load Cell");
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);   
}
void loop() 
{ 
  Serial.print("Reading: ");
  String data = String(get_units_kg()+offset, DEC_POINT);
  Serial.print(data);
  Serial.println(" kg");
}
float get_units_kg()
{
  return(scale.get_units()*0.453592);
}
