/*
 * CMUI.h
 *
 *  Created on: 2 ���. 2017 �.
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

void Welcome();
int GetCommandHandler(String command);	//������ ������
void CommandShell();	//������ ����������� ��������������
void CommandHelp();		//������� help
void CommandStatus(long params[], char paramCnt);	//������� status
unsigned int MCConfigure();		//������������� ������������
unsigned int Configure();
unsigned int MCConfigure(unsigned int Params);
void FastConfig(String strIn);
//String ConfigUncode(unsigned int configCode);
void CheckUPState();
void RotateConfig();

#endif /* CMUI_H_ */
