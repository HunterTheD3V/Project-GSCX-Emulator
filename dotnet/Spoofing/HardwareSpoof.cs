using System;
using System.Runtime.InteropServices;

namespace GSCX.Spoofing
{
    public static class HardwareSpoof
    {
        // Stub de spoofing: retorna identificadores simulados
        public static string GetConsoleId() => "GSCX-PS3-EMU";
        public static string GetBoardModel() => "CECH-XXXX";
        public static string GetRegion() => "US";

        // Futuro: exportar via UnmanagedCallersOnly para ser consumido por C++/Python
        [UnmanagedCallersOnly(EntryPoint = "GSCX_GetConsoleId")]
        public static IntPtr GSCX_GetConsoleId()
        {
            return Marshal.StringToCoTaskMemUTF8(GetConsoleId());
        }
    }
}