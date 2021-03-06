// wheeCAIR Air Quality Sensor Code
// (c) 2019-2021 Al Fischer, dfischer@wcu.edu
// written for Teensy 3.5 + BME680 (Seeed Studio) + Honeywell HPMA PM sensor

String deviceID = "SUTTONBRANCH2"; // change length of array in 3 places

#include "wheeCAIR.h"

void setup() {
  pinMode(33, OUTPUT);
  turnSensorsOn();
  
  ////////// set the measurement interval (RtcTimer) depending on use case
  ////////// for battery power, 10 minutes gives a batter life of ~1 month
  ////////// for mains power, 10 seconds is a good value
  alarm.setRtcTimer(0, 0, 10); // h, m, s
    
  pinMode(LED_BUILTIN, OUTPUT);
  sdCard.spiClockPin(chipSelect);
  createFileName(year(), month(), day()).toCharArray(dataFileName, 16);
  setBMESamplingParameters();
  Serial.begin(9600);
  checkConnection();
  turnSensorsOff();
}

void loop() {
  Snooze.hibernate(config_teensy35); 
  turnSensorsOn();
  bme.performReading();
  float pavg = performPMReading(30);
  dataString = printData(deviceID, bme.temperature, bme.humidity, bme.pressure, bme.gas_resistance, pavg);
  delay(100);
//  Serial.println(dataString); // uncomment to use serial port
  delay(1000);
  writeFile(dataFileName, dataString);
  delay(500);
  turnSensorsOff();
}
