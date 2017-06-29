/*
 * CMLocalLib.cpp
 *
 *  Created on: 27 мая 2017 г.
 *      Author: Nixiris
 */
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "CMLocalLib.h"
#include <FastLED.h>

/*bool flagModeL1SelectPrinted = false;
bool flagModeL2SelectPrinted = false;
bool flagModeL3SelectPrinted = false;
bool flagModeL4SelectPrinted = false;
bool flagModeL5SelectPrinted = false;*/

float msg7OutMultiplier = 3; //коэффициент усиления вывода MSGEQ7

void DebugMsg(String message)
{
	Serial.setTimeout(0);
	Serial.println("DEBUG:");
	Serial.println(message);
}

//Вывод конфигурации на сериал
void ShowParams()
{
	btSerial.print("Current config: ");
	btSerial.println(configParams);
}

/*//Запуск светомузыки
void LedPlay(CRGB leds[ledCnt])
{
	DebugMsg("LedPlay");
	//Implementation
	switch(realParams[0])
	{
	case 1:		//При частотном режиме
		LedPlayFreqMode(leds);
		break;
	case 2:		//При общем режиме
		LedPlayAmpMode(leds);
		break;
	}
}

void LedPlayFreqMode(CRGB leds[ledCnt])
{
	//Implementation
	switch(realParams[1])
	{
	case 1:		//Range
		LedPlayFMRange(leds);
		break;
	case 2:		//Rotation
		LedPlayFMRotation(leds);
		break;
	}
}

void LedPlayAmpMode(CRGB leds[ledCnt])
{
	//Implementation
	//TODO
}

void LedPlayFMRange(CRGB leds[ledCnt])
{
	//Implementation
	switch(realParams[2])
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
	}
}

void LedPlayFMRotation(CRGB leds[ledCnt])
{
	//TODO: Implementation
}*/

//Получение значений амплитуд частот с MSGEQ7
int* SpectrumAnalizer::GetFreqVals(int *FreqVals)
{
	 digitalWrite(msg7RESET, HIGH);   //сброс MSGEQ7
	    delay(5);
	    digitalWrite(msg7RESET, LOW);

	    for (int bound = 0; bound < 7; bound++){             // семь частот - семь STROBE-импульсов
	        digitalWrite(msg7Strobe, LOW);
	        delayMicroseconds(35);           //задержка для установления значения
	        int amp = analogRead(msg7DCout);      //прием звукового сигнала
	        int PWMvalue = map(amp, 0, 1023, 0, 255);  //преобразование диапазона полученных значений 0-1023 к диапазону 0-255 для ШИМ
			if (PWMvalue < 5)       //программный фильтр шума
			{
				PWMvalue = 0;
			}
			FreqVals[bound] = PWMvalue*msg7OutMultiplier;
	        digitalWrite(msg7Strobe, HIGH);
	    }
	return *FreqVals;
}

/*void LedPlayMFRGRB(int FreqVals[7], bool inv, CRGB leds[ledCnt])  //Mode 1-1-1-1/2
{

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
	DebugMsg("Rainbows:");
				for(int i=0;i<7;i++)
				{
					Serial.println(Rainbow[i], HEX);
				}

	LedPlayRangeFin(Rainbow, FreqVals, leds);
}

void LedPlayRangeFin(long rainbow[7], int FreqVals[7], CRGB leds[ledCnt])
{
	//TODO:
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
	LedMusic(ledColors, leds);
}*/

/*int GetCommandHandler(String command)
{
	if(command=="help") return 1;
	if(command=="status") return 2;
	if(command=="configui") return 3;
	return 0;
}*/

/*void ParamsJustInited()
{
	//digitalWrite(ledPin,HIGH);
	for(int i=0; i<cParamCnt; i++)
	{
		realParams[i] = preParams[i];
		preParams[i] = 0;
	}
	Serial.println("Configuration has been set.");
	ShowParams();
}*/

/*
 *
 void CommandShell()
{
	Serial.setTimeout(200);
	if(Serial.available())
			{
				String strIn = Serial.readString();
				switch(GetCommandHandler(strIn))
				{
				case 1:
					CommandHelp();		//"help"
					break;
				case 2:
					CommandStatus();	//"status"
					break;
				case 3:
					MCConfigure();		//"configure"
					break;
				default:
					Serial.print("Can't recognise command \"");
					Serial.print(strIn);
					Serial.println("\". Please, repeat. Try \"help\" for command list.");
					break;
				}

			}
}

void CommandHelp()
{
	Serial.println("List of available commands:");
						Serial.println("help - shows this text");
						Serial.println("status - shows current configuration");
						Serial.println("configui - runs configuring dialoge");
						Serial.println();
						Serial.println("Nixiris' MusicColor project 2017");
}

void CommandStatus()
{
	Serial.print("Your current configuration: ");
						for(int i=0; i<5; i++)
						{
							Serial.print(realParams[i]);
						}
						Serial.println();
}

int MCConfigure()		//Интерактивный конфигуратор
{
	long serInputLong = 0;
	flagParamsInited = false;
	if(preParams[0]==0)
			{
				//Первый уровень
				if(!flagModeL1SelectPrinted)	//Выберите режим.
				{
					Serial.println("Please, choose mode:");
					Serial.println("1) Freq");
					Serial.println("2) Amp");
					flagModeL1SelectPrinted = true;
				}
				Serial.setTimeout(0);
				if(Serial.available()){
					//Serial.println("DEBUG: 11");
					serInputLong = Serial.parseInt();
					//Serial.println(serInputLong);
					preParams[0] = serInputLong;
				}
				//else DebugMsg("WTF");
			}
			else if(preParams[1]==0)
			{
				//Второй уровень
				//Serial.println("DEBUG: 2");
				if(!flagModeL2SelectPrinted)
				{
					flagModeL1SelectPrinted = false;

					//Serial.println("DEBUG: 21");
					switch(preParams[0])
					{
						case 1:
							Serial.println("FreqMode. Please, specify:");
							Serial.println("1) Range");
							Serial.println("2) Rotation");
							flagModeL2SelectPrinted = true;
							break;
						case 2:
							Serial.println("AmpMode. Please, specify:");
							Serial.println("1) Simple Low to High");
							Serial.println("2) From center");
							flagModeL2SelectPrinted = true;
							break;
					}

				}
				//Serial.println("DEBUG!!!");
				Serial.setTimeout(0);
				if(Serial.available())
				{
					//Serial.println("DEBUG 23");
					serInputLong = Serial.parseInt();
					preParams[1] = serInputLong;
				}

			}
			else if(preParams[2]==0)
			{
				//Serial.println("DEBUG: 3");
				//Третий
				if(!flagModeL3SelectPrinted)
				{
					//Serial.println("DEBUG: 20");

					flagModeL2SelectPrinted = false;
					switch(preParams[1])
					{
						case 1:
							switch(preParams[0])
							{
							case 1:
								Serial.println("Please, specify:");
								Serial.println("1) Rainbow");
								Serial.println("2) Random color");
								Serial.println("3) Single color");
								break;
							case 2:
								Serial.println("Please, specify:");
								Serial.println("1) RGB");
								Serial.println("2) Single color");
								break;
							}
							flagModeL3SelectPrinted = true;
							break;
						case 2:
							switch(preParams[0])
							{
							case 1:
								Serial.println("Please, specify:");
								Serial.println("1) Rainbow");
								Serial.println("2) Random color");
								Serial.println("3) Single color");
								break;
							case 2:
								Serial.println("Please, specify:");
								Serial.println("1) RGB");
								Serial.println("2) Single color");
								break;
							}
							flagModeL3SelectPrinted = true;
							break;
					}
				}




				//Serial.println("DEBUG!!!");
				Serial.setTimeout(0);
				if(Serial.available())
				{
					//Serial.println("DEBUG 23");
					serInputLong = Serial.parseInt();
					preParams[2] = serInputLong;
				}
			}
			else if(preParams[3]==0)
			{
				//Serial.println("DEBUG: 4");
				//Четвертый
				if(!flagModeL4SelectPrinted)
				{
					flagModeL3SelectPrinted = false;
					switch(preParams[0])
					{
					case 1:
						switch(preParams[1])
						{
						case 1:
							switch(preParams[2])
							{
							case 1:
								Serial.println("Please, specify:");
								Serial.println("1) Straight");
								Serial.println("2) Reverse");
								break;
							case 2:
								//ВСЁ ЗАДАНО!!!!
								//DebugMsg("ParamsJustInited");
								ParamsJustInited();
								//Serial.println("Params initialised!");
								flagParamsInited = true;
								return 0;
								break;
							case 3:
								Serial.println("Please, specify:");
								Serial.println("1) Select color");
								Serial.println("2) Dynamic color");
								break;
							}
							break;
						case 2:
							switch(preParams[2])
							{
							case 1:
								Serial.println("Please, specify:");
								Serial.println("1) Straight");
								Serial.println("2) Reverse");
								break;
							case 2:
								//ВСЁ ЗАДАНО!!!!
								ParamsJustInited();
								//Serial.println("Params initialised!");
								flagParamsInited = true;
								return 0;
								break;
							case 3:
								Serial.println("Please, specify:");
								Serial.println("1) Select color");
								Serial.println("2) Dynamic color");
								break;
							}
							break;
						}
						flagModeL4SelectPrinted = true;
						break;
					case 2:
						switch(preParams[1])
						{
						case 1:
							switch(preParams[2])
							{
							case 1:
								//ВСЁ ЗАДАНО!!!
								ParamsJustInited();
								//Serial.println("Params initialised!");
								flagParamsInited = true;
								return 0;
								break;
							case 2:
								Serial.println("Please, specify:");
								Serial.println("1) Select color");
								Serial.println("2) Dynamic color");
								break;
							}
							break;
						case 2:
							switch(preParams[2])
							{
							case 1:
								//ВСЁ ЗАДАНО!!!
								Serial.println("Params initialised!");
								flagParamsInited = true;
								return 0;
								break;
							case 2:
								Serial.println("Please, specify:");
								Serial.println("1) Select color");
								Serial.println("2) Dynamic color");
								break;
							}
							break;
						}
						flagModeL4SelectPrinted = true;
						break;
					}
				}

				Serial.setTimeout(0);
				if(Serial.available())
				{
					serInputLong = Serial.parseInt();
					preParams[3] = serInputLong;
				}
			}
			else if(preParams[4]==0)
			{
				//Serial.println("DEBUG: 5");
				//Пятый
				if(preParams[3]==1 && ((preParams[2]==3 && preParams[0]==1) || (preParams[2]==2 && preParams[0]==2)))
				{
					//DebugMsg("Pick a color");
					if(!flagModeL5SelectPrinted)
					{
						flagModeL5SelectPrinted = false;
						DebugMsg("Message");
						Serial.println("Please, specify RGB color (000001-FFFFFF)");
						flagModeL5SelectPrinted = true;
					}
					if(Serial.available())
					{

						serInputLong = Serial.parseInt();
						preParams[4] = serInputLong;

					}
				}
				else
				{
					//ВСЁ ЗАДАНО!
					//DebugMsg("flagParamsInited = true");
					flagParamsInited = true;
					//DebugMsg("ParamsJustInited()");
					ParamsJustInited();
					//Serial.println("Params initialised!");
					return 0;
				}

			}
			else
				//ВСЁ ЗАДАНО!
			{
				flagModeL5SelectPrinted = false;
				flagParamsInited = true;
				ParamsJustInited();
				return 0;
				//Serial.println("Params initialised!");
				//ShowParams();
			}
	G_shellAvailable = false;	//не выходим в шелл
	return 1;
}

//Приветственное сообщение при загрузке ColorMusic
void Welcome()
{
	Serial.println("Welcome to ColorMusic! Powered by Arduino");
	Serial.println("We do respect your personality.");
	Serial.println(">>:");
}

void MCConfigure(long params[cParamCnt])
{
	for(int param=0; param<cParamCnt; param++)
	{
		realParams[param] = params[param];
	}
}
*/
void RGB2HSV(long rgb, int* h, int* s, int* v)
{
	float fh, fs, fv, fr, fg, fb, cr, cg, cb;
	fh = fs = fv = fr = fg = fb = cr = cg = cb = 0;

	int r = (RGB&0xFF0000)>>16;
	int g = (RGB&0x00FF00)>>8;
	int b = RGB & 0x0000FF;
	//onsole.WriteLine("RGB = {0:X}, R = {1:X}, G = {2:X}, B = {3:X} ", RGB, ir,ig,ib);

	if(r!=0)
	{
		fr = (float)r/255;
	}
	else
	{
		fr = 0;
	}

	if (g != 0)
	{
		fg = (float)g/255;
	}
	else
	{
		fg = 0;
	}

	if (b != 0)
	{
		fb = (float)b/255;
	}
	else
	{
		fb = 0;
	}

	fv = Max(fr, Max(fg, fb));
	float temp = Min(fr, Min(fg, fb));
	if(fv==0)
	{
		fs = 0;
	}
	else
	{
		fs = (fv - temp) / fv;
	}
	if(fs==0)
	{
		fh = 0;		//неопределенный
	}
	else
	{
		cr = (fv - fr) / (fv - temp);
		cg = (fv - fg) / (fv - temp);
		cb = (fv - fb) / (fv - temp);
		if(fr==fv)
		{
			fh = cb - cg;
		}
		if(fg==fv)
		{
			fh = 2 + cr - cb;
		}
		if(fb==fv)
		{
			fh = 4 + cg - cr;
		}
		fh = fh * 60;
		if(fh<0)
		{
			fh += 360;
		}
	}

	h = (int)fh;
	s = (int)(fs*255);
	v = (int)(fv*255);
}

float Max(float a, float b)
{
	return a>b?a:b;
}

float Min(float a, float b)
{
	return a<b?a:b;
}

//Зажигание ленты
/*void LedMusic(int hsvColors[ledCnt][3], CRGB leds[ledCnt])
{
	for(int led=0; led<ledCnt; led++)
	{
		leds[led].setHSV(hsvColors[0],hsvColors[1],hsvColors[2]);
	}
	FastLED.show();
}*/

uint8_t GetHueFromCRGB(CRGB* crgb, int index)
{
	return rgb2hsv_approximate(crgb[index]).h;
}
uint8_t GetSatFromCRGB(CRGB* crgb, int index)
{
	return rgb2hsv_approximate(crgb[index]).s;
}
uint8_t GetValFromCRGB(CRGB* crgb, int index)
{
	return rgb2hsv_approximate(crgb[index]).v;
}