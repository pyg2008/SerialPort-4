#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>
#include "common.h"
#include "data_format.h"

BOOL Send_Serial_Port(HWND hwnd, HANDLE com_handler) {

	HWND  hwnd_hex;
	INT   hex_format;
	TCHAR buffer[MAX_BUFFER_SIZE];//׼��д�������
	DWORD len = sizeof(buffer);         //Ҫд�����ݵ��ֽ�
	BOOL  check_flag = true;//�Ƿ��зǷ��ַ��ı�־
	BOOL  bWriteStat = FALSE;//�����ж�д���Ƿ�ɹ�
	OVERLAPPED m_osWrite = { 0 };

	if (com_handler == INVALID_HANDLE_VALUE) {
		MessageBox(hwnd, TEXT("Invalid Port Number"), TEXT("Error"), MB_OK);
	}
	else {

		hwnd_hex = GetDlgItem(hwnd, IDC_RADIO3);//����_�õ�16���Ƶİ�ť���
		hex_format = Button_GetCheck(hwnd_hex);//�õ������״̬
		PurgeComm(com_handler, PURGE_TXCLEAR);//���д������
		ZeroMemory(buffer, sizeof(buffer));
		GetDlgItemText(hwnd, IDC_EDIT2, buffer, sizeof(buffer));//�õ�Ҫд�������
		len = strlen(buffer);//Ҫд�����ݵĳ���
		if (1 == hex_format) {//����16��������
			check_flag = convent_to_hex(buffer, len);
		}

		if (!check_flag) {//�ж��зǷ��ַ�
			MessageBox(hwnd, TEXT("�������16������0-9��a-f��A-F֮��"), TEXT("Error"), MB_OK);
		}
		else {
			m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			bWriteStat = WriteFile(com_handler, buffer, len, &len, &m_osWrite);
			if (!bWriteStat) {
				if (GetLastError() == ERROR_IO_PENDING) {
					WaitForSingleObject(m_osWrite.hEvent, 1000);
				}
			}
			CloseHandle(m_osWrite.hEvent);
		}
	}
	return true;
}

