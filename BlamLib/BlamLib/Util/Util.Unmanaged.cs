/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/﻿using System;
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