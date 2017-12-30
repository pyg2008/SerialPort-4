#include "stdafx.h"

extern HWND main_hwnd;
DWORD WINAPI Receive_Serial_Port_Thread(LPVOID lpParam)//���ڶ��̵߳ĺ���
{
	HANDLE    com_handler = (HANDLE)lpParam; //��ô��ھ��
	TCHAR   lpInBuffer[MAX_BUFFER_SIZE]; //���ö�������
	DWORD   dwBytesRead = MAX_BUFFER_SIZE;//��ȡ�����ֽ���
	BOOL    bReadStatus;//���Ƿ�ɹ�
	DWORD   dwErrorFlags;
	COMSTAT ComStat;//�Դ��ھ��м�������
	OVERLAPPED m_osRead;
	HWND hwnd_hex = GetDlgItem(main_hwnd, IDC_RADIO1);//����_�õ�16���Ƶİ�ť���
	HWND hwnd_richedit = GetDlgItem(main_hwnd, IDC_RICHEDIT22);

	INT hex_format = 0;//����_�õ������״̬
	UINT32 i = 0;
	while (1) {
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		memset(lpInBuffer, 0, sizeof(lpInBuffer));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ClearCommError(com_handler, &dwErrorFlags, &ComStat);
		hex_format = Button_GetCheck(hwnd_hex);

		if (!ComStat.cbInQue) {//��������û�����ݣ���0==ComStat.cbInQue////////////////////////////////
			Sleep(50);//�����������Ҫ�����û�еĻ�cup��ʹ���ʻ�ܸߣ���Լ30%�����ù�֮��ή�ͺܶࣨ3%���ң��������ڴ������ݺ�Ƶ����ϵͳ������ʱ��Ҫ�����̣�����Ӱ��Ч�ʡ�
			continue;
		}
		else {//�������������ݣ���0��=ComStat.cbInQue
			dwBytesRead = (DWORD)ComStat.cbInQue;//�õ������������ֽ���
			bReadStatus = ReadFile(com_handler, lpInBuffer,
				dwBytesRead, &dwBytesRead, &m_osRead);//���ж����ڴ���
			if (!bReadStatus && (GetLastError() == ERROR_IO_PENDING)) { //���ReadFile��������FALSE
				GetOverlappedResult(com_handler, &m_osRead, &dwBytesRead, TRUE);
				PurgeComm(com_handler,
					PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				if (1 == hex_format) {//��16��������
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}

				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL, SB_BOTTOM, 0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//��ʾ��richedit��
				CloseHandle(m_osRead.hEvent);
			}
			else {// //ReadFile��������TRUE
				if (1 == hex_format) {//����16��������
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}
				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL, SB_BOTTOM, 0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//��ʾ��richedit��
				PurgeComm(com_handler, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				CloseHandle(m_osRead.hEvent);
			}
		}
	}
	return 0;
}