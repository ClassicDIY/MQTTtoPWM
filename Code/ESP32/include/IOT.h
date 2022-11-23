#pragma once

#include "WiFi.h"
#include "ArduinoJson.h"
#include <EEPROM.h>
extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include "Log.h"
#include "AsyncMqttClient.h"
#include <IotWebConf.h>
#include <IotWebConfUsing.h>
#include <IotWebConfESP32HTTPUpdateServer.h>
#include "Defines.h"
#include "IOTCallbackInterface.h"
#include "SSRCommandInterface.h"

namespace MQTTtoPWM
{


class IOT : public IOTCallbackInterface
{
public:
    IOT();
    void Init(SSRCommandInterface* pci);
    boolean Run();
    void Publish(const char *subtopic, const char *value, boolean retained = false);
    void Publish(const char *topic, float value, boolean retained = false);
    void Publish(const char *topic, int value, boolean retained = false);
    void PublishMessage(const char* topic, JsonDocument& payload);
    std::string getRootTopicPrefix();
    u_int getUniqueId() { return _uniqueId;};
    std::string getThingName();
    SSRCommandInterface* _pci;

private:
    bool _clientsConfigured = false;
    u_int _uniqueId = 0; // unique id from mac address NIC segment
};


} // namespace MQTTtoPWM

extern MQTTtoPWM::IOT _iot;
