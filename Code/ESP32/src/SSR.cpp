
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
void SSR::SetDutyCycle(int percent) { 
    if (percent > MIN_DUTY_CYCLE && percent < MAX_DUTY_CYCLE) {
        if (percent != _currentDutyCycle) { // any change?
            ledcWrite(PWM_Ch, percent);
            _currentDutyCycle = percent;
            _pcb->Publish("dutyCycle", percent, false);
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
            int dutyCycle = doc["dutyCycle"];
            if (dutyCycle >= MIN_DUTY_CYCLE && dutyCycle <= MAX_DUTY_CYCLE)
            {
                SetDutyCycle(dutyCycle);
                messageProcessed = true;
                logd("dutyCycle set to %d", dutyCycle);
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