// dllmain.cpp : DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"

HMODULE hInstance = NULL;

extern BOOL hkBitBlt(HDC hdc,int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
extern BOOL hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

ULONG InThreadIdList[1] = { 0xFFFFFFFF };

HOOK_TRACE_INFO hBitBltHook = { NULL };
HOOK_TRACE_INFO hPrintWindowHook = { NULL };


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance = hModule;
		LhInstallHook(GetProcAddress(GetModuleHandleA("gdi32.dll"), "BitBlt"), hkBitBlt, NULL, &hBitBltHook);
		LhSetExclusiveACL(InThreadIdList, 1, &hBitBltHook);
		LhInstallHook(GetProcAddress(GetModuleHandleA("user32.dll"), "PrintWindow"), hkPrintWindow, NULL, &hPrintWindowHook);
		LhSetExclusiveACL(InThreadIdList, 1, &hPrintWindowHook);
		break;

	case DLL_PROCESS_DETACH:
		LhUninstallHook(&hBitBltHook);
		LhWaitForPendingRemovals();
		LhUninstallHook(&hPrintWindowHook);
		LhWaitForPendingRemovals();
		break;
	}
	
	return TRUE;
}
