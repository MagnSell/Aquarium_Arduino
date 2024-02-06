#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>


// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Create Json here 
JsonDocument doc;

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  // initialize as to create a standard JSON for reliable deserialization
  doc["temperature"][0] = 0;
  doc["temperature"][1] = 0;
}

void loop(void){ 
  while (!Serial) continue;
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  doc["temperature"][0] = sensors.getTempCByIndex(0);
  doc["temperature"][1] = sensors.getTempCByIndex(1);

  // Prints the JSON on the bus and adds newline
  serializeJson(doc, Serial);
  Serial.println();
  //Refreshrate
  delay(1000);
}
