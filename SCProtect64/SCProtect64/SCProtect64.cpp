// SCProtect32.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"


HHOOK hhk = NULL;

BOOL hkBitBlt(HDC hdc,int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
BOOL hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

extern HOOK_TRACE_INFO hBitBltHook;
extern HOOK_TRACE_INFO hPrintWindowHook;
extern ULONG InThreadIdList[1];
extern HMODULE hInstance;

LRESULT CALLBACK dummy(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) BOOL installGlobalHook()
{
	hhk = SetWindowsHookExW(WH_DEBUG, dummy, hInstance, 0);
	if (hhk == NULL)
		return FALSE;
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL uninstallGlobalHook()
{
	if (hhk == NULL)
		return FALSE;
	UnhookWindowsHookEx(hhk);
	return TRUE;
}

BOOL hkBitBlt(HDC hdc,int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
	DWORD ObjectType = GetObjectType(hdcSrc);
	if (ObjectType != OBJ_DC)
		return BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	else
		return TRUE;
}

BOOL hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags)
{
	return TRUE;
}