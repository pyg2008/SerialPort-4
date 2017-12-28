#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <atlstr.h>
#include <commdlg.h>

#include "resource.h"
#include "SerialPort.h"
#include "common.h"

static BOOL get_comm_name(TCHAR *pname,INT len,HWND hwnd) {
	HWND  hwnd_comm; //��ô�������ľ��
	INT index = 0;
	*pname++ = '\\';
	*pname++ = '\\';
	*pname++ = '.';
	*pname++ = '\\';
	hwnd_comm = GetDlgItem(hwnd, IDC_COMBOSERIAL);//��ô�������ľ��
	index = ComboBox_GetCurSel(hwnd_comm);//�õ�����������ڵ�ѡ�������ֵ
	ComboBox_GetLBText(hwnd_comm, index, pname);//�õ�����ֵ������
	return TRUE;
}

HANDLE Open_Serial_Port(HWND hwnd) {
	INT   index;
	HANDLE hCom;  //���ھ��
	TCHAR comm_name[COMM_NAME_LEN] = {0};
	CString str;
	get_comm_name(comm_name, sizeof(comm_name), hwnd);
	hCom = CreateFile(comm_name,  //COM��
		GENERIC_READ | GENERIC_WRITE, //�������д
		0,  //��ռ��ʽ
		NULL,
		OPEN_EXISTING,  //�򿪶����Ǵ���
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص���ʽ
		NULL
		);

	if (INVALID_HANDLE_VALUE == hCom) {
		str.Format(_T("��%sʧ��!"),comm_name + strlen("\\\\.\\"));
		MessageBox(NULL,str, TEXT("Error"), NULL);
	}
	return hCom;
}