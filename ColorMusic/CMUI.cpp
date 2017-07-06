/*
 * CMUI.cpp
 *
 *  Created on: 2 июн. 2017 г.
 *      Author: Nixiris
 */
#include "CMUI.h"
#include "CMLocalLib.h"


//bool G_shellAvailable = true;

bool CMUI::shellAvailable = true;

uint8_t UserButtonLastState = LOW;
bool flagModeL1SelectPrinted = false;
bool flagModeL2SelectPrinted = false;
bool flagModeL3SelectPrinted = false;
bool flagModeL4SelectPrinted = false;
bool flagModeL5SelectPrinted = false;

unsigned int preParam = 0;

//Приветственное сообщение при загрузке ColorMusic
void CMUI::Welcome()
{
	btSerial.println("Welcome to ColorMusic! Powered by Arduino");
	btSerial.println("We do respect your personality.");
	btSerial.println(">>:");
}

//Хэндлер команд CommandShell
int CMUI::GetCommandHandler(String command)
{
	if(command=="help") return 1;
	if(command=="status") return 2;
	if(command=="configui") return 3;
	if (command.substring(0, command.indexOf(" ")) == "config") return 4;
	return 0;
}

//Командный интерпретатор
void CMUI::CommandShell()
{
	if (!shellAvailable)	//если вывод занят другой командой, то шелл не запускается
	{
		return;
	}
	btSerial.setTimeout(50);
	if(btSerial.available())
			{
				String strIn = btSerial.readString();
				switch(GetCommandHandler(strIn))
				{
				case 1:
					CommandHelp();		//"help"
					break;
				case 2:
					CommandStatus();	//"status"
					break;
				case 3:
					//preParam = 0;
					CMConfigure();		//"configui"
					break;
				case 4:
					FastConfig(strIn);	//"config CONFIGCODE"
					break;
				default:				//Unknown command
					btSerial.print("Can't recognise command \"");
					btSerial.print(strIn);
					btSerial.println("\". Please, repeat. Try \"help\" for command list.");
					break;
				}

			}
}

//Команда "help"
void CMUI::CommandHelp()
{
	btSerial.println("List of available commands:");
						btSerial.println("help - shows this text");
						btSerial.println("status - shows current configuration");
						btSerial.println("configui - runs configuring dialoge");
						btSerial.println("config CONFIGCODE - configures with CONFIGCODE parameters");
						btSerial.println();
						btSerial.println("Nixiris' MusicColor project 2017");
}

//Текущие параметры (команда "status")
void CMUI::CommandStatus()
{
	btSerial.print("Your current configuration: ");
	btSerial.print(configParams, HEX);
	btSerial.println();
}

unsigned int CMUI::CMConfigure(uint8_t Params, uint32_t Color[7])
{
	configParamsTmp = Params;
	for (int color = 0; color < 7; color++)
	{
		configColors[color] = Color[color];
	}
	return Params;
}

unsigned int CMUI::CMConfigure(uint8_t Params)
{
	uint32_t Colors[7] = { 0,0,0,0,0,0,0 };
	return CMConfigure(Params, Colors);
}

//Интерактивный конфигуратор
unsigned int CMUI::CMConfigure()
/*
 * 00000000 00000000 00000000 00 00 00 00
   (   R  ) (  G   ) (   B  )  4  3  2  1
None         =   0x0(0)
1. 0x0..0x3
   0x1(1)	=	Mode Freq			=	0x1
   0x2(2)	=	Mode Amp			=	0x2

2. 0x4, 0x8, 0xC
0x1+
	0x4(1)	=	Range				=	0x5
	0x8(2)	=	Rotation			=	0x9
0x2+
	0x4(1)	=	LowToHigh			=	0x6
	0x8(2)	=	From Center			=	0xA
3. 0x10, 0x20, 0x30
0x1+0x4+
	0x10(1)	=	Rainbow				=	0X15
	0x20(2)	=	Random Color		=	0x25
	0x30(3)	=	Single Color		=	0x35
	0x1+0x8+
	 	0x10(1)	=	Rainbow				=	0x19
	0x20(2)	=	Random Color		=	0x29
	0x30(3)	=	Single Color		=	0x39
	0x2+0x4+
	 	0x10(1)	=	RGB					=	0x16
	 	0x20(2)	=	Single Color		=	0x26
	0x2+0x8+
		0x10(1)	=	RGB					=	0x1A
	 	0x20(2)	=	Single Color		=	0x2A
4. 0x40, 0x80, 0xC0
0x1+0x4+0x10+
	0x40		=	Straight		=	0x55
	0x80		=	Reverse			=	0x95
0x1+0x4+0x30+
	0x40		=	Pick Color		=	0x75
	0x80		=	Dynamic Color	=	0xB5
0x1+0x8+0x10+
	0x40		=	Straight		=	0x59
	0x80		=	Reverse			=	0x99
0x1+0x8+0x30+
	0x40		=	Pick Color		=	0x79
	0x80		=	Dynamic Color	=	0xB9
0x2+0x4+0x20+
	0x40		=	Pick Color		=	0x66
	0x80		=	Dynamic Color	=	0xA6
0x2+0x8+0x20+
	0x40		=	Pick Color		=	0x6A
	0x80		=	Dynamic Color	=	0xAA
5.	RGB 24bit
0x75+RGB
0x79+RGB
0x66+RGB
0x6A+RGB
 */
{
	//Serial.setTimeout(0);
	//Serial.println(preParam);
	unsigned int serInput = 0;
	unsigned int prep = 0;
	shellAvailable = false;
	//flagParamsInited = false;
	switch(preParam)
	{
	case 0x25:
	case 0x29:
	case 0x16:
	case 0x1A:
	case 0x55:
	case 0x95:
	case 0xB5:
	case 0x59:
	case 0x99:
	case 0xA6:
	case 0xAA:
		if(configNeeded)
		{
			shellAvailable = true;
			//Serial.println(preParam);
			configNeeded = false;
			return preParam;
		}
		else
		{
			configNeeded = true;
			preParam = 0;
		}
		break;
	}
	if((prep=(preParam&(unsigned int)0x3))==(unsigned int)0)		//Первый параметр не установлен
	{
		//DebugMsg("sdfasf");
		if(!flagModeL1SelectPrinted)
		{
			//DebugMsg("ajsl");
			btSerial.println("Please, choose mode:");
			btSerial.println("1) Frequency Mode");
			btSerial.println("2) Amplitude Mode");
			flagModeL1SelectPrinted = true;
		}
		//DebugMsg("Available?");
		if(btSerial.available())
		{
			//DebugMsg("Yes");
			serInput = btSerial.parseInt();
			if((serInput>0)&&(serInput<3))
			{
				preParam +=serInput;
				flagModeL1SelectPrinted = false;
			}
			else
			{
				btSerial.println("You entered wrong number.");
				flagModeL1SelectPrinted = false;
			}
		}

	}
	else if(((prep=(preParam>>(unsigned int)0x2))&(unsigned int)0x3)==(unsigned int)0)	//Второй параметр не установлен
	{
		if(!flagModeL2SelectPrinted)
		{
			btSerial.println("Please, specify:");
			switch(preParam)
			{
			case 0x1:
				btSerial.println("1) Range");
				btSerial.println("2) Rotation");
				break;
			case 0x2:
				btSerial.println("1) Low To High");
				btSerial.println("2) From Center");
				break;
			}
			flagModeL2SelectPrinted = true;
		}
		if(btSerial.available())
		{
			serInput = btSerial.parseInt();
			if((serInput>0)&&(serInput<3))
			{
				preParam += serInput<<0x2;
				flagModeL2SelectPrinted = false;
			}
			else
			{
				btSerial.println("You entered wrong number.");
				flagModeL2SelectPrinted = false;
			}
		}
	}
	else if(((preParam>>(unsigned int)0x4)&(unsigned int)0x3)==(unsigned int)0)		//3
	{
		if(!flagModeL3SelectPrinted)
		{
			btSerial.println("Please, specify:");
			switch(preParam)
			{
			case 0x5:
				btSerial.println("1) Rainbow");
				btSerial.println("2) Random Color");
				btSerial.println("3) Single Color");
				break;
			case 0x9:
				btSerial.println("1) Rainbow");
				btSerial.println("2) Random Color");
				btSerial.println("3) Single Color");
				break;
			case 0x6:
				btSerial.println("1) RGB");
				btSerial.println("2) Single Color");
				break;
			case 0xA:
				btSerial.println("1) RGB");
				btSerial.println("2) Single Color");
				break;
			}
			flagModeL3SelectPrinted = true;
		}
		if(btSerial.available())
		{
			serInput = btSerial.parseInt();
			if((serInput>0)&&(serInput<4))
			{
				preParam += serInput<<0x4;
				flagModeL3SelectPrinted = false;
			}
			else
			{
				btSerial.println("You entered wrong number.");
				flagModeL3SelectPrinted = false;
			}
		}
	}
	else if(((preParam>>0x6)&0x3)==0)		//4
	{
		if(!flagModeL4SelectPrinted)
		{
			btSerial.println("Please, specify:");
			switch(preParam)
			{
			case 0x15:
				btSerial.println("1) Straight");
				btSerial.println("2) Reverse");
				break;
			case 0x35:
				btSerial.println("1) Pick Color");
				btSerial.println("2) Dynamic Color");
				break;
			case 0x19:
				btSerial.println("1) Straight");
				btSerial.println("2) Reverse");
				break;
			case 0x39:
				btSerial.println("1) Pick Color");
				btSerial.println("2) Dynamic Color");
				break;
			case 0x26:
				btSerial.println("1) Pick Color");
				btSerial.println("2) Dynamic Color");
				break;
			case 0x2A:
				btSerial.println("1) Pick Color");
				btSerial.println("2) Dynamic Color");
				break;
			}
			flagModeL4SelectPrinted = true;
		}
		if(btSerial.available())
		{
			serInput = btSerial.parseInt();
			if((serInput>0)&&(serInput<3))
			{
				preParam += serInput<<0x6;
				flagModeL4SelectPrinted = false;
			}
			else
			{
				btSerial.println("You entered wrong number.");
				flagModeL4SelectPrinted = false;
			}

		}
	}
	else if(((preParam>>0x8)&0xFF)==0)		//5
	{
		if(!flagModeL5SelectPrinted)
		{
			 btSerial.println("Please, specify color (RGB 0x000001-0xFFFFFF");
			flagModeL5SelectPrinted = true;
		}
		if(btSerial.available())
		{
			serInput = btSerial.parseInt();
			if((serInput>0)&&(serInput<0x1000000))
			{
				preParam += serInput<<0x8;
				flagModeL5SelectPrinted = false;
			}
			else
			{
				btSerial.println("You entered wrong number.");
				flagModeL5SelectPrinted = false;
			}

		}
	}
	//DebugMsg("return 0");
	return 0;
}

/*String ConfigUncode(unsigned int configCode)
{
	String retStr;
	switch(configCode)
	{
		case 0x1:
			return "Frequency Mode";
			break;
		case 0x2:
			return "Amplitude Mode";
			break;
		case 0x5:
			return "Frequency Mode->Range";
			break;
		case 0x9:
			return "Frequency Mode->Rotation";
			break;
		case 0x6:
			return "Amplitude Mode->LowToHigh";
			break;
		case 0xA:
			return "Amplitude Mode->From Center";
			break;
		case 0x15:
			return "Frequency Mode->Range->Rainbow";
			break;
		case 0x25:
			return "Frequency Mode->Range->Random Color";
			break;
		case 0x35:
			return "Frequency Mode->Range->Single Color";
			break;
		case 0x19:
			return "Frequency Mode->Rotation->Rainbow";
			break;
		case 0x29:
			return "Frequency Mode->Rotation->Random Color";
			break;
		case 0x39:
			return "Frequency Mode->Rotation->Single Color";
			break;
		case 0x16:
			return "Amplitude Mode->LowToHigh->RGB";
			break;
		case 0x26:
			return "Amplitude Mode->LowToHigh->Single Color";
			break;
		case 0x1A:
			return "Amplitude Mode->From Center->RGB";
			break;
		case 0x2A:
			return "Amplitude Mode->From Center->Single Color";
			break;
		case 0x55:
			return "Frequency Mode->Range->Rainbow->Straight";
			break;
		case 0x95:
			return "Frequency Mode->Range->Rainbow->Reverse";
			break;
		case 0x75:
			return "Frequency Mode->Range->Single Color->Pick Color";
			break;
		case 0xB5:
			return "Frequency Mode->Range->Single Color->Dynamic Color";
			break;
		case 0x59:
			return "Frequency Mode->Rotation->Rainbow->Straight";
			break;
		case 0x99:
			return "Frequency Mode->Rotation->Rainbow->Reverse";
			break;
		case 0x79:
			return "Frequency Mode->Rotation->Single Color->Pick Color";
			break;
		case 0xB9:
			return "Frequency Mode->Rotation->Single Color->Dynamic Color";
			break;
		case 0x66:
			return "Amplitude Mode->LowToHigh->Single Color->Pick Color";
			break;
		case 0xA6:
			return "Amplitude Mode->LowToHigh->Single Color->Dynamic Color";
			break;
		case 0x6A:
			return "Amplitude Mode->From Center->Single Color->Pick Color";
			break;
		case 0xAA:
			return "Amplitude Mode->From Center->Single Color->Dynamic Color";
			break;
		case 0x00000100 ... 0xFFFFFFFF:
			switch(configCode&0x11)
			{
			case 0x75:

				retStr = "Frequency Mode->Range->Single Color->Pick Color->";
				retStr = retStr + (unsigned int)(configCode>>0x100);
				return retStr;
				break;
			case 0x79:
				//String retStr;
				retStr = "Frequency Mode->Rotation->Single Color->Pick Color->";
				retStr = retStr + (unsigned int)(configCode>>0x100);
				return retStr;
				break;
			case 0x66:
				//String retStr;
				retStr = "Amplitude Mode->LowToHigh->Single Color->Pick Color->";
				retStr = retStr + (unsigned int)(configCode>>0x100);
				return retStr;
				break;
			case 0x6A:
				//String retStr;
				retStr = "Amplitude Mode->From Center->Single Color->Pick Color->";
				retStr = retStr + (unsigned int)(configCode>>0x100);
				return retStr;
				break;
			}
			break;
	}
	return "Incorrect Configuration";
}*/

//unsigned int CMUI::Configure()
//{
//	//Serial.println("Enter configuration code:");
//	bool sa = false;
//	String serInput;
//	int serInputInt = 0;
//	if (btSerial.available())
//	{
//		sa = true;
//		configNeeded = false;
//
//		while (btSerial.available())
//		{
//			btSerial.setTimeout(50);
//			//serInput += String(Serial.read());
//			serInputInt += btSerial.parseInt();
//			//return 0x55;
//		}
//
//	}
//	return serInputInt;
//}

void CMUI::FastConfig(String strIn)
{
	int parCnt = 0;
	uint8_t config = 0;
	uint32_t color[7];
	int shift = 0;
	while (shift = strIn.indexOf(" ", shift+1) != -1)
	{
		parCnt++;
		if (parCnt == 1)
		{
			if (strIn.indexOf(" ", shift + 1) != -1)
			{
				config = strIn.substring(strIn.indexOf(" ") + 1, strIn.indexOf(" ", shift + 1)).toInt();
			}
			else
			{
				config = strIn.substring(strIn.indexOf(" ") + 1).toInt();
			}
		}
	}
	if (parCnt == 1)
	{
		CMConfigure(config);
	}
	else
	{
		CMConfigure(config, color);
	}
	
	
}

//void CMUI::ActivateModeButton()
//{
//	if (digitalRead(ModeButton) == HIGH)
//	{
//		UserButtonLastState = HIGH;
//	}
//	else
//	{
//		if (UserButtonLastState == HIGH)
//		{
//			UserButtonLastState = LOW;
//			RotateConfig();
//		}
//	}
//}

void CMUI::ModeButton()
{
	if (digitalRead(ModeButtonPin) == LOW)
	{
		UserButtonLastState = LOW;
	}
	else
	{
		if (UserButtonLastState == LOW)
		{
			UserButtonLastState = HIGH;
			RotateConfig();
		}
	}
}

//Переключение режима по нажатию кнопки
void CMUI::RotateConfig()
{
	switch (configParams)
	{
	case 0x00:
		CMConfigure(0x55);
		break;
	case 0x16:
		CMConfigure(0x1A);
		break;
	case 0x1A:
		CMConfigure(0x00);
		break;
	case 0x55:
		CMConfigure(0x95);
		break;
	case 0x95:
		CMConfigure(0x16);
		break;
	}
}