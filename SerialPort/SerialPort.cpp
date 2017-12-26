#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>

#include "open_comm.h"
#include "close_comm.h"
#include "set_comm.h"
#include "send_data.h"
#include "receive_data.h"
#include "layout.h"

#include <atlstr.h>


static HANDLE com_handler = INVALID_HANDLE_VALUE;//���ھ��
//static TCHAR  visibla[10000] = { 0 };//

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
	set_layout(hwnd);
	return TRUE;
}



void Main_OnCommand(HWND hwnd, int command, HWND hwndCtl, UINT codeNotify)
{
	BOOL  close_flag = false;

	switch(command){
		case ID_OPEN:
			com_handler = Open_Serial_Port(hwnd);//�򿪴���
			if (com_handler != INVALID_HANDLE_VALUE) {
				Set_Serial_Port(hwnd, com_handler);
			}
			break;

		case ID_CLOSE:
			Close_Serial_Port(hwnd);
			break;

		case IDC_SEND:
			//�ж϶˿��Ƿ��
			Send_Serial_Port(hwnd, com_handler);
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

void Main_OnSize(HWND hwnd, UINT state, INT cx, INT cy)
{
	set_layout(hwnd);

}
void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd,0);
}

