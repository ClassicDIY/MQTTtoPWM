#pragma once
#include <Arduino.h>

class SSRCommandInterface
{
public:
    virtual bool ProcessMQTTMessage(char *payload);
};
