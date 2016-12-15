using System;
//using Contracts = System.Diagnostics.Contracts;
//using Contract = System.Diagnostics.Contracts.Contract;

namespace BlamLib//KSoft
{
	partial class IntegerMath
	{
		/// <summary>Convenience function for getting the high order bits (LSB) in an unsigned integer</summary>
		/// <param name="value"></param>
		/// <returns>Signed representation of the high-bits in <paramref name="value"/></returns>
		//[Contracts.Pure]
		public static int GetHighBitsSigned(uint value)	{ return (int)((value >> 16) & 0xFFFFFFFF); }
		/// <summary>Convenience function for getting the low order bits (MSB) in an unsigned integer</summary>
		/// <param name="value"></param>
		/// <returns>Signed representation of the low-bits in <paramref name="value"/></returns>
		//[Contracts.Pure]
		public static int GetLowBitsSigned(uint value)	{ return (int)(value & 0xFFFFFFFF); }

		/// <summary>Convenience function for getting the high order bits (LSB) in an unsigned integer</summary>
		/// <param name="value"></param>
		/// <returns>Unsigned representation of the high-bits in <paramref name="value"/></returns>
		//[Contracts.Pure]
		public static uint GetHighBits(ulong value)	{ return (uint)((value >> 32) & 0xFFFFFFFF); }
		/// <summary>Convenience function for getting the low order bits (MSB) in an unsigned integer</summary>
		/// <param name="value"></param>
		/// <returns>Unsigned representation of the low-bits in <paramref name="value"/></returns>
		//[Contracts.Pure]
		public static uint GetLowBits(ulong value)	{ return (uint)(value & 0xFFFFFFFF); }
	};

	/// <summary>Utility class for bit level manipulation</summary>
	// Reference: http://graphics.stanford.edu/~seander/bithacks.html
	public static class Bits
	{
		#region kBitCount
		/// <summary>Number of bits in a <see cref="System.Byte"/></summary>
		public const int kByteBitCount = sizeof(byte) * 8;
		/// <summary>Number of bits in a <see cref="System.Int16"/></summary>
		public const int kInt16BitCount = sizeof(ushort) * 8;
		/// <summary>Number of bits in a <see cref="System.Int32"/></summary>
		public const int kInt32BitCount = sizeof(uint) * 8;
		/// <summary>Number of bits in a <see cref="System.Int64"/></summary>
		public const int kInt64BitCount = sizeof(ulong) * 8;
		#endregion

		#region kBitShift
		/// <summary>Bit shift value for getting the bit count of a an <see cref="System.Byte"/> element</summary>
		public const int kByteBitShift =	3;
		/// <summary>Bit shift value for getting the bit count of a an <see cref="System.Int16"/> element</summary>
		public const int kInt16BitShift =	4;
		/// <summary>Bit shift value for getting the bit count of a an <see cref="System.Int32"/> element</summary>
		public const int kInt32BitShift =	5;
		/// <summary>Bit shift value for getting the bit count of a an <see cref="System.Int64"/> element</summary>
		public const int kInt64BitShift =	6;
		#endregion

		// look at this http://www.df.lth.se/~john_e/gems/gem002d.html
		#region BitCount
		/// <summary>Count the number of 'on' bits in an unsigned integer</summary>
		/// <param name="bits">Integer whose bits to count</param>
		/// <returns></returns>
		//[Contracts.Pure]
		public static int BitCount(byte bits)
		{
			uint x = bits;
			x = (x & 0x55) + ((x & 0xAA) >> 1);
			x = (x & 0x33) + ((x & 0xCC) >> 2);
			x = (x & 0x0F) + ((x & 0xF0) >> 4);

			return (int)x;
		}
		/// <summary>Count the number of 'on' bits in an unsigned integer</summary>
		/// <param name="bits">Integer whose bits to count</param>
		/// <returns></returns>
		//[Contracts.Pure]
		public static int BitCount(ushort bits)
		{
			uint x = bits;
			x = (x & 0x5555) + ((x & 0xAAAA) >>  1);
			x = (x & 0x3333) + ((x & 0xCCCC) >>  2);
			x = (x & 0x0F0F) + ((x & 0xF0F0) >>  4);
			x = (x & 0x00FF) + ((x & 0xFF00) >>  8);

			return (int)x;
		}
		/// <summary>Count the number of 'on' bits in an unsigned integer</summary>
		/// <param name="bits">Integer whose bits to count</param>
		/// <returns></returns>
		//[Contracts.Pure]
		public static int BitCount(uint bits)
		{
			uint x = bits;
			x = (x & 0x55555555) + ((x & 0xAAAAAAAA) >>  1);
			x = (x & 0x33333333) + ((x & 0xCCCCCCCC) >>  2);
			x = (x & 0x0F0F0F0F) + ((x & 0xF0F0F0F0) >>  4);
			x = (x & 0x00FF00FF) + ((x & 0xFF00FF00) >>  8);
			x = (x & 0x0000FFFF) + ((x & 0xFFFF0000) >> 16);

			return (int)x;
		}
		/// <summary>Count the number of 'on' bits in an unsigned integer</summary>
		/// <param name="bits">Integer whose bits to count</param>
		/// <returns></returns>
		//[Contracts.Pure]
		public static int BitCount(ulong bits)
		{
			ulong x = bits;
			x = (x & 0x5555555555555555) + ((x & 0xAAAAAAAAAAAAAAAA) >>  1);
			x = (x & 0x3333333333333333) + ((x & 0xCCCCCCCCCCCCCCCC) >>  2);
			x = (x & 0x0F0F0F0F0F0F0F0F) + ((x & 0xF0F0F0F0F0F0F0F0) >>  4);
			x = (x & 0x00FF00FF00FF00FF) + ((x & 0xFF00FF00FF00FF00) >>  8);
			x = (x & 0x0000FFFF0000FFFF) + ((x & 0xFFFF0000FFFF0000) >> 16);
			x = (x & 0x00000000FFFFFFFF) + ((x & 0xFFFFFFFF00000000) >> 32);

			return (int)x;
		}

		/// <summary>Calculate the bit-mask needed for a number of bits</summary>
		/// <param name="bit_count">Number of bits needed for the mask</param>
		/// <returns></returns>
		//[Contracts.Pure]
		public static uint BitCountToMask32(int bit_count)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_count > 0 && bit_count <= kInt32BitCount);

			return (2U << --bit_count) - 1U;
		}
		/// <summary>Calculate the bit-mask needed for a number of bits</summary>
		/// <param name="bit_count">Number of bits needed for the mask</param>
		/// <returns></returns>
		//[Contracts.Pure]
		public static ulong BitCountToMask64(int bit_count)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_count > 0 && bit_count <= kInt64BitCount);

			return (2UL << --bit_count) - 1UL;
		}
		#endregion

		#region GetBitmask
		// NOTE: A GetMaxEnumBits32 version isn't needed with the luxury of implicit casting

		/// <summary>Calculate how many bits are needed to represent the provided value</summary>
		/// <param name="max_value">An enumeration's <b>kMax</b> value</param>
		/// <returns>Number of bits needed to represent (<paramref name="max_value"/> - 1)</returns>
		/// <remarks>A <b>kMax</b> value should be unused and the last entry of an Enumeration. This is why 1 is subtracted from <paramref name="max_value"/>.</remarks>
		//[Contracts.Pure]
		public static int GetMaxEnumBits64(ulong max_value)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value > 1); // There is no point in this if '0' is the only fucking option dumb-ass
			//Contract.Ensures(Contract.Result<int>() > 0);

			if (max_value == 2)
				return 1; // chance condition that there can only be two values, represented by 0 and 1
			else if (max_value == 3)
				return 2; // login' 2 by 2 isn't the best formula
			// boolean expression covers both 4 and 5 which are the real problem children.
			// Figure I'd use this to handle the other two values (6,7) which require 3-bits, 
			// but this isn't mandatory.
			else if (max_value < 8)
				return 3;

			return Convert.ToInt32( System.Math.Ceiling(System.Math.Log(max_value-1, 2)) );
		}

		/// <summary>Calculate the masking value for an enumeration</summary>
		/// <param name="max_value">An enumeration's <b>kMax</b> value</param>
		/// <returns>The smallest bit mask value for (<paramref name="max_value"/> - 1)</returns>
		/// <remarks>A <b>kMax</b> value should be unused and the last entry of an Enumeration. This is why 1 is subtracted from <paramref name="max_value"/>.</remarks>
		//[Contracts.Pure]
		public static uint GetBitmaskEnum32(uint max_value)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value > 0);
			//Contract.Ensures(Contract.Result<uint>() > 0);

			int bit_count = GetMaxEnumBits64(max_value);

			return (1U << bit_count) - 1U;
		}
		/// <summary>Calculate the masking value for an enumeration</summary>
		/// <param name="max_value">An enumeration's <b>kMax</b> value</param>
		/// <returns>The smallest bit mask value for (<paramref name="max_value"/> - 1)</returns>
		/// <remarks>A <b>kMax</b> value should be unused and the last entry of an Enumeration. This is why 1 is subtracted from <paramref name="max_value"/>.</remarks>
		//[Contracts.Pure]
		public static ulong GetBitmaskEnum64(ulong max_value)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value > 0);
			//Contract.Ensures(Contract.Result<ulong>() > 0);

			int bit_count = GetMaxEnumBits64(max_value);

			return (1UL << bit_count) - 1UL;
		}

		/// <summary>Calculate the masking value for a series of flags</summary>
		/// <param name="max_value">An enumeration's <b>kMax</b> value</param>
		/// <returns>The smallest bit mask value for (<paramref name="max_value"/> - 1)</returns>
		/// <remarks>A <b>kMax</b> value should be unused and the last entry of an Enumeration. This is why 1 is subtracted from <paramref name="max_value"/>.</remarks>
		//[Contracts.Pure]
		public static uint GetBitmaskFlag32(uint max_value)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value > 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value <= kInt32BitCount);
			//Contract.Ensures(Contract.Result<uint>() > 0);

			return (1U << (int)--max_value) - 1U;
		}
		/// <summary>Calculate the masking value for a series of flags</summary>
		/// <param name="max_value">An enumeration's <b>kMax</b> value</param>
		/// <returns>The smallest bit mask value for (<paramref name="max_value"/> - 1)</returns>
		/// <remarks>A <b>kMax</b> value should be unused and the last entry of an Enumeration. This is why 1 is subtracted from <paramref name="max_value"/>.</remarks>
		//[Contracts.Pure]
		public static ulong GetBitmaskFlag64(uint max_value)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value > 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(max_value <= kInt64BitCount);
			//Contract.Ensures(Contract.Result<ulong>() > 0);

			return (1UL << (int)--max_value) - 1UL;
		}
		#endregion

		#region BitEncode (Enums)
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Clears the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> so 
		/// any possibly existing value will be zeroed before <paramref name="value"/> is added
		/// </remarks>
		//[Contracts.Pure]
		public static uint BitEncodeEnum32(uint value, uint bits, int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			return Util.Flags.Add(bits & ~(bit_mask << bit_index), // clear the bit-space
				(value & bit_mask) << bit_index); // add [value] to the newly cleared bit-space
		}
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Clears the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> so 
		/// any possibly existing value will be zeroed before <paramref name="value"/> is added
		/// </remarks>
		//[Contracts.Pure]
		public static ulong BitEncodeEnum64(ulong value, ulong bits, int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			return Util.Flags.Add(bits & ~(bit_mask << bit_index),// clear the bit-space
				(value & bit_mask) << bit_index); // add [value] to the newly cleared bit-space
		}

		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <remarks>
		/// On return <paramref name="bits"/> has <paramref name="value"/> encoded into it and <paramref name="bit_index"/> 
		/// is incremented by the bit count (determined from <paramref name="bit_mask"/>)
		/// </remarks>
		//[Contracts.Pure]
		public static void BitEncodeEnum32(uint value, ref uint bits, ref int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < Bits.kInt32BitCount);

			int bit_count = BitCount(bit_mask);
			//Contract.Assert((bit_index + bit_count) < Bits.kInt32BitCount);

			bits = BitEncodeEnum32(value, bits, bit_index, bit_mask);
			bit_index += bit_count;
		}
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <remarks>
		/// On return <paramref name="bits"/> has <paramref name="value"/> encoded into it and <paramref name="bit_index"/> 
		/// is incremented by the bit count (determined from <paramref name="bit_mask"/>)
		/// </remarks>
		//[Contracts.Pure]
		public static void BitEncodeEnum64(ulong value, ref ulong bits, ref int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < Bits.kInt64BitCount);

			int bit_count = BitCount(bit_mask);
			//Contract.Assert((bit_index + bit_count) < Bits.kInt64BitCount);

			bits = BitEncodeEnum64(value, bits, bit_index, bit_mask);
			bit_index += bit_count;
		}
		#endregion

		#region BitEncode (Flags)
		/// <summary>Bit encode a flags value into an unsigned integer</summary>
		/// <param name="value">Flags to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Doesn't clear the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> 
		/// so any possibly existing flags will be retained before and after <paramref name="value"/> is added
		/// </remarks>
		//[Contracts.Pure]
		public static uint BitEncodeFlags32(uint value, uint bits, int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			return Util.Flags.Add(bits,
				(value & bit_mask) << bit_index); // add [value] to the existing bits
		}
		/// <summary>Bit encode a flags value into an unsigned integer</summary>
		/// <param name="value">Flags to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Doesn't clear the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> 
		/// so any possibly existing flags will be retained before and after <paramref name="value"/> is added
		/// </remarks>
		//[Contracts.Pure]
		public static ulong BitEncodeFlags64(ulong value, ulong bits, int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			return Util.Flags.Add(bits,
				(value & bit_mask) << bit_index); // add [value] to the existing bits
		}

		/// <summary>Bit encode a flags value into an unsigned integer</summary>
		/// <param name="value">Flags to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <remarks>
		/// On return <paramref name="bits"/> has <paramref name="value"/> encoded into it and <paramref name="bit_index"/> 
		/// is incremented by the bit count (determined from <paramref name="bit_mask"/>)
		/// </remarks>
		//[Contracts.Pure]
		public static void BitEncodeFlags32(uint value, ref uint bits, ref int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < Bits.kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			int bit_count = BitCount(bit_mask);
			//Contract.Assert((bit_index + bit_count) < Bits.kInt32BitCount);

			bits = BitEncodeFlags32(value, bits, bit_index, bit_mask);
			bit_index += bit_count;
		}
		/// <summary>Bit encode a flags value into an unsigned integer</summary>
		/// <param name="value">Flags to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <remarks>
		/// On return <paramref name="bits"/> has <paramref name="value"/> encoded into it and <paramref name="bit_index"/> 
		/// is incremented by the bit count (determined from <paramref name="bit_mask"/>)
		/// </remarks>
		//[Contracts.Pure]
		public static void BitEncodeFlags64(ulong value, ref ulong bits, ref int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < Bits.kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			int bit_count = BitCount(bit_mask);
			//Contract.Assert((bit_index + bit_count) < Bits.kInt64BitCount);

			bits = BitEncodeFlags64(value, bits, bit_index, bit_mask);
			bit_index += bit_count;
		}
		#endregion

		#region BitEncode
		/// <summary>Bit encode a value into an unsigned integer, removing the original data in the value's range</summary>
		/// <param name="value">Value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Clears the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> 
		/// so any existing values will be lost after <paramref name="value"/> is added
		/// </remarks>
		//[Contracts.Pure]
		public static uint BitEncode32(uint value, uint bits, int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			// Use the bit mask's invert so we can get all of the non-value bits
			return BitEncodeFlags32(value, bits & (~bit_mask), bit_index, bit_mask);
		}
		/// <summary>Bit encode a value into an unsigned integer, removing the original data in the value's range</summary>
		/// <param name="value">Value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Clears the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> 
		/// so any existing values will be lost after <paramref name="value"/> is added
		/// </remarks>
		//[Contracts.Pure]
		public static ulong BitEncode64(ulong value, ulong bits, int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			// Use the bit mask's invert so we can get all of the non-value bits
			return BitEncodeFlags64(value, bits & (~bit_mask), bit_index, bit_mask);
		}

		/// <summary>Bit encode a value into an unsigned integer, removing the original data in the value's range</summary>
		/// <param name="value">Value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Clears the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> 
		/// so any existing values will be lost after <paramref name="value"/> is added
		/// 
		/// On return <paramref name="bit_index"/> is incremented by the bit count (determined 
		/// from <paramref name="bit_mask"/>)
		/// </remarks>
		//[Contracts.Pure]
		public static void BitEncode32(uint value, ref uint bits, ref int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			// Use the bit mask's invert so we can get all of the non-value bits
			bits &= (~bit_mask);
			BitEncodeFlags32(value, ref bits, ref bit_index, bit_mask);
		}
		/// <summary>Bit encode a value into an unsigned integer, removing the original data in the value's range</summary>
		/// <param name="value">Value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>
		/// Clears the bit-space between <paramref name="bit_index"/> + <paramref name="bit_mask"/> 
		/// so any existing values will be lost after <paramref name="value"/> is added
		/// 
		/// On return <paramref name="bit_index"/> is incremented by the bit count (determined 
		/// from <paramref name="bit_mask"/>)
		/// </remarks>
		//[Contracts.Pure]
		public static void BitEncode64(ulong value, ref ulong bits, ref int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			// Use the bit mask's invert so we can get all of the non-value bits
			bits &= (~bit_mask);
			BitEncodeFlags64(value, ref bits, ref bit_index, bit_mask);
		}
		#endregion

		#region BitDecode
		/// <summary>Bit decode an enumeration or flags from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <param name="bit_mask">Masking value for the enumeration\flags type</param>
		/// <returns>The enumeration\flags value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		//[Contracts.Pure]
		public static uint BitDecode32(uint bits, int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			return (bits >> bit_index) & bit_mask;
		}
		/// <summary>Bit decode an enumeration or flags from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <param name="bit_mask">Masking value for the enumeration\flags type</param>
		/// <returns>The enumeration\flags value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		//[Contracts.Pure]
		public static ulong BitDecode64(ulong bits, int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			return (bits >> bit_index) & bit_mask;
		}

		/// <summary>Bit decode an enumeration or flags from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <param name="bit_mask">Masking value for the enumeration\flags type</param>
		/// <returns>The enumeration\flags value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		/// <remarks>On return <paramref name="bit_index"/> is incremented by the bit count (determined from <paramref name="bit_mask"/>)</remarks>
		//[Contracts.Pure]
		public static uint BitDecode32(uint bits, ref int bit_index, uint bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt32BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			int bit_count = BitCount(bit_mask);
			//Contract.Assert((bit_index + bit_count) < Bits.kInt32BitCount);

			uint value = (bits >> bit_index) & bit_mask;
			bit_index += bit_count;

			return value;
		}
		/// <summary>Bit decode an enumeration or flags from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <param name="bit_mask">Masking value for the enumeration\flags type</param>
		/// <returns>The enumeration\flags value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		/// <remarks>On return <paramref name="bit_index"/> is incremented by the bit count (determined from <paramref name="bit_mask"/>)</remarks>
		//[Contracts.Pure]
		public static ulong BitDecode64(ulong bits, ref int bit_index, ulong bit_mask)
		{
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index >= 0);
			//Contract.Requires/*<ArgumentOutOfRangeException>*/(bit_index < kInt64BitCount);
			//Contract.Requires/*<ArgumentException>*/(bit_mask != 0);

			int bit_count = BitCount(bit_mask);
			//Contract.Assert((bit_index + bit_count) < Bits.kInt64BitCount);

			ulong value = (bits >> bit_index) & bit_mask;
			bit_index += bit_count;

			return value;
		}
		#endregion
	};
}