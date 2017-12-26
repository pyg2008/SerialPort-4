#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "common.h"
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