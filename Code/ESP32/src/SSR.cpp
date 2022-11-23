
#include "SSR.h"
#include "Log.h"

namespace MQTTtoPWM
{

SSR::SSR()
{
    _currentDutyCycle = 0;
}

void SSR::begin(IOTCallbackInterface* pcb) { 
    _pcb = pcb;
    ledcAttachPin(SSR_PIN, PWM_Ch);
    ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
};

//SSRCommandInterface
void SSR::SetDutyCycle(uint32_t percent) { 
    if (percent >= MIN_DUTY_CYCLE && percent <= MAX_DUTY_CYCLE) {
        if (percent != _currentDutyCycle) { // any change?
            _currentDutyCycle = percent;
            // conversion from max=100% PWM to 8-bits output
            float dutyCycle = percent;
            dutyCycle *= 255.0/100;
            ledcWrite(PWM_Ch, abs(dutyCycle));
            _pcb->Publish("dutyCycle", (int)percent, false);
            logd("dutyCycle set to %d (%f)", percent, dutyCycle);
        }
    }
};

bool SSR::ProcessMQTTMessage(char *payload) {
    boolean messageProcessed = false;
    StaticJsonDocument<64> doc;
	DeserializationError err = deserializeJson(doc, payload);
	if (err) // not json!
	{
		logd("MQTT payload {%s} is not valid JSON!", payload);
	}
	else 
	{
        if (doc.containsKey("dutyCycle") && doc["dutyCycle"].is<int>())
        {
            uint32_t dutyCycle = doc["dutyCycle"];
            if (dutyCycle >= MIN_DUTY_CYCLE && dutyCycle <= MAX_DUTY_CYCLE)
            {
                SetDutyCycle(dutyCycle);
                messageProcessed = true;
            }
            else
            {
                logd("dutyCycle is out of rage!");
            }
        }
    }
    return messageProcessed;
}

}// namespace MQTTtoPWM