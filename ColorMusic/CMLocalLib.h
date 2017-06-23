/*
 * CMLocalLib.h
 *
 *  Created on: 27 мая 2017 г.
 *      Author: Nixiris
 */

#ifndef CMLOCALLIB_H_
#define CMLOCALLIB_H_
#include <FastLED.h>

//DEBUG:
void DebugMsg(String message);

//MSGEQ7:
#define msg7RESET 11
#define msg7Strobe 12
#define msg7DCout A0

//WS2812B
#define neoPin 6
//#define ledCnt 30
#define ledBrightness 255

const int cParamCnt = 5;


//Color Constants:
const long cColorRed	 = 0xFF0000;		//000h255s255v
const long cColorOrange	 = 0xFFA500;		//028h255s255v
const long cColorYellow  = 0xFFFF00;		//043h255s255v
const long cColorGreen	 = 0x00FF00;		//085h255s255v
const long cColorLtBlue	 = 0x87CEBB;		//116h088s206v
const long cColorBlue	 = 0x0000FF;		//170h255s255v
const long cColorViolet	 = 0xEE82EE;		//213h116s238v

//extern long preParams[cParamCnt];	//Конфигурация параметров
extern unsigned int realParams;
extern bool configNeeded;
extern int ledCnt;
//extern bool flagParamsInited;
//extern bool G_shellAvailable;
extern CRGB *leds;

void ShowParams();	//Вывод конфигурации на сериал
//void ParamsJustInited();

/*
void LedPlay(CRGB leds[ledCnt]);	//Not implemented
void LedPlayFreqMode(CRGB leds[ledCnt]);
void LedPlayAmpMode(CRGB leds[ledCnt]);
void LedPlayFMRange(CRGB leds[ledCnt]);
void LedPlayFMRotation(CRGB leds[ledCnt]);	//Not implemented
void LedPlayMFRGRB(int FreqVals[7], bool inv, CRGB leds[ledCnt]);  //Mode 1-1-1-1/2
void LedPlayRangeFin(long rainbow[7], int FreqVals[7], CRGB leds[ledCnt]);		//Rainbow
void LedPlayRangeFin(long color, int FreqVals[7], CRGB leds[ledCnt]);		//Single
void LedPlayRangeFin(CRGB leds[ledCnt]);		//Dynamic
//unsigned int CalcColors(long color, int value, CRGB leds[ledCnt]);	//Определение нового цвета RGB для индикации амплитуды
void LedMusic(int hsvColors[ledCnt][3], CRGB leds[ledCnt]);	//Зажечь светодиоды указанными HSV-цветами
*/


int* GetFreqVals(int *FreqVals);	//Получение значений амплитуд частот с MSGEQ7
void RGB2HSV(long rgb, int* h, int* s, int* v);		//Перевод из модели RGB в HSV
float Max(float a, float b);
float Min(float a, float b);
uint8_t GetHueFromCRGB(CRGB* crgb, int index);
uint8_t GetSatFromCRGB(CRGB* crgb, int index);
uint8_t GetValFromCRGB(CRGB* crgb, int index);

//UI
//void Welcome();
//void CommandShell();	//Запуск коммандного интерпретатора
//int GetCommandHandler(String command);	//Список команд
//void CommandHelp();		//Команда help
//void CommandStatus();	//Команда status
//int MCConfigure();		//Интерактивный конфигуратор
//void MCConfigure(long params[cParamCnt]);

#endif /* CMLOCALLIB_H_ */
