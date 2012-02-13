/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace YeloDebug
{
    /// <summary>
    /// Kornman H4x - helper functions used to inject dlls into an xbox process.
    /// </summary>
    public class XboxDll
    {
        [DllImport("UtilDll.dll", EntryPoint = "Util_RelocateDll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public extern static int RelocateDll(
            [MarshalAs(UnmanagedType.LPStr)] string Path,
            [In, Out, MarshalAs(UnmanagedType.LPArray)] byte[] reloc_dll,
            uint reloc_dll_size,
            ref uint base_address);

        [DllImport("UtilDll.dll", EntryPoint = "Util_CalculateDllCodeSize", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public extern static int CalculateDllCodeSize(
            [MarshalAs(UnmanagedType.LPStr)] string Path,
            out uint code_size);
    }
}