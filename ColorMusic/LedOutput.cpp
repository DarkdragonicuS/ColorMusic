#include "LedOutput.h"

unsigned long time = 0;
bool faded = false;

void LedFade(char ledHigh, char ledLow)
{
	const unsigned long fadeDuration = 5000;
	int dval = 0;
	int dt = 0;
	unsigned long timeRem = 0;
	if (faded)
	{
		time = millis();
	}
	else
	{
		if (millis() - time >= fadeDuration)
		{
			for (char led = ledCnt; led > ledLow; led--)
			{
				dval = GetValFromCRGB(leds, led);
				timeRem = fadeDuration - (millis() - time);
				dt = timeRem / dval;
			}
			faded = true;
		}
	}
}