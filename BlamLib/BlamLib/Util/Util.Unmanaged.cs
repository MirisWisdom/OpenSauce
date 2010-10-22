/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

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
﻿using System;
using System.Runtime.InteropServices;

namespace BlamLib
{
	partial class Util
	{
		/// <summary>
		/// Utility functions related to unmanaged operations
		/// </summary>
		public static class Unmanaged
		{
			/// <summary>
			/// Convert a handle to object
			/// </summary>
			/// <param name="ptr">Handle</param>
			/// <param name="t">Type to use for converting</param>
			/// <returns>Managed object</returns>
			public static object IntPtrToStructure(IntPtr ptr, Type t)	{ return Marshal.PtrToStructure(ptr, t); }
			/// <summary>
			/// Convert a handle to object
			/// </summary>
			/// <param name="ptr">Handle</param>
			/// <returns>Managed object</returns>
			public static T IntPtrToStructure<T>(IntPtr ptr)			{ return (T)Marshal.PtrToStructure(ptr, typeof(T)); }
			/// <summary>
			/// Allocate unmanaged memory for an object of type <paramref name="t"/>
			/// </summary>
			/// <param name="t">Type to allocate memory for</param>
			/// <returns>Handle to allocated memory</returns>
			public static IntPtr New(Type t)							{ return Marshal.AllocHGlobal(Marshal.SizeOf(t)); }
			/// <summary>
			/// Free unmanaged memory for an existing object
			/// </summary>
			/// <param name="ptr">Memory allocated by <c>New</c></param>
			public static void Delete(IntPtr ptr)						{ Marshal.FreeHGlobal(ptr); }
		};
	};
}