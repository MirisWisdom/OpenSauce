/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Runtime.InteropServices;

namespace BlamLib.IO
{
	public static class XCompress
	{
		const string kDllName = "OS_XnaNative.dll";

		#region Compression
		[DllImport(kDllName, CallingConvention=CallingConvention.Cdecl)]
		public extern static
			IntPtr CreateCompressionContext();
		[DllImport(kDllName, CallingConvention=CallingConvention.Cdecl)]
		public extern static
			void DestroyCompressionContext(IntPtr context);
		[DllImport(kDllName, CallingConvention=CallingConvention.Cdecl)]
		public extern static
			uint Compress(IntPtr context, [In, Out] byte[] dest, ref int dest_size, [In] byte[] src, int src_size);
		#endregion

		#region Decompression
		[DllImport(kDllName, CallingConvention=CallingConvention.Cdecl)]
		public extern static
			IntPtr CreateDecompressionContext();
		[DllImport(kDllName, CallingConvention=CallingConvention.Cdecl)]
		public extern static
			void DestroyDecompressionContext(IntPtr context);

		[DllImport(kDllName, CallingConvention=CallingConvention.Cdecl)]
		public extern static
			uint Decompress(IntPtr context, [In, Out] byte[] dest, ref int dest_size, [In] byte[] src, ref int src_size);
		#endregion
	};
}