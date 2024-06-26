#include <OneWire.h>
#include <DallasTemperature.h>

#include <ArduinoJson.h>
#include "DFRobot_PH.h"
#include <EEPROM.h>

#include <Arduino.h>

#define ONE_WIRE_BUS 4

#define PH_PIN_0 A0
#define PH_PIN_1 A1 

#define DO_PIN_0 A2
#define DO_PIN_1 A3

//DO definitions
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution

// Dissolved_Oxygen Calibration
//CAL1 High temperature point, CAL2 Low temperature point
//Node 0
#define CAL_High_V_0 (1030) //mv
#define CAL_High_T_0 (23.19)   //℃

#define CAL_Low_V_0 (903) //mv
#define CAL_Low_T_0 (21.31)   //℃

//Node 1
#define CAL_High_V_1 (1245) //mv
#define CAL_High_T_1 (23.19)   //℃

#define CAL_Low_V_1 (844) //mv
#define CAL_Low_T_1 (21.31)   //℃

// oneWire instance for temperature sensors
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Create Json here 
JsonDocument doc;

float voltage_0,phValue_0,temp_0 = 0;
float voltage_1,phValue_1,temp_1 = 0;

DFRobot_PH ph;

const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};

uint16_t ADC_Raw_0;
uint16_t ADC_Voltage_0;
uint16_t DO_0=0;

uint16_t ADC_Raw_1;
uint16_t ADC_Voltage_1;
uint16_t DO_1=0;

int16_t readDO_0(uint32_t voltage_mv, uint8_t temperature_c)
{
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL_Low_T_0) * ((uint16_t)CAL_High_V_0 - CAL_Low_V_0) / ((uint8_t)CAL_High_T_0 - CAL_Low_T_0) + CAL_Low_V_0;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
}

int16_t readDO_1(uint32_t voltage_mv, uint8_t temperature_c)
{
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL_Low_T_1) * ((uint16_t)CAL_High_V_1 - CAL_Low_V_1) / ((uint8_t)CAL_High_T_1 - CAL_Low_T_1) + CAL_Low_V_1;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
}

void setup(void)
{
  // Start serial communication
  Serial.begin(115200);
  // Start up the library
  ph.begin();
  sensors.begin();
  // initialize as to create a standard JSON for reliable deserialization
  doc["temperature"][0] = temp_0;
  doc["temperature"][1] = temp_1;
  
  doc["pH"][0] = phValue_0;
  doc["pH"][1] = phValue_1;

  doc["dissolved_oxygen"][0] = DO_0;
  doc["dissolved_oxygen"][1] = DO_1;
}

void loop(void){ 
  //while (!Serial) continue; //?? Necessary? Proboably to prevent from writing on top of itself

  sensors.requestTemperatures();
  
  temp_0 = sensors.getTempCByIndex(0);
  voltage_0 = analogRead(PH_PIN_0)/1024.0*5000;  // read the voltage
  phValue_0 = ph.readPH(voltage_0,temp_0);
  
  temp_1 = sensors.getTempCByIndex(1);
  voltage_1 = analogRead(PH_PIN_1)/1024.0*5000;  // read the voltage
  phValue_1 = ph.readPH(voltage_1,temp_1);


  ADC_Raw_0 = analogRead(DO_PIN_0);
  ADC_Voltage_0 = uint32_t(VREF) * ADC_Raw_0 / ADC_RES;
  DO_0=readDO_0(ADC_Voltage_0, temp_0);

  ADC_Raw_1 = analogRead(DO_PIN_1);
  ADC_Voltage_1 = uint32_t(VREF) * ADC_Raw_1 / ADC_RES;
  DO_1=readDO_1(ADC_Voltage_1, temp_1);
  
  doc["temperature"][0] = temp_0;
  doc["temperature"][1] = temp_1;

  doc["pH"][0] = phValue_0;
  doc["pH"][1] = phValue_1;

  doc["dissolved_oxygen"][0] = DO_0;
  doc["dissolved_oxygen"][1] = DO_1;

  // Prints the JSON on the bus and adds newline
  serializeJson(doc, Serial);
  Serial.println();
  //Refreshrate
  delay(1000);
}
