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
		[DllImport("UtilDll.dll", EntryPoint = "Util_RebaseModule", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public extern static int RebaseModule(
            [MarshalAs(UnmanagedType.LPStr)] string Path,
            [In, Out, MarshalAs(UnmanagedType.LPArray)] byte[] reloc_dll,
            uint reloc_dll_size,
            ref uint base_address,
			uint reloc_alloc_count);

		[DllImport("UtilDll.dll", EntryPoint = "Util_CalculateModuleCodeSize", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public extern static int CalculateModuleCodeSize(
            [MarshalAs(UnmanagedType.LPStr)] string Path,
            out uint code_size);
    }
}