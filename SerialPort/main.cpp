// main.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Resource.h"
#include "SerialPort.h"
#include <commctrl.h>
#include <commdlg.h>
//#define MAX_LOADSTRING 100
//
// ȫ�ֱ���:
//HINSTANCE hInst;								// ��ǰʵ��
//TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
//TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//InitCommonControls();

	LoadLibrary( "riched20.dll" );
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN),NULL,Main_Proc);
	return 0;
}