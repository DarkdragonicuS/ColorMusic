/*
 * LedOps.h
 *
 *  Created on: 8 июн. 2017 г.
 *      Author: Nixiris
 */

#ifndef LEDLOGIC_H_
#define LEDLOGIC_H_
#include "CMLocalLib.h"

void LedPlay(CRGB leds[ledCnt]);
void LedPlayFreqMode(CRGB leds[ledCnt]);
void LedPlayAmpMode(CRGB leds[ledCnt]);
void LedPlayFMRange(CRGB leds[ledCnt]);
void LedPlayFMRotation(CRGB leds[ledCnt]);	//Not implemented
void LedPlayMFRGRB(int FreqVals[7], bool inv, CRGB leds[ledCnt]);  //Mode 1-1-1-1/2
void LedPlayRangeFin(long rainbow[7], int FreqVals[7], CRGB leds[ledCnt]);		//Rainbow
void LedPlayRangeFin(long color, int FreqVals[7], CRGB leds[ledCnt]);		//Single
void LedPlayRangeFin(CRGB leds[ledCnt]);		//Dynamic
unsigned int CalcColors(long color, int value, CRGB leds[ledCnt]);	//Определение нового цвета RGB для индикации амплитуды
void LedMusic(int hsvColors[ledCnt][3]);	//Зажечь светодиоды указанными HSV-цветами
char LedPlayFMRange(char mode);
char LedPlayFMRange(char mode, unsigned int opt);
void LedPlayFMRB(bool inv);
void LedPlayFMRgDyn();
void LedOff();
void LedPlayAMLTH();		//Low To High RGB
void LedPlayAMFC();			//From Center RGB
void LedPlayFMRgColor(unsigned int RGBColor);	//FM Range Single Color
void LedPlayFMRandColor();

#endif /* LEDLOGIC_H_ */
