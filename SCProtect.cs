using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Windows.Forms;

namespace PROJECT
{
    class SCProtect
    {
        private static Process SCProtectLoader = null;
        private static bool __Is64BitOS = false;

        [DllImport("InjectHelper32.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "installHooks")]
        private static extern int install();
        [DllImport("InjectHelper32.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "uninstallHooks")]
        private static extern int uninstall();
        [STAThread]
        public static void installHooks()
        {
            //PrintScreen key 후킹
			//Hook PrintScreen key
            _hookID = SetHook(_proc);
            //스크린 캡처 방지(32비트)
			//Enable screen capture prevention for 32-bit systems
            install();
            //64비트 시스템 여부를 확인하여 저장
			//Check whether current OS is 64-bit or not, and save the result for later use
            __Is64BitOS = Is64BitOS();
            if (__Is64BitOS)
            {
                //스크린 캡처 방지(64비트)
				//Enable screen capture prevention for 64-bit systems
                Process[] list = Process.GetProcessesByName("SCProtect64LoaderCS");
                if (list.Length > 0)
                {
                    for (int i = 0; i < list.Length; i++)
                    {
                        list[i].Kill();
                    }
                }
                SCProtectLoader = Process.Start(new ProcessStartInfo() { WindowStyle = ProcessWindowStyle.Hidden,
																		CreateNoWindow = true,
																		FileName = "SCProtect64LoaderCS.exe",
																		Arguments = Process.GetCurrentProcess().Id.ToString() });
                SCProtectLoader.EnableRaisingEvents = true;
                SCProtectLoader.Exited += new EventHandler(SCProtectLoader_Exited);
            }
        }

        public static void uninstallHooks()
        {
            //PrintScreen key 후킹 해제
			//Unhook PrintScreen key
            UnhookWindowsHookEx(_hookID);
            //스크린 캡처 방지 해제(32비트)
			//Disable screen capture prevention for 32-bit systems
            uninstall();
            //64비트 시스템 여부 확인
			//Is this system 64-bit?
            if (__Is64BitOS)
            {
                //스크린 캡처 방지 해제(64비트)
				//Disable screen capture prevention for 64-bit systems
                SCProtectLoader.EnableRaisingEvents = false;
                SCProtectLoader.Kill();
            }
        }

        private static void SCProtectLoader_Exited(object Sender, System.EventArgs e)
        {
            //64비트 OS용 캡처 방지 모듈 강제 종료시 메인 프로그램도 함께 종료
			//When 64-bit capture prevention module loader is killed,
			//then stop the program together to prevent screen being captured
            Application.Exit();
        }

        private const int WH_KEYBOARD_LL = 13;
        private const int WM_KEYDOWN = 0x0100;
        private const int WM_SYSKEYDOWN = 0x0104;
        private const int HC_ACTION = 0;

        private static LowLevelKeyboardProc _proc = HookCallback;
        private static IntPtr _hookID = IntPtr.Zero;

		//Keyboard Hook
        private static IntPtr SetHook(LowLevelKeyboardProc proc)
        {
            using (Process curProcess = Process.GetCurrentProcess())
            using (ProcessModule curModule = curProcess.MainModule)
            {
                return SetWindowsHookEx(WH_KEYBOARD_LL, proc, GetModuleHandle(curModule.ModuleName), 0);
            }
        }

        private delegate IntPtr LowLevelKeyboardProc(int nCode, IntPtr wParam, IntPtr lParam);

		//Callback fucntion for PrintScreen key hook
        private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
        {
            if (nCode == HC_ACTION && (wParam == (IntPtr)WM_KEYDOWN || wParam == (IntPtr)WM_SYSKEYDOWN))
                if (Marshal.ReadInt32(lParam) == 44)
                {
                    //PrintScreen을 누르고 Clipboard에 이미지가 올라가는데 시간이 걸림
                    //PrintScreen 키가 눌리면 Clipboard에 든 비트맵 이미지를 없어질때까지 제거 시도
					//When PrintScreen is pressed, it takes some time to copy current screen on clipboard
					//Try to clear clipboard until the image is completely removed from clipboard
                    while (Clipboard.ContainsImage())
                        Clipboard.Clear();
                    return (IntPtr)1;
                }
            return CallNextHookEx(_hookID, nCode, wParam, lParam);
        }

        private static bool Is64BitOS()
        {
            using (Process p = Process.GetCurrentProcess())
            {
                bool ret = false;
                return (IntPtr.Size == 8 || (DoesWin32MethodExist("kernel32.dll", "IsWow64Process") && IntPtr.Size == 4 && IsWow64Process(p.Handle, out ret) && ret));
            }
        }

        private static bool DoesWin32MethodExist(string moduleName, string methodName)
        {
            IntPtr moduleHandle = GetModuleHandle(moduleName);
            if (moduleHandle == IntPtr.Zero)
                return false;
            return (GetProcAddress(moduleHandle, methodName) != IntPtr.Zero);
        }

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr SetWindowsHookEx(int idHook, LowLevelKeyboardProc lpfn, IntPtr hMod, uint dwThreadId);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr GetProcAddress([In]IntPtr moduleHandle, [MarshalAs(UnmanagedType.LPStr)]string MethodName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsWow64Process([In]IntPtr hProcess, [Out]out bool lpSystemInfo);

        

    }
}
