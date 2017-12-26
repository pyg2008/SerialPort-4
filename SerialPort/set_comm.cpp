#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>

BOOL Set_Serial_Port(HWND hwnd, HANDLE com_handler) {
	//HWND hwndIDOK    = GetDlgItem(hwnd, IDOK);//���δ򿪴��ڰ�ť
	//ShowWindow(hwndIDOK,SW_HIDE);
	//HWND hwndIDCLOSE = GetDlgItem(hwnd, IDCLOSE);//����Թرմ��ڰ�ť������
	//ShowWindow(hwndIDCLOSE,SW_SHOW);
	HWND hwnd_data_bit;  //����λ handler
	HWND hwnd_baud_rate; //������ handler
	HWND hwnd_check_bit; //У��λ handler
	HWND hwnd_stop_bit;  //ֹͣλ handler
	TCHAR buffer[10] = { 0 };
	INT   index = 0;
	DCB   dcb;
	COMMTIMEOUTS TimeOuts;

	SetupComm(com_handler, 1024, 1024); //���뻺����������������Ĵ�С����1024

	TimeOuts.ReadIntervalTimeout = 1000;//�趨����ʱ
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 5000;

	TimeOuts.WriteTotalTimeoutMultiplier = 500;//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(com_handler, &TimeOuts);    //���ó�ʱ

												/* Get current comm port state */
	GetCommState(com_handler, &dcb);
	dcb.DCBlength = sizeof(dcb);

	/* Get data bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_data_bit = GetDlgItem(hwnd, IDC_DATA_BIT);
	index = ComboBox_GetCurSel(hwnd_data_bit);//�õ����ڵ�ѡ�������ֵ
	ComboBox_GetLBText(hwnd_data_bit, index, buffer);//�õ�����ֵ������
	dcb.ByteSize = atoi(buffer);    //ÿ���ֽ���8λ

									/* Get stop bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_stop_bit = GetDlgItem(hwnd, IDC_STOP_BIT);
	index = ComboBox_GetCurSel(hwnd_stop_bit);//�õ����ڵ�ѡ�������ֵ
	ComboBox_GetLBText(hwnd_stop_bit, index, buffer);//�õ�����ֵ������
	dcb.StopBits = atoi(buffer) - 1; //ֹͣλ

									 /* Get boud rate config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_baud_rate = GetDlgItem(hwnd, IDC_BOUD_RATE);//���ò�����
	index = ComboBox_GetCurSel(hwnd_baud_rate);
	ComboBox_GetLBText(hwnd_baud_rate, index, buffer);
	dcb.BaudRate = atoi(buffer);       //������

									   /*Get check bit config*/
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_check_bit = GetDlgItem(hwnd, IDC_CHECK_BIT);//����У��λ
	index = ComboBox_GetCurSel(hwnd_check_bit);
	ComboBox_GetLBText(hwnd_check_bit, index, buffer);
	dcb.Parity = *buffer; //������żУ��λ

						  /* Set current comm port state */
	SetCommState(com_handler, &dcb);
	//PurgeComm(com_handler,PURGE_TXCLEAR|PURGE_RXCLEAR);
	//SetCommMask(com_handler,);

	/* Create a thread to receive data */
	CreateThread(NULL, 0, Receive_Serial_Port_Thread, hwnd, 0, 0);//��ʼ���߳�
	return true;
}

