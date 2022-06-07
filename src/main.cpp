/*************************************************************
project: <Dc/Dcc Controller>
author: <Thierry PARIS>
description: <Dcc Serial Controller sample>
*************************************************************/

#include <DCCpp.h>

#ifndef USE_TEXTCOMMAND
#error To be able to compile this sample,the line #define USE_TEXTCOMMAND must be uncommented in DCCpp.h
#endif

void setup()
{
	Serial.begin(115200);
  Serial.println("Setup Start");
	DCCpp::begin();
  // Configuration for my LMD18200. See the page 'Configuration lines' in the documentation for other samples.
#if defined(ARDUINO_ARCH_ESP32)
  DCCpp::beginMain(UNDEFINED_PIN, 33, 32, 36);
#else
  DCCpp::beginMain(UNDEFINED_PIN, DCC_SIGNAL_PIN_MAIN, 3, A0);
#endif
DCCpp::powerOn();
Serial.println("Setup End");

}

unsigned long last = 0;
int sens = 0;
int locoId = -1;
void loop()
{
  if (locoId == -1)
  {
    if ((millis() % 1000) == 0)
    {
      locoId = DCCpp::identifyLocoIdMain();
      if (locoId == -1)
      {
        Serial.println("Loco not found...");
      }
      else
      {
        Serial.print("Loco found with id: ");
        Serial.println(locoId);
      }
    }
  }
  else
  {
    DCCpp::loop();
    if (millis() - last > 7000)
    {
      last = millis();
      sens = 1 - sens;
      DCCpp::mainRegs.setThrottle(1, locoId, 30, sens);
    }
    // 	if ((millis() % 3000) == 0)
    // {
    // 	Serial.println("Scan:");
    //     Serial.println(DCCpp::identifyLocoIdMain());
    // }
  }
}