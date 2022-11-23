#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "log.h"
#include "IOTCallbackInterface.h"
#include "SSRCommandInterface.h"
#include "Defines.h"

namespace MQTTtoPWM
{

class SSR : public SSRCommandInterface
{
    
public:
	SSR();
    void begin(IOTCallbackInterface* pcb);
    void SetDutyCycle(int percent);
    bool ProcessMQTTMessage(char *payload);

protected:
    IOTCallbackInterface* _pcb;
private:
    int _currentDutyCycle = 0;
};
} // namespace MQTTtoPWM