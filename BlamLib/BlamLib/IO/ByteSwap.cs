/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.IO
{
    /// <summary>
	/// Byte swapping tools
	/// </summary>
	public static class ByteSwap
	{
		/// <summary>
		/// Delegate definition for functions which can byte swap tag data
		/// </summary>
		/// <param name="owner">owner object of the <paramref name="data"/> field</param>
		/// <param name="data">field which holds the tag data</param>
		/// <returns>new buffer which holds the byte swapped data</returns>
		public delegate byte[] ByteSwapDelegate(TagInterface.IStructureOwner owner, TagInterface.Data data);

		#region Codes
		/// <summary>
		/// Byteswap code for 8 bits of data
		/// </summary>
		public const int Byte = 1;
		/// <summary>
		/// Byteswap code for 16 bits of data
		/// </summary>
		public const int Word = -2;
		/// <summary>
		/// Byteswap code for 32 bits of data
		/// </summary>
		public const int DWord = -4;
		/// <summary>
		/// Byteswap code for 64 bits of data
		/// </summary>
		public const int QWord = -8;
		/// <summary>
		/// Byteswap code for the start of a repeated table of byte swap codes.
		/// Next int in the byte swap code list is the amount of times to repeat the codes
		/// </summary>
		public const int ArrayStart = -100;
		/// <summary>
		/// Byteswap code for the end of a repeated table of byte swap codes
		/// </summary>
		public const int ArrayEnd = -101;
		#endregion

		#region short
		/// <summary>
		/// Swaps a short integer at a position in a bye array
		/// </summary>
		/// <param name="data">source array</param>
		/// <param name="offset">offset to perform swap</param>
		public static void SwapWord(byte[] data, int offset)
		{
			byte hi = data[offset];
			byte low = data[offset + 1];

			data[offset] = low;
			data[offset + 1] = hi;
		}

		/// <summary>
		/// Swaps a short integer by reference
		/// </summary>
		/// <param name="word"></param>
		public static void SwapWord(ref short word) { word = (short)SwapWord( word ); }

		/// <summary>
		/// Swaps a short integer and returns the result
		/// </summary>
		/// <param name="word"></param>
		/// <returns></returns>
		public static short SwapWord(short word) { return unchecked((short)SwapUWord( (ushort)word )); }

		/// <summary>
		/// Replaces 2 bytes in an array with a short integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, short value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 2; x++, i++)
				data[x] = b[i];
		}
		#endregion

		#region ushort
		/// <summary>
		/// Swaps a unsigned short integer by reference
		/// </summary>
		/// <param name="word"></param>
		public static void SwapUWord(ref ushort word) { word = unchecked( (ushort)((word << 8) | (word >> 8)) ); }

		/// <summary>
		/// Swaps a unsigned short integer and returns the result
		/// </summary>
		/// <param name="word"></param>
		/// <returns></returns>
		public static ushort SwapUWord(ushort word) { return unchecked( (ushort)((word << 8) | (word >> 8)) ); }

		/// <summary>
		/// Replaces 2 bytes in an array with a unsigned short integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, ushort value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 2; x++, i++)
				data[x] = b[i];
		}
		#endregion

		#region int
		/// <summary>
		/// Swaps a long integer at a position in a bye array
		/// </summary>
		/// <param name="data">source array</param>
		/// <param name="offset">offset to perform swap</param>
		public static void SwapDWord(byte[] data, int offset)
		{
			byte left_hi = data[offset];
			byte left_low = data[offset + 1];

			byte right_hi = data[offset + 2];
			byte right_low = data[offset + 3];

			data[offset] = right_low;
			data[offset + 1] = right_hi;

			data[offset + 2] = left_low;
			data[offset + 3] = left_hi;
		}

		/// <summary>
		/// Swaps a long integer by reference
		/// </summary>
		/// <param name="dword"></param>
		public static void SwapDWord(ref int dword) { dword = (int)SwapDWord( dword ); }
		//public static void SwapDWord(ref int dword) { dword = (int)((dword << 24) & 0xff000000) | ((dword << 8) & 0x00ff0000) | ((dword >> 8) & 0x0000ff00) | ((dword >> 24) & 0x000000ff); }

		/// <summary>
		/// Swaps a long integer and returns the result
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static int SwapDWord(int value) { return unchecked((int)SwapUDWord( (uint)value )); }
		//public static int SwapDWord(int value) { return (int)((value << 24) & 0xff000000) | ((value << 8) & 0x00ff0000) | ((value >> 8) & 0x0000ff00) | ((value >> 24) & 0x000000ff); }

		/// <summary>
		/// Replaces 4 bytes in an array with a long integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, int value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 4; x++, i++)
				data[x] = b[i];
		}
		#endregion

		#region uint
		/// <summary>
		/// Swaps a unsigned long integer by reference
		/// </summary>
		/// <param name="dword"></param>
		public static void SwapUDWord(ref uint dword) { dword = unchecked( ((dword << 24) & 0xff000000) | ((dword << 8) & 0x00ff0000) | ((dword >> 8) & 0x0000ff00) | ((dword >> 24) & 0x000000ff) ); }

		/// <summary>
		/// Swaps a unsigned long integer and returns the value
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static uint SwapUDWord(uint value) { return unchecked( ((value << 24) & 0xff000000) | ((value << 8) & 0x00ff0000) | ((value >> 8) & 0x0000ff00) | ((value >> 24) & 0x000000ff) ); }

		/// <summary>
		/// Replaces 4 bytes in an array with a unsigned long integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, uint value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 4; x++, i++)
				data[x] = b[i];
		}

		public static void SwapUDWordAndWrite(uint value, System.IO.MemoryStream ms)
		{
			value = SwapUDWord(value);
			ms.WriteByte((byte)((value & 0x000000FF) >>  0));
			ms.WriteByte((byte)((value & 0x0000FF00) >>  8));
			ms.WriteByte((byte)((value & 0x00FF0000) >> 16));
			ms.WriteByte((byte)((value & 0xFF000000) >> 24));
		}
		public static void SwapUDWordAndInsert(uint value, byte[] data, int offset)
		{
			value = SwapUDWord(value);
			data[offset+0] = (byte)((value & 0x000000FF) >>  0);
			data[offset+1] = (byte)((value & 0x0000FF00) >>  8);
			data[offset+2] = (byte)((value & 0x00FF0000) >> 16);
			data[offset+3] = (byte)((value & 0xFF000000) >> 24);
		}
		#endregion

		#region long
		/// <summary>
		/// Swaps a 64bit integer at a position in a bye array
		/// </summary>
		/// <param name="data">source array</param>
		/// <param name="offset">offset to perform swap</param>
		public static void SwapQWord(byte[] data, int offset)
		{
			// TODO: do it
// 			byte left_hi = data[offset];
// 			byte left_low = data[offset + 1];
// 
// 			byte right_hi = data[offset + 2];
// 			byte right_low = data[offset + 3];
// 
// 			data[offset] = right_low;
// 			data[offset + 1] = right_hi;
// 
// 			data[offset + 2] = left_low;
// 			data[offset + 3] = left_hi;
		}

		/// <summary>
		/// Swaps a 64bit integer by reference
		/// </summary>
		/// <param name="dword"></param>
		public static void SwapQWord(ref long dword) { dword = (long)SwapQWord(dword); }

		/// <summary>
		/// Swaps a 64bit integer and returns the result
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static long SwapQWord(long value) { return unchecked((long)SwapUQWord((ulong)value)); }

		/// <summary>
		/// Replaces 8 bytes in an array with a 64bit integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, long value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 8; x++, i++)
				data[x] = b[i];
		}
		#endregion

		#region ulong
		/// <summary>
		/// Swaps a unsigned long integer by reference
		/// </summary>
		/// <param name="qword"></param>
		public static void SwapUQWord(ref ulong qword)
		{
			qword = unchecked(
				( (qword >> 48) & 0x00000000000000FFUL ) |
				( (qword >> 32) & 0x000000000000FF00UL ) |
				( (qword << 24) & 0x0000000000FF0000UL ) |
				( (qword << 8 ) & 0x00000000FF000000UL ) |
				( (qword >> 8 ) & 0x000000FF00000000UL ) |
				( (qword >> 24) & 0x0000FF0000000000UL ) |
				( (qword << 32) & 0x00FF000000000000UL ) |
				( (qword << 48) & 0xFF00000000000000UL ) );
		}

		/// <summary>
		/// Swaps a unsigned long integer and returns the value
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static ulong SwapUQWord(ulong value)
		{
			return unchecked(
				( (value >> 48) & 0x00000000000000FFUL ) |
				( (value >> 32) & 0x000000000000FF00UL ) |
				( (value << 24) & 0x0000000000FF0000UL ) |
				( (value << 8 ) & 0x00000000FF000000UL ) |
				( (value >> 8 ) & 0x000000FF00000000UL ) |
				( (value >> 24) & 0x0000FF0000000000UL ) |
				( (value << 32) & 0x00FF000000000000UL ) |
				( (value << 48) & 0xFF00000000000000UL ) );
		}

		/// <summary>
		/// Replaces 4 bytes in an array with a unsigned long integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, ulong value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 8; x++, i++)
				data[x] = b[i];
		}
		#endregion

		#region float
		/// <summary>
		/// Swaps a real integer by reference
		/// </summary>
		/// <param name="dword"></param>
		public static void SwapFloat(ref float dword)
		{
// #if NO_UNSAFE
// 			uint val = BitConverter.ToUInt32(BitConverter.GetBytes(dword), 0);
// 			dword = BitConverter.ToSingle(BitConverter.GetBytes(SwapUDWord(val)), 0);
// #else
// 			unsafe
// 			{
// 				float value = dword; // have to do this due to dword being 'ref'
// 				uint val = *((uint*)&value);
// 				SwapUDWord(ref val);
// 				dword = *((float*)&val);
// 			}
// #endif
			LowLevel.ByteSwap.SwapSingle(ref dword);
		}

		/// <summary>
		/// Swaps a real integer and returns the value
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static float SwapFloat(float value)
		{
// #if NO_UNSAFE
// 			uint val = BitConverter.ToUInt32(BitConverter.GetBytes(value), 0);
// 			return BitConverter.ToSingle(BitConverter.GetBytes(SwapUDWord(val)), 0);
// 
// #else
// 			unsafe
// 			{
// 				uint val = *((uint*)&value);
// 				val = SwapUDWord(val);
// 				return *((float*)&val);
// 			}
// #endif
			return LowLevel.ByteSwap.SwapSingle(value);
		}

		/// <summary>
		/// Replaces 4 bytes in an array with a real integer value
		/// </summary>
		/// <param name="data"></param>
		/// <param name="offset"></param>
		/// <param name="value"></param>
		public static void ReplaceBytes(byte[] data, int offset, float value)
		{
			byte[] b = BitConverter.GetBytes(value);
			for (int x = offset, i = 0; x < offset + 4; x++, i++)
				data[x] = b[i];
		}
		#endregion
	};
}