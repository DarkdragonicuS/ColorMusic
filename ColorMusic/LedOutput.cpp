#include "LedOutput.h"

unsigned long StartTime = 0;
bool faded = false;
bool firstLaunch = true;

void LedOutput::LedFade(uint8_t ledHigh, uint8_t ledLow)
{
	unsigned int fadeDuration = 500;
	int dtl = fadeDuration / ledCnt;
	int dtv = dtl / 10;
	int v = 0;
	if (firstLaunch)
	{
		for (int led = 0; led < ledCnt; led++)
		{
			leds[led].setColorCode(0xFF0000);
		}
		firstLaunch = !firstLaunch;
		StartTime = millis();
	}
	int timeGone = millis() - StartTime;
	for (int led = ledHigh; led > ledLow-1; led--)
	{
		if ((timeGone > (ledHigh - ledLow) - led + 1)*dtv)
		{
			//Serial.println((ledHigh - ledLow) - led + 1);
			int v = 255;
			int decV = timeGone / (((ledHigh - ledLow) - led + 1)*dtv);
			if (decV == 0)
			{
				v = 255;
			}
			else
			if (decV < 10)
			{
				v *= (float)(10 - decV)/10;
			}
			else
			{
				v = 0;
			}
			
			Serial.print(decV);
			Serial.print('\t');
			Serial.println(v);
			leds[led].setHSV(GetHueFromCRGB(leds, led), GetSatFromCRGB(leds, led), v);
		}
		
	}
	Serial.println("****");
	FastLED.show();
}
