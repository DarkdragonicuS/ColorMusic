/*
 * CMUI.h
 *
 *  Created on: 2 ���. 2017 �.
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
	static void CommandShell();	//������ ����������� ��������������
	static void ModeButton();
private:
	static int GetCommandHandler(String command);	//������ ������
	static void CommandHelp();		//������� help
	static void CommandStatus();	//������� status
	static unsigned int CMConfigure();		//������������� ������������
	//static unsigned int Configure();
	static unsigned int CMConfigure(uint8_t Params, uint32_t Color[7]);	//��������� ��������� ������������
	static unsigned int CMConfigure(uint8_t Params);	
	static void FastConfig(String strIn);	//������� config CONFIGCODE
	static void RotateConfig();	//������������ ������

	static bool shellAvailable;	//���� ���������� ������
};



#endif /* CMUI_H_ */
