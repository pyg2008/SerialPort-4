#include "stdafx.h"

BOOL close_serial_port(HWND hwnd, HANDLE sp_hdr) {
	BOOL ret        = FALSE;
	HWND hwnd_open  = NULL;
	HWND hwnd_close = NULL;

	ret = CloseHandle(sp_hdr);//close serial port
	
	if (TRUE == ret) {
		sp_hdr = INVALID_HANDLE_VALUE;
		hwnd_open = GetDlgItem(hwnd, ID_OPEN);//����Դ򿪴��ڰ�ť������
		EnableWindow(hwnd_open, TRUE);
		hwnd_close = GetDlgItem(hwnd, ID_CLOSE);//ʹ�رմ��ڰ�ť����
		EnableWindow(hwnd_close, FALSE);
	}

	return ret;
}