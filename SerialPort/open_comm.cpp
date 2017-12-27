#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>
#include "common.h"


HANDLE Open_Serial_Port(HWND hwnd) {
	INT   index;
	HANDLE hCom;  //���ھ��
	TCHAR comm_name[256];
	HWND  hwnd_comm; //��ô�������ľ��
	ZeroMemory(comm_name, sizeof(comm_name));
	hwnd_comm = GetDlgItem(hwnd, IDC_COMBOSERIAL);//��ô�������ľ��
	index = ComboBox_GetCurSel(hwnd_comm);//�õ�����������ڵ�ѡ�������ֵ
	//get_comm_name(comm_name, index);	/* Get comm name */
	ComboBox_GetLBText(hwnd_comm, index, comm_name);//�õ�����ֵ������

	hCom = CreateFile(comm_name,  //COM��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0,  //��ռ��ʽ
		NULL,
		OPEN_EXISTING,  //�򿪶����Ǵ���
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص���ʽ
		NULL
		);

	if (INVALID_HANDLE_VALUE == hCom) {
		MessageBox(NULL, TEXT("��COMʧ��!"), TEXT("Error"), NULL);
		return hCom;
	}
	return hCom;
}