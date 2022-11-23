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
    void SetDutyCycle(uint32_t percent);
    bool ProcessMQTTMessage(char *payload);

protected:
    IOTCallbackInterface* _pcb;
private:
    uint32_t _currentDutyCycle = 0;
};
} // namespace MQTTtoPWM