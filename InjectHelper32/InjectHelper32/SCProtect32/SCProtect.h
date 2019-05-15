#ifndef SCPROTECT_H

#define SCPROTECT_H

extern "C" __declspec(dllimport) BOOL installGlobalHook();
extern "C" __declspec(dllimport) BOOL uninstallGlobalHook();

#endif