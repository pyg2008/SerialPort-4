#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>
#include <stdio.h>
#include <atlstr.h>
#include "OpenSerialPort.h"

#define MAX_BUFFER_SIZE (1024)
#define MAX_COMM_NUM    (32)
#define MAX_BOUD_BUFFER (16)

static HANDLE com_handler = INVALID_HANDLE_VALUE;//���ھ��
static TCHAR  visibla[10000] = { 0 };//

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
		
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, Main_OnSize);
		HANDLE_MSG(hWnd, WM_CLOSE, Main_OnClose);
    }

    return FALSE;
}

BOOL get_comm_name(LPBYTE buffer, UINT32 num) {
	UINT32 i = 0;
	UINT32 r = 0;
	BYTE temp = 0;
	if (NULL == buffer || num >= MAX_COMM_NUM) {
		return FALSE;
	}
	buffer[i++] = 'C';
	buffer[i++] = 'O';
	buffer[i++] = 'M';
	itoa(num, (char *)&buffer[i], 10);
	return TRUE;
}


BOOL set_candidate_comm_name(HWND hwnd) {
	UINT32 i = 0;
	HWND  hwnd_comm_sel;
	BOOL result = FALSE;
	BYTE comm_name[32] = { 0 };
	hwnd_comm_sel = GetDlgItem(hwnd, IDC_COMBOSERIAL);//��Ӵ���ѡ��
	for (i = 0; i < MAX_COMM_NUM; i++) {
		result = get_comm_name(comm_name, i);
		if (FALSE == result) {
			break;
		}
		ComboBox_InsertString(hwnd_comm_sel, -1, comm_name);
		ZeroMemory(comm_name, sizeof(comm_name));
	}
	ComboBox_SetCurSel(hwnd_comm_sel, i / 2);
	return TRUE;
}


BOOL set_candidate_boud_rate(HWND hwnd) {
	UINT32 i = 0;
	HWND  hwnd_boud_sel;
	TCHAR  buffer[MAX_BOUD_BUFFER];
	UINT32 boud_rate[] = {600,	1200,	2400,	4800,	9600,	19200,
						38400,	56000,	57600,	115200,	128000,	256000,	1440000};
	hwnd_boud_sel = GetDlgItem(hwnd, IDC_COMBOBOUDRATE);//��Ӳ�����ѡ��
	for (i = 0; i < sizeof(boud_rate) / sizeof(boud_rate[0]); i++) {
		itoa(boud_rate[i], buffer, 10);
		ComboBox_InsertString(hwnd_boud_sel, -1, buffer);
	}
	ComboBox_SetCurSel(hwnd_boud_sel, 4);
	return TRUE;
}


BOOL set_candidate_data_bit(HWND hwnd) {
	HWND hwndDate = GetDlgItem(hwnd, IDC_DATE);//����λѡ��
	ComboBox_InsertString(hwndDate, -1, "4");
	ComboBox_InsertString(hwndDate, -1, "5");
	ComboBox_InsertString(hwndDate, -1, "6");
	ComboBox_InsertString(hwndDate, -1, "7");
	ComboBox_InsertString(hwndDate, -1, "8");
	ComboBox_SetCurSel(hwndDate, 4);
	return TRUE;
}

BOOL set_candidate_stop_bit(HWND hwnd) {
	HWND hwndStop = GetDlgItem(hwnd, IDC_STOP);//ֹͣλѡ��
	ComboBox_InsertString(hwndStop, -1, "1");
	ComboBox_InsertString(hwndStop, -1, "2");
	ComboBox_SetCurSel(hwndStop, 0);
	return TRUE;
}

BOOL set_candidate_check_bit(HWND hwnd) {

	HWND hwnd_check_bit = GetDlgItem(hwnd, IDC_COMBO3);//У��λ
	//EVENPARITY żУ��     NOPARITY ��У��
	//MARKPARITY ���У��   ODDPARITY ��У��
	ComboBox_InsertString(hwnd_check_bit, -1, "NOPARITY");
	ComboBox_InsertString(hwnd_check_bit, -1, "ODDPARITY");
	ComboBox_InsertString(hwnd_check_bit, -1, "EVENPARITY");
	ComboBox_InsertString(hwnd_check_bit, -1, "MARKPARITY");
	ComboBox_SetCurSel(hwnd_check_bit, 0);
	return TRUE;
}

BOOL set_candidate_data_format(HWND hwnd) {
	
	HWND receiveHex = GetDlgItem(hwnd, IDC_RADIO1);//����_�õ�16���Ƶİ�ť���
	Button_SetCheck(receiveHex, 1);//����Ĭ��ѡ��
	HWND sendHex = GetDlgItem(hwnd, IDC_RADIO3);//����_�õ�16���Ƶİ�ť���
	Button_SetCheck(sendHex, 1);//����Ĭ��ѡ��
	return TRUE;
}

BOOL set_default_icon(HWND hwnd) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(SerialPort));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	return TRUE;
}




HANDLE Open_Serial_Port(TCHAR com_name[]){

	HANDLE hCom;  //���ھ��
	hCom = CreateFile(com_name,  //COM��
				GENERIC_READ | GENERIC_WRITE, //�������д
				0,  //��ռ��ʽ
				NULL,
				OPEN_EXISTING,  //�򿪶����Ǵ���
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص���ʽ
				NULL
			);

	if (INVALID_HANDLE_VALUE == hCom){
		MessageBox(NULL, TEXT("��COMʧ��!"), TEXT("Error"), NULL);
		return hCom;
	}
	return hCom;
}



BOOL convent_to_hex(TCHAR *buffer, DWORD len) {

	DWORD i = 0;

	if (NULL == buffer || len < 0 || len > MAX_BUFFER_SIZE) {
		return false;
	}

	for (i = 0; i < len; i++) {
		if ('0' <= buffer[i] && buffer[i] <= '9') {//��0-9֮������ֽ���ת��
			buffer[i] = buffer[i] - '0';
		}
		else if ('A' <= buffer[i] && buffer[i] <= 'F') {//��A-F֮������ݽ���ת��
			buffer[i] = buffer[i] - 'A' + 10;
		}
		else if ('a' <= buffer[i] && buffer[i] <= 'f') {//��a-f֮������ݽ���ת��	
			buffer[i] = buffer[i] - 'a' + 10;
		}
		else {//���зǷ��ַ����б��
			return false;
		}
	}
	return true;
}

DWORD WINAPI Receive_Serial_Port_Thread(LPVOID lpParam)//���ڶ��̵߳ĺ���
{
	HWND    hwnd = (HWND)lpParam; //��ô��ھ��
	TCHAR   lpInBuffer[MAX_BUFFER_SIZE]; //���ö�������
	DWORD   dwBytesRead = MAX_BUFFER_SIZE;//��ȡ�����ֽ���
	BOOL    bReadStatus;//���Ƿ�ɹ�
	DWORD   dwErrorFlags;
	COMSTAT ComStat;//�Դ��ھ��м�������
	OVERLAPPED m_osRead;
	HWND hwnd_hex = GetDlgItem(hwnd, IDC_RADIO1);//����_�õ�16���Ƶİ�ť���
	HWND hwnd_richedit = GetDlgItem(hwnd, IDC_RICHEDIT22);

	INT hex_format = 0;//����_�õ������״̬
	UINT32 i = 0;
	while (1){
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		memset(lpInBuffer, 0, sizeof(lpInBuffer));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ClearCommError(com_handler, &dwErrorFlags, &ComStat);
		hex_format = Button_GetCheck(hwnd_hex);

		if (!ComStat.cbInQue){//��������û�����ݣ���0==ComStat.cbInQue////////////////////////////////
			Sleep(50);//�����������Ҫ�����û�еĻ�cup��ʹ���ʻ�ܸߣ���Լ30%�����ù�֮��ή�ͺܶࣨ3%���ң��������ڴ������ݺ�Ƶ����ϵͳ������ʱ��Ҫ�����̣�����Ӱ��Ч�ʡ�
			continue;
		}
		else{//�������������ݣ���0��=ComStat.cbInQue
			dwBytesRead = (DWORD)ComStat.cbInQue;//�õ������������ֽ���
			bReadStatus = ReadFile(com_handler, lpInBuffer,
							dwBytesRead, &dwBytesRead, &m_osRead);//���ж����ڴ���
			if (!bReadStatus && (GetLastError() == ERROR_IO_PENDING)){ //���ReadFile��������FALSE
				GetOverlappedResult(com_handler, &m_osRead, &dwBytesRead, TRUE);
				PurgeComm(com_handler,
					PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				if (1 == hex_format) {//��16��������
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}
				
				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL,SB_BOTTOM,0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//��ʾ��richedit��
				CloseHandle(m_osRead.hEvent);
			}
			else{// //ReadFile��������TRUE
				if (1 == hex_format){//����16��������
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

BOOL Send_Serial_Port(HWND hwnd) {

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

		hwnd_hex   = GetDlgItem(hwnd, IDC_RADIO3);//����_�õ�16���Ƶİ�ť���
		hex_format = Button_GetCheck(hwnd_hex);//�õ������״̬
		PurgeComm(com_handler, PURGE_TXCLEAR);//���д������
		ZeroMemory(buffer, sizeof(buffer));
		GetDlgItemText(hwnd, IDC_EDIT2, buffer, sizeof(buffer));//�õ�Ҫд�������
		len = strlen(buffer);//Ҫд�����ݵĳ���
		if (1 == hex_format) {//����16��������
			check_flag = convent_to_hex(buffer, len);
		}
	
		if (!check_flag){//�ж��зǷ��ַ�
			MessageBox(hwnd, TEXT("�������16������0-9��a-f��A-F֮��"), TEXT("Error"), MB_OK);
		}
		else{
			m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			bWriteStat = WriteFile(com_handler, buffer,len, &len, &m_osWrite);
			if (!bWriteStat){
				if (GetLastError() == ERROR_IO_PENDING){
					WaitForSingleObject(m_osWrite.hEvent, 1000);
				}
			}
			CloseHandle(m_osWrite.hEvent);
		}
	}
	return true;
}
BOOL Set_Serial_Port(HWND hwnd) {
	//HWND hwndIDOK    = GetDlgItem(hwnd, IDOK);//���δ򿪴��ڰ�ť
	//ShowWindow(hwndIDOK,SW_HIDE);
	//HWND hwndIDCLOSE = GetDlgItem(hwnd, IDCLOSE);//����Թرմ��ڰ�ť������
	//ShowWindow(hwndIDCLOSE,SW_SHOW);
	HWND hwnd_data_bit;  //����λ handler
	HWND hwnd_baud_rate; //������ handler
	HWND hwnd_check_bit; //У��λ handler
	HWND hwnd_stop_bit;  //ֹͣλ handler
	TCHAR buffer[10] = {0};
	INT   index      = 0;
	DCB   dcb;
	COMMTIMEOUTS TimeOuts;

	SetupComm(com_handler, 1024, 1024); //���뻺����������������Ĵ�С����1024
	
	TimeOuts.ReadIntervalTimeout         = 1000;//�趨����ʱ
	TimeOuts.ReadTotalTimeoutMultiplier  = 500;
	TimeOuts.ReadTotalTimeoutConstant    = 5000;

	TimeOuts.WriteTotalTimeoutMultiplier = 500;//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutConstant   = 2000;
	SetCommTimeouts(com_handler, &TimeOuts);    //���ó�ʱ

	/* Get current comm port state */
	GetCommState(com_handler, &dcb);
	dcb.DCBlength = sizeof(dcb);

	/* Get data bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_data_bit = GetDlgItem(hwnd, IDC_DATA_BIT);
	index         = ComboBox_GetCurSel(hwnd_data_bit);//�õ����ڵ�ѡ�������ֵ
	ComboBox_GetLBText(hwnd_data_bit, index, buffer);//�õ�����ֵ������
	dcb.ByteSize  = atoi(buffer);    //ÿ���ֽ���8λ

	/* Get stop bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_stop_bit = GetDlgItem(hwnd, IDC_STOP_BIT);
	index         = ComboBox_GetCurSel(hwnd_stop_bit);//�õ����ڵ�ѡ�������ֵ
	ComboBox_GetLBText(hwnd_stop_bit, index, buffer);//�õ�����ֵ������
	dcb.StopBits  = atoi(buffer) - 1; //ֹͣλ

	/* Get boud rate config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_baud_rate = GetDlgItem(hwnd, IDC_BOUD_RATE);//���ò�����
	index          = ComboBox_GetCurSel(hwnd_baud_rate);
	ComboBox_GetLBText(hwnd_baud_rate, index, buffer);
	dcb.BaudRate   = atoi(buffer);       //������

	/*Get check bit config*/
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_check_bit = GetDlgItem(hwnd, IDC_CHECK_BIT);//����У��λ
	index          = ComboBox_GetCurSel(hwnd_check_bit);
	ComboBox_GetLBText(hwnd_check_bit, index, buffer);
	dcb.Parity     = *buffer; //������żУ��λ

	/* Set current comm port state */
	SetCommState(com_handler, &dcb);
	//PurgeComm(com_handler,PURGE_TXCLEAR|PURGE_RXCLEAR);
	//SetCommMask(com_handler,);

	/* Create a thread to receive data */
	CreateThread(NULL, 0, Receive_Serial_Port_Thread, hwnd, 0, 0);//��ʼ���߳�
	return true;
}


BOOL Close_Serial_Port(HWND hwnd) {
	BOOL close_flag    = false;
	HWND hwnd_ID_OPEN  = NULL;
	HWND hwnd_ID_CLOSE = NULL;
	close_flag  = CloseHandle(com_handler);//�رմ���
	com_handler = INVALID_HANDLE_VALUE;
	//hwnd_ID_OPEN  = GetDlgItem(hwnd, ID_OPEN);//����Դ򿪴��ڰ�ť������
	//ShowWindow(hwnd_ID_OPEN, SW_SHOW);
	//hwnd_ID_CLOSE = GetDlgItem(hwnd, ID_CLOSE);//ʹ�رմ��ڰ�ť����
	//ShowWindow(hwndIDCLOSE,SW_HIDE);
	return close_flag;
	//ShowWindow(hwnd,SW_HIDE);
}


BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//HWND hwndIDCLOSE = GetDlgItem(hwnd,IDCLOSE);//��ùرհ�ť�ľ��
	//ShowWindow(hwndIDCLOSE,SW_HIDE);//�����ιرմ���ѡ��
	//ShowWindow(hwndIDOK,SW_);
	BOOL result = FALSE;
	set_candidate_comm_name(hwnd);
	set_candidate_boud_rate(hwnd);
	set_candidate_data_bit(hwnd);
	set_candidate_stop_bit(hwnd);
	set_candidate_check_bit(hwnd);
	set_candidate_data_format(hwnd);
	//����ͼ���ļ�...
	set_default_icon(hwnd);
	return TRUE;
}


void Main_OnSize(HWND hwnd, UINT state, INT cx, INT cy)
{
	RECT stRect;
	RECT rich_edit_rect1;
	RECT rich_edit_rect2;
	HWND hwnd_richedit;
	int left = 0;
	int top = 0;
	int height = 0;
	int width = 0;
	CString s;
	hwnd_richedit = GetDlgItem(hwnd, IDC_RICHEDIT22);
	GetClientRect(hwnd, &stRect); // ��ȡ���ڿͻ�����С
	GetWindowRect(hwnd_richedit, &rich_edit_rect1);
	GetClientRect(hwnd_richedit, &rich_edit_rect2);
	// ��RichEdit��С����Ϊ�ͻ�����С</span>
	//s.Format(_T("top = %d  bottom = %d  left = %d  right = %d"), rich_edit_rect.top, rich_edit_rect.bottom, rich_edit_rect.left, rich_edit_rect.right);
	ScreenToClient(hwnd, (LPPOINT)&rich_edit_rect1);

	
	s.Format(_T("top = %d  bottom = %d  left = %d  right = %d  top = %d  bottom = %d  left = %d  right = %d  top = %d  bottom = %d  left = %d  right = %d  "), stRect.top, stRect.bottom, stRect.left, stRect.right,
		
		rich_edit_rect2.top, rich_edit_rect2.bottom, rich_edit_rect2.left, rich_edit_rect2.right,
		rich_edit_rect1.top, rich_edit_rect1.bottom, rich_edit_rect1.left, rich_edit_rect1.right
		);
	SetDlgItemText(hwnd, IDC_RICHEDIT22, s);
	height = (stRect.bottom - rich_edit_rect1.top) * 500 /1000;
	width = stRect.right - rich_edit_rect1.left - 20;
	//MessageBox(hwnd, s, TEXT("Error"), MB_OK);
	MoveWindow(hwnd_richedit, rich_edit_rect1.left, rich_edit_rect1.top,  width,height, TRUE);
}

void Main_OnCommand(HWND hwnd, int command, HWND hwndCtl, UINT codeNotify)
{
	HWND  hwnd_comm; //��ô�������ľ��
	TCHAR comm_name[256];	
	INT   index;
	BOOL  close_flag = false;

	switch(command){
		case ID_OPEN:
			ZeroMemory(comm_name, sizeof(comm_name));
			hwnd_comm   = GetDlgItem(hwnd, IDC_COMBOSERIAL);//��ô�������ľ��
			index       = ComboBox_GetCurSel(hwnd_comm);//�õ�����������ڵ�ѡ�������ֵ
			/* Get comm name */
			ComboBox_GetLBText(hwnd_comm, index, comm_name);//�õ�����ֵ������
			com_handler = Open_Serial_Port(comm_name);//�򿪴���
			
			if (com_handler != INVALID_HANDLE_VALUE) {
				Set_Serial_Port(hwnd);
			}
			break;

		case ID_CLOSE:
			Close_Serial_Port(hwnd);
			break;

		case IDC_SEND:
			//�ж϶˿��Ƿ��
			Send_Serial_Port(hwnd);
			break;

		case IDC_CLEAR_RECEIVE:
			SetDlgItemText(hwnd,IDC_RICHEDIT22,NULL);//��ս�����
			break;

		case IDC_CLEAR_SEND:
			SetDlgItemText(hwnd,IDC_EDIT2,NULL);//��շ�����
			break;
        default:
		    break;
    }
}

void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd,0);
}

