// dllmain.cpp : DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"

HMODULE hInstance = NULL;

char oldBitBlt[7] = { 0, 0, 0, 0, 0, 0, 0 };
char oldPrintWindow[5] = { 0, 0, 0, 0, 0 };

extern BOOL installHk(const char* dllName, const char* apiName, DWORD addr, char *originalCode);
extern BOOL uninstallHk(const char* dllName, const char* apiName, char* originalCode);
extern BOOL installHkThreadSafe(const char* dllName, const char* apiName, DWORD addr, char *originalCode);
extern BOOL uninstallHkThreadSafe(const char* dllName, const char* apiName, char* originalCode);
extern BOOL WINAPI hkBitBlt(HDC hdc,int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
extern BOOL WINAPI hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hInstance = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		installHkThreadSafe("gdi32.dll", "BitBlt", (DWORD)hkBitBlt, oldBitBlt);
		installHk("user32.dll", "PrintWindow", (DWORD)hkPrintWindow, oldPrintWindow);
		break;

	case DLL_PROCESS_DETACH:
		uninstallHkThreadSafe("gdi32.dll", "BitBlt", oldBitBlt);
		uninstallHk("user32.dll", "PrintWindow", oldPrintWindow);
		break;
	}
	
	return TRUE;
}
