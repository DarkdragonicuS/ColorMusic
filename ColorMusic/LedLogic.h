/*
 * LedOps.h
 *
 *  Created on: 8 июн. 2017 г.
 *      Author: Nixiris
 */

#ifndef LEDLOGIC_H_
#define LEDLOGIC_H_
#include "CMLocalLib.h"

class LedLogic
{
public:
	static void LedPlay(CRGB leds[ledCnt]);
private:
	static void LedPlayFreqMode(CRGB leds[ledCnt]);
	static void LedPlayAmpMode(CRGB leds[ledCnt]);
	static void LedPlayFMRange(CRGB leds[ledCnt]);
	static void LedPlayFMRotation(CRGB leds[ledCnt]);	//Not implemented
	static void LedPlayMFRGRB(int FreqVals[7], bool inv, CRGB leds[ledCnt]);  //Mode 1-1-1-1/2
	static void LedPlayRangeFin(long rainbow[7], int FreqVals[7], CRGB leds[ledCnt]);		//Rainbow
	static void LedPlayRangeFin(long color, int FreqVals[7], CRGB leds[ledCnt]);		//Single
	static void LedPlayRangeFin(CRGB leds[ledCnt]);		//Dynamic
	static unsigned int CalcColors(long color, int value, CRGB leds[ledCnt]);	//Определение нового цвета RGB для индикации амплитуды
	static char LedPlayFMRange(char mode);
	static char LedPlayFMRange(char mode, unsigned int opt);
	static void LedPlayFMRB(bool inv);
	static void LedPlayFMRgDyn();
	static void LedPlayAMLTH();		//Low To High RGB
	static void LedPlayAMFC();			//From Center RGB
	static void LedPlayFMRgColor(unsigned int RGBColor);	//FM Range Single Color
	static void LedPlayFMRandColor();
};

class CMExecutor
{
public:
	static void LedMusic(int hsvColors[ledCnt][3]);	//Зажечь светодиоды указанными HSV-цветами
	static void LedOff();	//Отключение светодиодов
	static void Init(uint8_t ledCount);	//Инициализация светодиодной ленты
};






#endif /* LEDLOGIC_H_ */
