// InjectHelper32.cpp : DLL ���� ���α׷��� ���� ������ �Լ��� �����մϴ�.
//

#include "stdafx.h"

extern "C" __declspec(dllexport) int __cdecl installHooks()
{
	return installGlobalHook();
}

extern "C" __declspec(dllexport) int __cdecl uninstallHooks()
{
	return uninstallGlobalHook();
}