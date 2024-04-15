#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define VREF    5000//VREF(mv)
#define ADC_RES 1024//ADC Resolution

uint32_t raw;

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(115200);
  // Start up the library
  sensors.begin();
}

void loop(void){ 
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  
  Serial.println("NODE 0");
  Serial.print(" C ");
  Serial.print(sensors.getTempCByIndex(0)); //OR 1
  Serial.print(", ");
  raw=analogRead(A2); //OR A3
  Serial.println("raw:\t"+String(raw)+"\tVoltage(mv)"+String(raw*VREF/ADC_RES));

  Serial.println("NODE 1");
  
  Serial.print(" C ");
  Serial.print(sensors.getTempCByIndex(1)); //OR 1
  Serial.print(", ");
  raw=analogRead(A3); //OR A3
  Serial.println("raw:\t"+String(raw)+"\tVoltage(mv)"+String(raw*VREF/ADC_RES));
  
  
  delay(1000);
}
