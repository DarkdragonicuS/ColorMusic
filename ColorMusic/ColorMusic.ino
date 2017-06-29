/*
 Name:		ColorMusic.ino
 Created:	16.06.2017 15:44:19
 Author:	Nixiris
*/

//#include <Arduino.h>
#include <FastLED.h>
#include "CMLocalLib.h"
#include "CMUI.h"
#include "LedLogic.h"
//#include <MSGEQ7.h>
#include "LedOutput.h"
#include <SoftwareSerial.h>

//#define pinAnalogLeft A0
//#define pinAnalogRight A0
//#define pinReset 11
//#define pinStrobe 12
//#define MSGEQ7_INTERVAL ReadsPerSecond(50)
//#define MSGEQ7_SMOOTH false
//bool ccode = false;
//bool flagParamsInited = false;	//������� �������������������� ���� ����������
//bool G_shellAvailable = true;

//CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

unsigned int configParamsTmp = 0x0;	//������������� ������������
unsigned int configParams = 0x0;	//������������ ������������ ������
bool configNeeded = true;
SoftwareSerial btSerial = SoftwareSerial(btRX, btRX);	//��������� ������� RX � TX ��� ����������������� ����������
CRGB *ledStrip = new CRGB[ledCnt];	//�������� ������� ������������ �����

void setup() {
	//Serial.begin(38400);
	
	btSerial.begin(38400);	//��������� �������� ������ �� ����������������� ����������
	//Serial.setTimeout(0);
	//strip.begin();
	//FastLED.addLeds<NEOPIXEL, neoPin>(leds, ledCnt).setCorrection(0xFFB0F0);	//��������� ����������
	//FastLED.setTemperature(Tungsten100W);
	//FastLED.setBrightness(ledBrightness);
	CMExecutor::Init(ledCnt);	//������������� ������ NeoPixel
	btSerial.setTimeout(50);
	//analogReference(INTERNAL);
	analogReference(DEFAULT);
	pinMode(neoPin, OUTPUT);		//��������� ������ ������ �� DIN-���� WS2812B
	pinMode(msg7DCout, INPUT);	//��������� ������ ����� � ����� OUT MSGEQ7
	pinMode(ModeButton, INPUT);	//��������� ������ ������ ������� � ������ Mode
	//LedLogic::LedPlay(leds);
	/*for (char led = 0; led < ledCnt; led++)		//��� ��������� ��������� ���� (�������� ������ WS2812B)
	{
		leds[led].setColorCode(0xFFFFFF);
	}
	for (int led = 0; led < ledCnt; led++)
	{
		leds[led].setHSV(0, 255, 255);
	}
	FastLED.show();*/
	
//	MSGEQ7.begin();
	//DebugMsg("");
	CMUI::Welcome();	//�������������� ���������
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
		LedLogic::LedPlay(ledStrip);
	}
	CMUI::ModeButton();
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
		if (configParams != configParamsTmp)
		{
			configParams = configParamsTmp;
			btSerial.println("Config set. Your current configuration:");
			btSerial.print("0x");
			btSerial.println(configParams, HEX);
		}
		//if (G_shellAvailable)
		{
			CMUI::CommandShell();
		}
	}

	//LedOutput::LedFade(30, 2);

	

	
}
	