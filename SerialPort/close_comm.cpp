#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>

BOOL Close_Serial_Port(HWND hwnd, HANDLE com_handler) {
	BOOL close_flag = false;
	HWND hwnd_ID_OPEN = NULL;
	HWND hwnd_ID_CLOSE = NULL;
	close_flag = CloseHandle(com_handler);//�رմ���
	com_handler = INVALID_HANDLE_VALUE;
	//hwnd_ID_OPEN  = GetDlgItem(hwnd, ID_OPEN);//����Դ򿪴��ڰ�ť������
	//ShowWindow(hwnd_ID_OPEN, SW_SHOW);
	//hwnd_ID_CLOSE = GetDlgItem(hwnd, ID_CLOSE);//ʹ�رմ��ڰ�ť����
	//ShowWindow(hwndIDCLOSE,SW_HIDE);
	return close_flag;
	//ShowWindow(hwnd,SW_HIDE);
}