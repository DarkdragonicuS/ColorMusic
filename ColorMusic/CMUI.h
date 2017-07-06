/*
 * CMUI.h
 *
 *  Created on: 2 июн. 2017 г.
 *      Author: Nixiris
 */

#ifndef CMUI_H_
#define CMUI_H_
#include <Arduino.h>

//extern bool G_shellAvailable;

//bool flagParamsInited = false;
extern unsigned int preParam;
extern unsigned int configParamsTmp;
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
	static void ModeButton();
private:
	static int GetCommandHandler(String command);	//Список команд
	static void CommandHelp();		//Команда help
	static void CommandStatus();	//Команда status
	static unsigned int CMConfigure();		//Интерактивный конфигуратор
	//static unsigned int Configure();
	static unsigned int CMConfigure(uint8_t Params, uint32_t Color[7]);	//установка указанной конфигурации
	static unsigned int CMConfigure(uint8_t Params);	
	static void FastConfig(String strIn);	//команда config CONFIGCODE
	static void RotateConfig();	//переключение режима

	static bool shellAvailable;	//флаг блокировки вывода
};



#endif /* CMUI_H_ */
