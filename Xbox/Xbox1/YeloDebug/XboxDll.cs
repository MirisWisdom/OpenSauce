/*
    OpenSauceBox: SDK for Xbox User Modding

    Copyright (C)  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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