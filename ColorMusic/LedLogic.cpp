/*
 * LedLogic.cpp
 *
 *  Created on: 8 июн. 2017 г.
 *      Author: Nixiris
 */
#include "LedLogic.h"
#include "CMLocalLib.h"
bool colorsSet = false;
char G_LEDCOLORS[7][3];

//Запуск светомузыки
void LedLogic::LedPlay(CRGB leds[ledCnt])
{
	//DebugMsg("LedPlay");
	//Implementation
	switch (realParams)
	{
	case 0x00:
		LedOut::LedOff();
		break;
	case 0x25:
		//DebugMsg("OK?");
		//LedPlayFMRange(2);
		LedPlayFMRandColor();
		break;
	case 0x29:
		break;
	case 0x16:
		LedPlayAMLTH();
		break;
	case 0x1A:
		LedPlayAMFC();
		break;
	case 0x55:
		LedPlayFMRange(1, 0);
		break;
	case 0x95:
		LedPlayFMRange(1, 1);
		break;
	case 0xB5:
		LedPlayFMRange(3, 0);
		break;
	case 0x59:
		break;
	case 0x99:
		break;
	case 0xA6:
		break;
	case 0xAA:
		break;
	}
}

void LedLogic::LedPlayFreqMode(CRGB leds[ledCnt])
{
	//Implementation
	/*switch(realParams[1])
	{
	case 1:		//Range
		LedPlayFMRange(leds);
		break;
	case 2:		//Rotation
		LedPlayFMRotation(leds);
		break;
	}*/
}

void LedLogic::LedPlayAmpMode(CRGB leds[ledCnt])
{
	//Implementation
	//TODO
}

void LedLogic::LedPlayFMRange(CRGB leds[ledCnt])
{
	//Implementation
	/*switch(realParams[2])
	{
	case 1:		//Rainbow
		//TODO:
		//1) Получить сигналы (амплитуды) с MSGEQ7 в виде int FreqVals[7]
		int FreqVals[7];
		GetFreqVals(FreqVals);
		//2) Вызвать LedPlayMFRGRB(int FreqVals[7], bool inv),
		//   где bool inv - признак инвертации позиций цветов из params[3]
		if(realParams[3]==1)
		{
			LedPlayMFRGRB(FreqVals, false, leds);
		}
		else
		{
			LedPlayMFRGRB(FreqVals, true, leds);
		}
		break;
	case 2:		//Random Color
		//TODO
		break;
	case 3:		//Single Color
		//TODO
		break;
	}*/
}

void LedLogic::LedPlayFMRotation(CRGB leds[ledCnt])
{
	//TODO: Implementation
}

void LedLogic::LedPlayMFRGRB(int FreqVals[7], bool inv, CRGB leds[ledCnt])  //Mode 1-1-1-1/2
{
	/*
	 Прямое расположение цветов:
	 	 от низких частот к высоким:

	 	---------------------------------------------------------------------------------------
	 	| Freq |  63 Hz   |  160 Hz  |  400 Hz  |  1 kHz   |  2.5 kHz  |  6.25 kHz  |  16 kHz  |
	 	---------------------------------------------------------------------------------------
	 	| Color|   Red    |  Orange  |  Yellow  |  Green   | Lite Blue |    Blue    |  Violet  |
	 	---------------------------------------------------------------------------------------
	 	|  HEX | 0xFF0000 | 0xFFA500 | 0xFFFF00 | 0x00FF00 | 0x87CEBB  |  0x0000FF  | 0xEE82EE |
        ---------------------------------------------------------------------------------------

	 Обратное расположение цветов:

	 	---------------------------------------------------------------------------------------
	 	| Freq |  63 Hz   |  160 Hz  |  400 Hz  |  1 kHz   |  2.5 kHz  |  6.25 kHz  |  16 kHz  |
	 	---------------------------------------------------------------------------------------
	 	| Color|  Violet  |  Blue    | Lite Blue|  Green   |  Yellow   |   Orange   |    Red   |
	 	---------------------------------------------------------------------------------------
	 	|  HEX | 0xFF0000 | 0xFFA500 | 0xFFFF00 | 0x00FF00 | 0x87CEBB  |  0x0000FF  | 0xEE82EE |
        ---------------------------------------------------------------------------------------
	 */
	long Rainbow[7];
	if(inv)	//Обратное расположение цветов
	{
		Rainbow[0] = cColorViolet;
		Rainbow[1] = cColorBlue;
		Rainbow[2] = cColorLtBlue;
		Rainbow[3] = cColorGreen;
		Rainbow[4] = cColorYellow;
		Rainbow[5] = cColorOrange;
		Rainbow[6] = cColorRed;

	}
	else	//Прямое расположение цветов
	{
		Rainbow[0] = cColorRed;
		Rainbow[1] = cColorOrange;
		Rainbow[2] = cColorYellow;
		Rainbow[3] = cColorGreen;
		Rainbow[4] = cColorLtBlue;
		Rainbow[5] = cColorBlue;
		Rainbow[6] = cColorViolet;
	}
	/*DebugMsg("Rainbows:");
				for(int i=0;i<7;i++)
				{
					Serial.println(Rainbow[i], HEX);
				}
				*/
	LedPlayRangeFin(Rainbow, FreqVals, leds);
}

void LedLogic::LedPlayRangeFin(long rainbow[7], int FreqVals[7], CRGB leds[ledCnt])
{
	/*//TODO:
	//Implementation
	//unsigned int newColors[7];
	int ledColors[ledCnt][3];		//цвета светодиодов по модели HSV
	int	newRainbow[7][3];			//цвета радуги по модели HSV


	for(int color=0; color<7; color++)		//определение новых цветов с измененной яркостью
	{
		RGB2HSV(rainbow[color],newRainbow[color][0],newRainbow[color][1],newRainbow[color][2]);
		newRainbow[color][0] = FreqVals[color];
	}

	int ledGroupCnt = ledCnt/7; //Определяем, по сколько LED в каждой группе (полосе частот). В самой низкой полосе будет +остаток, т.е. >= всех остальных групп
	for(int Group = 1; Group < 7; Group++)						//Для всех групп, за исключением самой высокой, определяем значение яркости
	{
		for(int ledShift = 0; ledShift<ledGroupCnt; ledShift++)
		{
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][0] = newRainbow[Group][0];
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][1] = newRainbow[Group][1];
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][2] = newRainbow[Group][2];

			leds[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)].setColorCode(rainbow[Group]);

			CHSV chsvColor = rgb2hsv_approximate(leds[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)]);

			chsvColor.v = (float)FreqVals[Group] / 255 * 100;
			hsv2rgb_rainbow(chsvColor,leds[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)]);
		}
	}
	for(int ledShift = 0; ledShift<ledGroupCnt+(ledCnt-ledGroupCnt*7); ledShift++)
	{
		ledColors[ledShift+(ledCnt-ledGroupCnt*7)][0] = newRainbow[0][0];
		ledColors[ledShift+(ledCnt-ledGroupCnt*7)][1] = newRainbow[0][1];
		ledColors[ledShift+(ledCnt-ledGroupCnt*7)][2] = newRainbow[0][2];

		leds[ledShift].setColorCode(rainbow[0]);
		CHSV chsvColor = rgb2hsv_approximate(leds[ledShift]);
		chsvColor.v = (float)FreqVals[0] / 255 * 100;
		hsv2rgb_rainbow(chsvColor,leds[ledShift]);
	}
	//DebugMsg("DEBUG::");
	//FastLED.show();

	//Передача цветов
	LedMusic(ledColors, leds);*/
}

//TODO:
char LedLogic::LedPlayFMRange(char mode)
{
	void LedPlayFMRandColor();
	return 0;
}

//TODO:
char LedLogic::LedPlayFMRange(char mode, unsigned int opt)
{
	switch(mode)
	{
	case 1:
		if(opt==0)
		{
			LedPlayFMRB(false);
		}
		else
		{
			LedPlayFMRB(true);
		}
		break;
	case 2:
		LedPlayFMRandColor();
		break;
	case 3:
		switch(opt)
		{
		case 0:			//Freq Range Dynamic
			break;
		default:		//Spec Color
			LedPlayFMRgColor(opt);
			break;
		}
		break;
	}
	return 0;
}

void LedLogic::LedPlayFMRB(bool inv)
{
	int ledColors[ledCnt][3];		//цвета светодиодов по модели HSV
	for(int i = 0; i<ledCnt; i++)
		for(int j = 0; j<3; j++)
			ledColors[i][j]=0;
	int	Rainbow[7][3];			//цвета радуги по модели HSV
	if(inv)
	{
		Rainbow[0][0] = 0;
		Rainbow[0][1] = 255;
		Rainbow[0][2] = 255;

		Rainbow[1][0] = 28;
		Rainbow[1][1] = 255;
		Rainbow[1][2] = 255;

		Rainbow[2][0] = 43;
		Rainbow[2][1] = 210;
		Rainbow[2][2] = 255;

		Rainbow[3][0] = 85;
		Rainbow[3][1] = 255;
		Rainbow[3][2] = 215;

		Rainbow[4][0] = 130;
		Rainbow[4][1] = 200;
		Rainbow[4][2] = 230;

		Rainbow[5][0] = 170;
		Rainbow[5][1] = 255;
		Rainbow[5][2] = 255;

		Rainbow[6][0] = 210;
		Rainbow[6][1] = 200;
		Rainbow[6][2] = 230;
	}
	else
	{
		Rainbow[0][0] = 210;
		Rainbow[0][1] = 200;
		Rainbow[0][2] = 230;

		Rainbow[1][0] = 170;
		Rainbow[1][1] = 255;
		Rainbow[1][2] = 255;

		Rainbow[2][0] = 130;
		Rainbow[2][1] = 200;
		Rainbow[2][2] = 230;

		Rainbow[3][0] = 85;
		Rainbow[3][1] = 255;
		Rainbow[3][2] = 255;

		Rainbow[4][0] = 43;
		Rainbow[4][1] = 210;
		Rainbow[4][2] = 255;

		Rainbow[5][0] = 28;
		Rainbow[5][1] = 255;
		Rainbow[5][2] = 255;

		Rainbow[6][0] = 0;
		Rainbow[6][1] = 255;
		Rainbow[6][2] = 255;
	}
	int FreqVals[7] = {0,0,0,0,0,0,0};
	*FreqVals = SpectrumAnalizer::GetFreqVals(FreqVals);
	for(int color=0; color<7; color++)		//определение новых цветов с измененной яркостью
	{
		//Serial.println(FreqVals[color]);
		Rainbow[color][2] = (float)FreqVals[color]/255*Rainbow[color][2];
	}
	//Serial.println("****");
/*	for(int i = 0; i<30; i++)
	{
		for(int j=0;j<3;j++)
		{
			Serial.print(ledColors[i][j]);
		}
		Serial.println("****");
	}*/
	int ledGroupCnt = ledCnt/7; //Определяем, по сколько LED в каждой группе (полосе частот). В самой низкой полосе будет +остаток, т.е. >= всех остальных групп
	for(int Group = 1; Group < 7; Group++)						//Для всех групп, за исключением самой высокой, определяем значение яркости
	{
		for(int ledShift = 0; ledShift<ledGroupCnt; ledShift++)
		{
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][0] = Rainbow[Group][0];
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][1] = Rainbow[Group][1];
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][2] = Rainbow[Group][2];
		}
		/*Serial.print(Rainbow[Group][0]);
		Serial.print("+");
		Serial.print(Rainbow[Group][1]);
		Serial.print("+");
		Serial.println(Rainbow[Group][2]);*/
	}
	//Serial.println("----");
	for(int ledShift = 0; ledShift<ledGroupCnt+(ledCnt-ledGroupCnt*7); ledShift++)
	{
		ledColors[ledShift][0] = Rainbow[0][0];
		ledColors[ledShift][1] = Rainbow[0][1];
		ledColors[ledShift][2] = Rainbow[0][2];
		/*Serial.print(Rainbow[0][0]);
		Serial.print("+");
		Serial.print(Rainbow[0][1]);
		Serial.print("+");
		Serial.println(Rainbow[0][2]);*/
	}
	//Serial.println("****");
	/*for(int led = 0; led < ledCnt; led++)
	{
		int h = ledColors[led][0];
		int s = ledColors[led][1];
		int v = ledColors[led][2];
		Serial.print(h);
		Serial.print("+");
		Serial.print(s);
		Serial.print("+");
		Serial.println(v);
	}
	Serial.println("****");*/
	LedOut::LedMusic(ledColors);
}

void LedOut::LedMusic(int hsvColors[ledCnt][3])
{
	for(int led = 0; led < ledCnt; led++)
	{

		leds[led].setHSV(hsvColors[led][0], hsvColors[led][1], hsvColors[led][2]);
		/*char R = ((leds[led].r));
		char G = ((leds[led].g));
		char B = ((leds[led].b));
		long RGB = R+G+B;
		Serial.println(RGB);*/
		//int h,s,v;
		//RGB2HSV(RGB,h,s,v);
/*		Serial.println(+R);
		Serial.println(+G);
		Serial.println(+B);*/
		/*Serial.print(h);
		Serial.print("+");
		Serial.print(s);
		Serial.print("+");
		Serial.println(v);*/
	}
	FastLED.show();
}

void LedLogic::LedPlayFMRgDyn()
{
	int *ledColors[ledCnt][3];		//цвета светодиодов по модели HSV
	int *FreqVals = SpectrumAnalizer::GetFreqVals(FreqVals);
	int ledColor[3] = {0, 0, 0};	//TODO:
									//Определить и установить цвет исходя от пройденного времени использования предыдущего цвета
	int ledGroupCnt = ledCnt/7; //Определяем, по сколько LED в каждой группе (полосе частот). В самой низкой полосе будет +остаток, т.е. >= всех остальных групп
	for(int Group = 1; Group < 7; Group++)						//Для всех групп, за исключением самой высокой, определяем значение яркости
	{
		for(int ledShift = 0; ledShift<ledGroupCnt; ledShift++)
		{
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][0] = FreqVals[Group];
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][1] = ledColor[1];
			ledColors[Group*ledGroupCnt+ledShift+(ledCnt-ledGroupCnt*7)][2] = ledColor[2];
		}
	}
	for(int ledShift = 0; ledShift<ledGroupCnt+(ledCnt-ledGroupCnt*7); ledShift++)
	{
		ledColors[ledShift+(ledCnt-ledGroupCnt*7)][0] = FreqVals[0];
		ledColors[ledShift+(ledCnt-ledGroupCnt*7)][1] = ledColor[1];
		ledColors[ledShift+(ledCnt-ledGroupCnt*7)][2] = ledColor[2];
	}

	//LedMusic(ledColors, ledCnt);
}

void LedOut::LedOff()
{
	for (char led = 0; led < ledCnt; led++)
	{
		leds[led].setColorCode(0x00);
	}
	FastLED.show();
}

void LedLogic::LedPlayAMLTH()
{
	int ledColors[ledCnt][3];
	char *rGroup, *yGroup, *gGroup;
	char gGroupCnt = ledCnt * 0.25;
	char yGroupCnt = ledCnt * 0.6;
	char rGroupCnt = ledCnt - (gGroupCnt + yGroupCnt);
	char Cg = 35 / gGroupCnt;
	char Cy = 21 / yGroupCnt;
	char Cr = (float)40/360*255 / rGroupCnt;
	int FreqVals[7] = { 0,0,0,0,0,0,0 };
	*FreqVals = SpectrumAnalizer::GetFreqVals(FreqVals);
	int FreqValsMax = 0;
	for (char val = 0; val < 7; val++)
	{
		if (FreqValsMax < FreqVals[val])
		{
			FreqValsMax = FreqVals[val];
		}
	}
	//Serial.println(FreqValsMax);
	
	if (FreqValsMax > 0)
	{
		//Serial.println(FreqValsMax);
		FreqValsMax = (float)FreqValsMax / 255 * ledCnt;
		//Serial.println(FreqValsMax);
		//Serial.println("****");
	}
	//Serial.print("VAL: ");
	//Serial.println(FreqValsMax);
	//Serial.println("GREEN:");
	for (char led = 0; led < gGroupCnt; led++)
	{
		ledColors[led][0] = 85 - Cg * led;
		ledColors[led][1] = 255;
		ledColors[led][2] = 255;
		//Serial.println(ledColors[led][0]);
	}
	//Serial.println("YELLOW:");
	for (char led = gGroupCnt; led < gGroupCnt+yGroupCnt; led++)
	{
		ledColors[led][0] = 50 - Cy * (led- gGroupCnt);
		ledColors[led][1] = 255;
		ledColors[led][2] = 255;
		//Serial.println(ledColors[led][0]);
	}
	//Serial.println("RED:");
	for (char led = gGroupCnt+yGroupCnt; led < ledCnt; led++)
	{
		ledColors[led][0] = 28 - Cr * (led-(gGroupCnt + yGroupCnt)+1);
		ledColors[led][1] = 255;
		ledColors[led][2] = 255;
		//Serial.println(ledColors[led][0]);
	}

	for (char led = 0; led < ledCnt; led++)
	{
		if (led > FreqValsMax-1)
		{
			ledColors[led][2] = 0;
		}
	}
	//Serial.println(FreqValsMax);
	LedOut::LedMusic(ledColors);
}

void  LedLogic::LedPlayAMFC()
{
	DebugMsg("0x1A");
	int ledColors[ledCnt][3];
	//char *rGroup, *yGroup, *gGroup;
	char ledCnt2 = ledCnt / 2;
	char ledShift;
	if ((ledCnt2 * 2) != ledCnt)
	{
		ledShift = 1;
	}
	else
	{
		ledShift = 0;
	}
	char gGroupCnt = ledCnt2 * 0.25;
	char yGroupCnt = ledCnt2 * 0.6;
	char rGroupCnt = ledCnt2 - (gGroupCnt + yGroupCnt);
	char Cg = 35 / gGroupCnt;
	char Cy = 21 / yGroupCnt;
	char Cr = (float)40 / 360 * 255 / rGroupCnt;
	int FreqVals[7] = { 0,0,0,0,0,0,0 };
	*FreqVals = SpectrumAnalizer::GetFreqVals(FreqVals);
	int FreqValsMax = 0;
	for (char val = 0; val < 7; val++)
	{
		if (FreqValsMax < FreqVals[val])
		{
			FreqValsMax = FreqVals[val];
		}
	}
	////Serial.println(FreqValsMax);

	if (FreqValsMax > 0)
	{
	//	Serial.println(FreqValsMax);
		FreqValsMax = (float)FreqValsMax / 255 * ledCnt2;
		//Serial.println(FreqValsMax);
		//Serial.println("****");
	}
	////Serial.print("VAL: ");
	////Serial.println(FreqValsMax);
	Serial.println("GREEN:");
	if (ledShift)
	{
		ledColors[ledCnt2][0] = 85;
		ledColors[ledCnt2][1] = 255;
		ledColors[ledCnt2][2] = 255;
	}
	for (char led = 0; led < gGroupCnt; led++)
	{
		ledColors[ledCnt2 + ledShift + led][0] = 85 - Cg * led;
		ledColors[ledCnt2 + ledShift + led][1] = 255;
		ledColors[ledCnt2 + ledShift + led][2] = 255;
		ledColors[ledCnt2 - 1 - led][0] = 85 - Cg * led;
		ledColors[ledCnt2 - 1 - led][1] = 255;
		ledColors[ledCnt2 - 1 - led][2] = 255;
		Serial.print(ledCnt2 + ledShift + led);
		Serial.println(ledCnt2 - 1 - led);
		//Serial.println(ledColors[led][0]);
	}
	//Serial.println("YELLOW:");
	for (char led = gGroupCnt; led < gGroupCnt + yGroupCnt; led++)
	{
		ledColors[ledCnt2 + ledShift + led][0] = 50 - Cy * (led - gGroupCnt);
		ledColors[ledCnt2 + ledShift + led][1] = 255;
		ledColors[ledCnt2 + ledShift + led][2] = 255;
		ledColors[ledCnt2 - 1 - led][0] = 50 - Cy * (led - gGroupCnt);
		ledColors[ledCnt2 - 1 - led][1] = 255;
		ledColors[ledCnt2 - 1 - led][2] = 255;
		//Serial.print(ledCnt2 + ledShift + led);
		//Serial.println(ledCnt2 - 1 - led);
		//Serial.println(ledColors[led][0]);
	}
	//Serial.println("RED:");
	for (char led = gGroupCnt + yGroupCnt; led < ledCnt2; led++)
	{
		ledColors[ledCnt2 + ledShift + led][0] = 28 - Cr * (led - (gGroupCnt + yGroupCnt) + 1);
		ledColors[ledCnt2 + ledShift + led][1] = 255;
		ledColors[ledCnt2 + ledShift + led][2] = 255;
		ledColors[ledCnt2 - 1 - led][0] = 28 - Cr * (led - (gGroupCnt + yGroupCnt) + 1);
		ledColors[ledCnt2 - 1 - led][1] = 255;
		ledColors[ledCnt2 - 1 - led][2] = 255;
		//Serial.print(ledCnt2 + ledShift + led);
		//Serial.println(ledCnt2 - 1 - led);
		//Serial.println(ledColors[led][0]);
	}

	if (ledShift)
	{
		if (FreqValsMax == 0)
		{
			ledColors[ledCnt2][2] = 0;
		}
	}

	for (char led = 0; led < ledCnt2; led++)
	{
		if (led > FreqValsMax - 1)
		{
			ledColors[ledCnt2 + led + ledShift][2] = 0;
			ledColors[ledCnt2 - 1 - led][2] = 0;
		}
	}
	////Serial.println(FreqValsMax);
	LedOut::LedMusic(ledColors);
}

void LedLogic::LedPlayFMRandColor()
{
	DebugMsg("Random:");
	//int ledColors[ledCnt][3];		//цвета светодиодов по модели HSV
	//for (int i = 0; i<ledCnt; i++)
	//	for (int j = 0; j<3; j++)
	//		ledColors[i][j] = 0;
	CHSV chsvColors[7];
	int ledColors[ledCnt][3];
	for (char i = 0; i < ledCnt; i++)
		for (char j = 0; j < 3; j++)
			ledColors[i][j] = 0;
	int ledGroupCnt = ledCnt / 7; //Определяем, по сколько LED в каждой группе (полосе частот). В самой низкой полосе будет +остаток, т.е. >= всех остальных групп
	for (char led = 0; led < ledCnt; led++)
	{
		chsvColors[led] = CHSV(0,0,0);
	}
	//char color;
	//bool colorsSet = false;
	//unsigned char ColorsHV[7][2];			//цвета радуги по модели HSV
	//ColorsHV[0][0] = random(1, 255);
	//for (char led = 0; led < ledCnt; led++)		//S = 255 всегда
	//{
	//	ledColors[led][1] = 255;
	//}
	for (char color = 0; color < 7; color++)
	{
		chsvColors[color].h = 0;
		chsvColors[color].s = 255;
	}
	

	if (colorsSet)
	{
		for (char led = 0; led < ledCnt; led++)
		{
			ledColors[led][0] = rgb2hsv_approximate(leds[led]).h;
			//ledColors[led][2] = rgb2hsv_approximate(leds[led]).v;
		}
	}
	else
	{
		
		//DebugMsg("DBG");
		//Serial.println(ColorsHV[0][0],DEC);
		chsvColors[0].h = random(1, 255);
		//Serial.println(chsvColors[0].h);
		while (!colorsSet)
		{
			//DebugMsg("DBG");
			colorsSet = true;
			for (char pos = 1; pos < 7; pos++)
			{
				//Serial.print("POS: ");
				//Serial.println(pos, DEC);
				//Serial.println(chsvColors[pos].h);
				//Serial.println(colorsSet);
				colorsSet = true;
				//if (abs(chsvColors[pos].h - chsvColors[pos - 1].h) < 20)
				{//Serial.println(ledColors[0][0]); 
				//Serial.println(chsvColors[pos].h);
					//	//Serial.print("POS: ");
					//	//Serial.println(pos, DEC);
					//	Serial.println(chsvColors[pos].h);
					//	Serial.println(chsvColors[pos-1].h);
					//Serial.print("****");
					//Serial.println(abs(chsvColors[pos].h - chsvColors[pos - 1].h));
					chsvColors[pos].h = random(1, 255);
					//Serial.println(chsvColors[pos].h);
					colorsSet = false;
				}/*
				if (pos == 6)
				{
					if (abs(chsvColors[pos].h - chsvColors[0].h) < 20)
					{
						chsvColors[pos].h = random(1, 255);
						colorsSet = false;
					}
				}*/
			}
		}
		DebugMsg("EXIT");
		/*
		for (char Group = 1; Group < 7; Group++)						//Для всех групп, за исключением самой высокой, определяем значение яркости
		{
			for (char ledShift = 0; ledShift<ledGroupCnt; ledShift++)
			{
				ledColors[Group*ledGroupCnt + ledShift + (ledCnt - ledGroupCnt * 7)][0] = chsvColors[Group].h;

			}

		}
		//Serial.println("----");
		for (int ledShift = 0; ledShift<ledGroupCnt + (ledCnt - ledGroupCnt * 7); ledShift++)
		{
			ledColors[ledShift][0] = chsvColors[0].h;

		}
		*/

	}
	/*
	
	int FreqVals[7] = { 0,0,0,0,0,0,0 };
	*FreqVals = GetFreqVals(FreqVals);
	
	for (char led = 0; led < ledCnt; led++)
	{
		Serial.print("LED: ");
		Serial.println(led, DEC);
		//Serial.println(chsvColors[led].h);
		//Serial.println(ledColors[led][0]);
		//Serial.println(ledColors[led][1]);
		//Serial.println(ledColors[led][2]);
	}
	
	
	for (char Group = 1; Group < 7; Group++)						//Для всех групп, за исключением самой высокой, определяем значение яркости
	{
		for (char ledShift = 0; ledShift<ledGroupCnt; ledShift++)
		{

			ledColors[Group*ledGroupCnt + ledShift + (ledCnt - ledGroupCnt * 7)][1] = 255;
			ledColors[Group*ledGroupCnt + ledShift + (ledCnt - ledGroupCnt * 7)][2] = chsvColors[Group].v;
		}

	}

	for (char ledShift = 0; ledShift<ledGroupCnt + (ledCnt - ledGroupCnt * 7); ledShift++)
	{
		ledColors[ledShift][1] = 255;
		ledColors[ledShift][2] = chsvColors[0].v;

	}

	for (char led = 0; led < ledCnt; led++)
	{
		Serial.println(ledColors[led][0]);
		Serial.println(ledColors[led][1]);
		Serial.println(ledColors[led][2]);
	}
	Serial.println("****");
	*/
	//LedMusic(ledColors);
}