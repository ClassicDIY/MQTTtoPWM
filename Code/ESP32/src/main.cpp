#include <Arduino.h>
#include "Log.h"
#include "IOT.h"
#include "SSR.h"

#define WATCHDOG_TIMER 600000 //time in ms to trigger the watchdog

using namespace MQTTtoPWM;

MQTTtoPWM::SSR _SSR = MQTTtoPWM::SSR();
hw_timer_t *_watchdogTimer = NULL;

void IRAM_ATTR resetModule()
{
	// ets_printf("watchdog timer expired - rebooting\n");
	esp_restart();
}

void init_watchdog()
{
	if (_watchdogTimer == NULL)
	{
		_watchdogTimer = timerBegin(0, 80, true);					   //timer 0, div 80
		timerAttachInterrupt(_watchdogTimer, &resetModule, true);	  //attach callback
		timerAlarmWrite(_watchdogTimer, WATCHDOG_TIMER * 1000, false); //set time in us
		timerAlarmEnable(_watchdogTimer);							   //enable interrupt
	}
}

void feed_watchdog()
{
	if (_watchdogTimer != NULL)
	{
		timerWrite(_watchdogTimer, 0); // feed the watchdog
	}
}


void setup()
{
	Serial.begin(115200);
	while (!Serial) {}
	_iot.Init(&_SSR);
	init_watchdog();
	_SSR.begin(&_iot);
	logd("Done setup");
}

void loop()
{
	if (_iot.Run()) {
	// 	if (_lastPublishTimeStamp < millis())
	// 	{
			feed_watchdog();
	// 		unsigned long currentPublishRate = _Pylon.Transmit() == true ? _iot.PublishRate() : COMMAND_PUBLISH_RATE;
	// 		_lastPublishTimeStamp = millis() + currentPublishRate;
	// 	}
	}
	else {
		feed_watchdog(); // don't reset when not configured
	}
}
