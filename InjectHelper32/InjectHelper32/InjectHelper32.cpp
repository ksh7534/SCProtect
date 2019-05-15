// InjectHelper32.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
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