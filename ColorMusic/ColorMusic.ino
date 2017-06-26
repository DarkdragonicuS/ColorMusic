/*
 Name:		ColorMusic.ino
 Created:	16.06.2017 15:44:19
 Author:	Nixiris
*/

#include <Arduino.h>
#include <FastLED.h>
#include "CMLocalLib.h"
#include "CMUI.h"
#include "LedLogic.h"
#include <MSGEQ7.h>
#include "LedOutput.h"

#define pinAnalogLeft A0
#define pinAnalogRight A0
#define pinReset 11
#define pinStrobe 12
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH false
//bool ccode = false;
//bool flagParamsInited = false;	//Признак инициализированности всех параметров
//bool G_shellAvailable = true;

CRGB *leds = new CRGB[ledCnt];
CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

unsigned int curParams = 0x0;	//Настраиваемая конфигурация
unsigned int realParams = 0x0;	//Используемая конфигурация работы
bool configNeeded = true;

void setup() {
	Serial.begin(38400);
	//Serial.setTimeout(0);
	//strip.begin();
	FastLED.addLeds<NEOPIXEL, neoPin>(leds, ledCnt).setCorrection(0xFFB0F0);	//Добавляем светодиоды
	FastLED.setTemperature(Tungsten100W);
	FastLED.setBrightness(ledBrightness);
	Serial.setTimeout(50);
	//analogReference(INTERNAL);
	analogReference(DEFAULT);
	pinMode(neoPin, OUTPUT);		//WS2812B DIN
	pinMode(msg7DCout, INPUT);	//MSGEQ7 OUT
	pinMode(2, INPUT);
	for (char led = 0; led < ledCnt; led++)		//При включении выключаем свет (обнуляем память WS2812B)
	{
		leds[led].setColorCode(0x000000);
	}
	for (int led = 0; led < ledCnt; led++)
	{
		leds[led].setHSV(0, 255, 255);
	}
	FastLED.show();
	MSGEQ7.begin();
	//DebugMsg("");
	CMUI::Welcome();	//Приветственное сообщение
}

void loop() {
	/*int data[7];
	{
		for (int i = 0; i < 7; )
		{
			bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);
			if (newReading)
			{
				data[i] = mapNoise(MSGEQ7.get(i,0));
				Serial.print(data[i]);
				Serial.print('\t');
				i++;
			}
		}
	}
	
	Serial.println();
	*/
	//if (realParams)

	{
		LedLogic::LedPlay(leds);
	}
	CMUI::CheckUPState();
	//Serial.println(realParams, HEX);
	//Serial.print("UPSTATE: ");
	//Serial.println(digitalRead(2));

	if (configNeeded)
	{
		//curParams = MCConfigure();
		//DebugMsg("DNG");
		//curParams = Configure();
		//configNeeded = false;
	}
	//else
	{
		if (realParams != curParams)
		{
			realParams = curParams;
			Serial.println("Config set. Your current configuration:");
			Serial.print("0x");
			Serial.println(realParams, HEX);
		}
		if (G_shellAvailable)
		{
			CMUI::CommandShell();
		}
	}

	//LedOutput::LedFade(30, 2);

	

	
}
	