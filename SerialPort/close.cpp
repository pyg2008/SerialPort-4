#include "stdafx.h"

BOOL Close_Serial_Port(HWND hwnd, HANDLE com_handler) {
	BOOL close_flag = false;
	HWND hwnd_open  = NULL;
	HWND hwnd_close = NULL;
	close_flag = CloseHandle(com_handler);//�رմ���
	if (TRUE == close_flag) {
		com_handler = INVALID_HANDLE_VALUE;

		hwnd_open = GetDlgItem(hwnd, ID_OPEN);//����Դ򿪴��ڰ�ť������
		EnableWindow(hwnd_open, TRUE);
		hwnd_close = GetDlgItem(hwnd, ID_CLOSE);//ʹ�رմ��ڰ�ť����
		EnableWindow(hwnd_close, FALSE);
	}
	return close_flag;
	//ShowWindow(hwnd,SW_HIDE);
}