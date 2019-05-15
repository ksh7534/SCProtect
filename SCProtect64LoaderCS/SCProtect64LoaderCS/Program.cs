using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace SCProtect64LoaderCS
{
    class Program
    {
        [DllImport("InjectHelper64.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "installHooks")]
        private static extern int install();
        [DllImport("InjectHelper64.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "uninstallHooks")]
        private static extern int uninstall();

        static void Main(string[] args)
        {
            try
            {
                Process p = Process.GetProcessById(Convert.ToInt32(args[0]));
                p.EnableRaisingEvents = true;
                p.Exited += new EventHandler(Program_Exited);
                install();
                Console.Read();
                uninstall();
            }
            catch
            {
                //아무것도 안함
                //do nothing
            }
        }

        private static void Program_Exited(object Sender, System.EventArgs e)
        {
            //메인 프로그램 종료시 64비트 OS용 캡처 방지 모듈도 함께 종료
            //If the main program is killed,
            //then SCProtect64LoaderCS should be killed together
            uninstall();
            Process.GetCurrentProcess().Kill();
        }
    }
}

