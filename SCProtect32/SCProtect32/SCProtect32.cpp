// SCProtect32.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"


HHOOK hhk = NULL;
extern char oldBitBlt[5];

BOOL installHk(const char* dllName, const char* apiName, DWORD addr, char *originalCode);
BOOL uninstallHk(const char* dllName, const char* apiName, char* originalCode);
BOOL WINAPI hkBitBlt(HDC hdc,int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
BOOL WINAPI hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

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

BOOL installHk(const char* dllName, const char* apiName, DWORD addr, char *originalCode)
{
	DWORD dwOldProtect;
	DWORD apiAddr = (DWORD)GetProcAddress(GetModuleHandleA(dllName), apiName);
	if (apiAddr == NULL)
		return FALSE;
	BYTE *pbTargetCode = (BYTE *) apiAddr;
	BYTE *pbReplaced = (BYTE *) addr;
	VirtualProtect((void *) apiAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(originalCode, pbTargetCode, 5);
	*pbTargetCode++ = 0xE9;
	*((unsigned int *)(pbTargetCode)) = pbReplaced - (pbTargetCode +4);
	VirtualProtect((void *) apiAddr, 5, dwOldProtect, NULL);
	FlushInstructionCache(GetCurrentProcess(), NULL, NULL);
	return TRUE;
}

BOOL uninstallHk(const char* dllName, const char* apiName, char *originalCode)
{
	DWORD dwOldProtect;
	DWORD apiAddr = (DWORD)GetProcAddress(GetModuleHandleA(dllName), apiName);
	if (apiAddr == NULL)
		return FALSE;
	BYTE *pbTargetCode = (BYTE *) apiAddr;
	VirtualProtect((void *) apiAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(pbTargetCode, originalCode, 5);
	VirtualProtect((void *) apiAddr, 5, dwOldProtect, NULL);
	FlushInstructionCache(GetCurrentProcess(), NULL, NULL);
	return TRUE;
}

BOOL installHkThreadSafe(const char* dllName, const char* apiName, DWORD addr, char *originalCode)
{
	DWORD dwOldProtect;
	DWORD apiAddr = (DWORD)GetProcAddress(GetModuleHandleA(dllName), apiName);
	if (apiAddr == NULL)
		return FALSE;
	BYTE *pbTargetCode = (BYTE *) apiAddr - 5;
	BYTE *pbReplaced = (BYTE *) addr;
	VirtualProtect((void *) pbTargetCode, 7, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(originalCode, pbTargetCode, 7);
	*pbTargetCode++ = 0xE9;
	*((unsigned int *)(pbTargetCode)) = pbReplaced - (pbTargetCode +4);
	pbTargetCode += 4;
	*pbTargetCode++ = 0xEB;
	*pbTargetCode = 0xF9;
	VirtualProtect((void *) pbTargetCode, 7, dwOldProtect, NULL);
	FlushInstructionCache(GetCurrentProcess(), NULL, NULL);
	return TRUE;
}

BOOL uninstallHkThreadSafe(const char* dllName, const char* apiName, char *originalCode)
{
	DWORD dwOldProtect;
	DWORD apiAddr = (DWORD)GetProcAddress(GetModuleHandleA(dllName), apiName);
	if (apiAddr == NULL)
		return FALSE;
	BYTE *pbTargetCode = (BYTE *) apiAddr - 5;
	VirtualProtect((void *) pbTargetCode, 7, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(pbTargetCode, originalCode, 7);
	VirtualProtect((void *) pbTargetCode, 7, dwOldProtect, NULL);
	FlushInstructionCache(GetCurrentProcess(), NULL, NULL);
	return TRUE;
}

BOOL WINAPI hkBitBlt(HDC hdc,int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
	DWORD ObjectType = GetObjectType(hdcSrc);
	if (ObjectType != OBJ_DC)
		return ((BOOL (WINAPI *)(HDC, int, int, int, int, HDC, int, int, DWORD))((BYTE *)BitBlt + 2))(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	else
		return FALSE;
}

BOOL WINAPI hkPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags)
{
	return TRUE;
}