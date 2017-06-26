/*
 * CMUI.h
 *
 *  Created on: 2 июн. 2017 г.
 *      Author: Nixiris
 */

#ifndef CMUI_H_
#define CMUI_H_
#include <Arduino.h>

extern bool G_shellAvailable;

//bool flagParamsInited = false;
extern unsigned int preParam;
extern unsigned int curParams;
extern bool flagModeL1SelectPrinted;
extern bool flagModeL2SelectPrinted;
extern bool flagModeL3SelectPrinted;
extern bool flagModeL4SelectPrinted;
extern bool flagModeL5SelectPrinted;

class CMUI
{
public:
	static void Welcome();
	static void CommandShell();	//Запуск коммандного интерпретатора
	//String ConfigUncode(unsigned int configCode);
	static void CheckUPState();
private:
	static int GetCommandHandler(String command);	//Список команд
	static void CommandHelp();		//Команда help
	static void CommandStatus(long params[], char paramCnt);	//Команда status
	static unsigned int MCConfigure();		//Интерактивный конфигуратор
	static unsigned int Configure();
	static unsigned int MCConfigure(unsigned int Params);
	static void FastConfig(String strIn);
	static void RotateConfig();
};



#endif /* CMUI_H_ */
