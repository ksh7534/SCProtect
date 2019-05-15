# SCProtect

### Introduction
Module for screen capture prevention written in C++, with loader written in C#.

- This module is designed to work with 32-bit C# programs running on 32/64-bit Windows.

- In theory, it should work any Windows OS newer than XP SP2, but only tested on Windows 10 version 1607 (64-bit) & Windows 7 SP1 (32/64-bit), so it might not work with other version of Windows.

- This module mess with some low-level system stuff like Global API Hooking, so it may conflict with other programs(Anti-Virus, Anti-Cheat, etc...) and **may result in catastrophic system failure**.

- ***USE AT YOUR OWN RISK!!!!!!!***

### Usage
- Download latest version of EasyHook from [easyhook.github.io][EasyHook]

- Copy easyhook.h, EasyHook64.dll and EasyHook64.lib to SCProtect64\SCProtect64\EasyHook

- Build SCProtect32, SCProtect64 and SCProtect64LoaderCS

- Copy SCProtect32.dll and SCProtect32.lib to InjectHelper32\InjectHelper32\SCProtect32

- Copy SCProtect64.dll and SCProtect64.lib to InjectHelper64\InjectHelper64\SCProtect64

- Build InjectHelper32 and InjectHelper64

- Import InjectHelper32.dll, InjectHelper64.dll, SCProtect32.dll, SCProtect64.dll, SCProtect64LoaderCS.exe, EasyHook64.dll on your C# project

- Set those 6 files to be copied to output folder

- Import SCProtect.cs on your C# project

- Change namespace of SCProtect class

- Call SCProtect.installHooks() and SCProtect.uninstallHooks() to start/stop screen caputre prevention

### How does it work?
- This module performs global API hooking to prevent the GDI API called *BitBlt* from copying OBJ_DC source to arbitrary buffer. It also disables WinUser API called *PrintWindow* via global API hooking. By restricting these two APIs, most of screen capture program will not work correctly.

- However, even if we hook those two APIs, PrintScreen key will work as usual, so additional low-level keyboard hook written in C# will clear clipboard when PrintScreen key is pressed.

### License
- MIT License

### Open Source Information
- EasyHook - [easyhook.github.io][EasyHook]

------------

### 개요
C++로 작성된 화면 캡처 방지 모듈과, 해당 모듈을 로드하기 위한 C# 로더입니다.
- 이 모듈은 32/64비트 윈도우 환경에서 실행되는 32비트 C# 프로그램을 위해 설계되었습니다.
- 이론상 이 모듈은 윈도우 XP SP2 이후의 모든 버전에서 작동하도록 만들어졌으나, 윈도우 10 버전 1607(64비트) 및 윈도우 7 SP1(32/64비트)에서만 테스트되었으므로, 다른 버전의 윈도우에서는 작동하지 않을 수 있습니다.
- 이 모듈은 전역 API 후킹과 같이 저수준에서 시스템에 영향을 미치는 행위를 하기 때문에, 다른 프로그램(백신, 치트방지 프로그램, 인터넷 뱅킹 보안프로그램 등)과 충돌을 일으켜 **심각한 시스템 장애를 일으킬 가능성이 있습니다**.
- ***이 모듈을 사용하여 발생하는 문제는 사용자 본인 책임입니다!!!!!***

### 사용법
- EasyHook의 최신 버전을 [easyhook.github.io][EasyHook] 에서 다운로드합니다.

- easyhook.h, EasyHook64.dll, EasyHook64.lib 3개 파일을 SCProtect64\SCProtect64\EasyHook 안에 복사합니다.

- SCProtect32, SCProtect64, SCProtect64LoaderCS 3개 프로젝트를 빌드합니다.

- SCProtect32.dll, SCProtect32.lib 2개 파일을 InjectHelper32\InjectHelper32\SCProtect32 안에 복사합니다.

- SCProtect64.dll, SCProtect64.lib 2개 파일을 InjectHelper64\InjectHelper64\SCProtect64 안에 복사합니다.

- InjectHelper32, InjectHelper64 2개 프로젝트를 빌드합니다.

- InjectHelper32.dll, InjectHelper64.dll, SCProtect32.dll, SCProtect64.dll, SCProtect64LoaderCS.exe, EasyHook64.dll 6개 파일을 적용할 C# 프로젝트에 포함시킵니다.

- 포함된 6개 파일을 출력 시 결과 폴더에 복사하도록 설정합니다.

- SCProtect.cs 파일을 적용할 C# 프로젝트에 포함시킵니다.

- SCProtect 클래스의 네임스페이스를 적절하게 변경합니다.

- SCProtect.installHooks(), SCProtect.uninstallHooks() 함수를 원하는 위치에서 호출하여 화면 캡처 방지 기능을 시작/종료할 수 있습니다.

### 어떻게 작동하나요?
- 이 모듈은 현재 시스템에서 실행중이거나 앞으로 실행될 모든 프로세스에서 GDI API중 하나인 BitBlt가 호출될 때 인자 중 hdcSrc의 타입을 검사하여 OBJ_DC 타입인 경우 기능이 작동하지 않도록 전역 API 후킹을 수행합니다. 또한 화면을 저장할 수 있는 또 다른 윈도우 사용자 API인 PrintWindow 함수에 대해서도 전역 API 후킹을 수행합니다. BitBlt와 PrintWindow의 사용을 제한하는 것으로도 상당수의 화면 캡처 프로그램을 무력화할 수 있습니다.
- 하지만, 윈도우에 기본적으로 내장된 PrintScreen을 눌러 화면을 캡처하는 행위는 위의 2가지 API를 후킹하더라도 막을 수 없기 때문에 추가적으로 C# 로더 클래스에서 저수준 키보드 후킹을 통해 PrintScreen을 눌러 클립보드에 이미지가 채워지는 즉시 클립보드의 내용을 삭제하여 결과물을 얻을 수 없도록 처리하였습니다.

### 라이선스
- MIT 라이선스

### 오픈소스 정보
- EasyHook - [easyhook.github.io][EasyHook]

[EasyHook]: https://github.com/EasyHook/EasyHook "EasyHook Homepage"