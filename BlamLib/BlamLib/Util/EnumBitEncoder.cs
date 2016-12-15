using System;
using System.Reflection;
//using Contracts = System.Diagnostics.Contracts;
//using Contract = System.Diagnostics.Contracts.Contract;

namespace BlamLib//KSoft
{
	/// <summary>
	/// Apply to enumerations which are invalid with the <see cref="EnumBitEncoderBase">EnumBitEncoder</see> classes
	/// </summary>
	[AttributeUsage(AttributeTargets.Enum, AllowMultiple=false)]
	public sealed class EnumBitEncoderDisableAttribute : Attribute	{};

	/// <summary>Nothing public to see here, move along.</summary>
	public abstract class EnumBitEncoderBase
	{
		/// <summary>
		/// Applied to enumeration members <b>kMax</b> and <b>kAll</b> which aren't meant to be used in operational code
		/// </summary>
		public const string kObsoleteMsg = "For 'KSoft.Util.EnumBitEncoderBase' use only!";

		protected const string kEnumMaxMemberName = "kMax";
		protected const string kFlagsMaxMemberName = "kAll";

		protected static bool ValidateTypeIsNotEncoderDisabled(Type t)
		{
			var attr = t.GetCustomAttributes(typeof(EnumBitEncoderDisableAttribute), false);

			return attr.Length == 0;
		}

		protected static void InitializeBase(Type t)
		{
			Reflection.EnumUtils.AssertTypeIsEnum(t);

			if (!ValidateTypeIsNotEncoderDisabled(t))
				throw new ArgumentException(string.Format(
					"EnumBitEncoder can't operate on enumerations with an EnumBitEncoderDisableAttribute! {0}",
					t.FullName));
		}

		[System.Diagnostics.Conditional("TRACE")]
		protected static void ProcessMembers_DebugCheckMemberName(Type t, bool kIsFlags, string member_name)
		{
			if (kIsFlags && member_name == kEnumMaxMemberName)
				System.Diagnostics.Trace.Assert(false);//Debug.Trace.Util.TraceInformation("Flags enum '{0}' has the Enum EnumBitEncoder member. Is this intentional?", t);
			else if (!kIsFlags && member_name == kFlagsMaxMemberName)
				System.Diagnostics.Trace.Assert(false);//Debug.Trace.Util.TraceInformation("Enum '{0}' has the Flags EnumBitEncoder member. Is this intentional?", t);
		}
	};

	/// <summary>Utility class for encoding Enumerations into an integer's bits.</summary>
	/// <typeparam name="TEnum"></typeparam>
	/// <remarks>
	/// Regular Enumerations should have a member called <b>kMax</b>. This value
	/// must be the highest value and shouldn't actually be used.
	/// If <b>kMax</b> doesn't exist, the highest value found, plus 1, is used as 
	/// the assumed <b>kMax</b>
	/// 
	/// <see cref="FlagsAttribute"/> Enumerations should have a member called 
	/// <b>kAll</b>. This value must be equal to all the usable bits in the type. 
	/// If you want to leave a certain bit or bits out of the encoder, don't include 
	/// them in <b>kAll</b>'s value.
	/// If <b>kAll</b> doesn't exist, ALL members are OR'd together to create the 
	/// assumed <b>kAll</b> value.
	/// 
	/// NOTE: Might end up turning this into a struct instead
	/// </remarks>
	public sealed class EnumBitEncoder32<TEnum> : EnumBitEncoderBase where TEnum : struct
	{
		/// <summary>The underlying integer type of the enumeration</summary>
		static readonly TypeCode kUnderlyingType;
		/// <summary>Does the underlying enumeration have a <see cref="FlagsAttribute"/>?</summary>
		static readonly bool kIsFlags;
		/// <summary>
		/// The <see cref="kEnumMaxMemberName"/>\<see cref="kFlagsMaxMemberName"/> 
		/// value or the member value whom this class assumed would be the max
		/// </summary>
		static readonly uint kMaxValue;
		/// <summary>Masking value that can be used to single out this enumeration's value(s)</summary>
		public static readonly uint kBitmask;
		/// <summary>How many bits the enumeration consumes</summary>
		public static readonly int kBitCount;

		#region Static Initialize
		static void ProcessMembers(Type t, out uint max_value)
		{
			max_value = uint.MaxValue;
			var mvalues = System.Enum.GetValues(t);
			var mnames = System.Enum.GetNames(t);;

			#region is_type_signed
			Func<bool> func_is_type_signed = delegate()
			{
				switch (kUnderlyingType)
				{
					case TypeCode.SByte:
					case TypeCode.Int16:
					case TypeCode.Int32:
					/*case TypeCode.Int64:*/ return true;
					default: return false;
				}
			};
			bool is_type_signed = func_is_type_signed();
			#endregion

			uint greatest = 0, temp;
			for (int x = 0; x < mvalues.Length; x++)
			{
				// Validate members when the underlying type is signed
				if (!kIsFlags && is_type_signed && Convert.ToInt32(mvalues.GetValue(x)) < 0)
					throw new ArgumentOutOfRangeException("TEnum",
						string.Format("{0}:{1} is invalid (negative)!", t.FullName, mnames[x]));

				temp = Convert.ToUInt32(mvalues.GetValue(x));
				// Base max_value off the predetermined member name first
				if ((!kIsFlags && mnames[x] == kEnumMaxMemberName) ||	// Enum case
					(kIsFlags && mnames[x] == kFlagsMaxMemberName))		// Flags case
				{
					max_value = greatest = temp;
					break;
				}
				// Record the greatest value thus far in case the above doesn't exist
				else
				{
					if(!kIsFlags)
						greatest = System.Math.Max(greatest, temp);
					else
						greatest |= temp; // just add all the flag values together
				}

				ProcessMembers_DebugCheckMemberName(t, kIsFlags, mnames[x]);
			}

			// If the Enum doesn't have a member named k*MaxMemberName, use the assumed max value
			if (max_value == uint.MaxValue && greatest != uint.MaxValue) // just in case k*MaxMemberName actually equaled uint.MaxValue
			{
				max_value = greatest;

				// NOTE: we add +1 because the [Bits.GetBitmaskEnum32] method assumes the parameter 
				// isn't a real member of the enumeration. We didn't find a k*MaxMemberName so we 
				// fake it
				if (!kIsFlags)
					max_value += 1;
			}
		}

		static EnumBitEncoder32()
		{
			Type t = typeof(TEnum);
			InitializeBase(t);

			kUnderlyingType = Type.GetTypeCode(System.Enum.GetUnderlyingType(t));

			kIsFlags = t.GetCustomAttributes(typeof(FlagsAttribute), false).Length > 0;

			ProcessMembers(t, out kMaxValue);
			kBitmask = kIsFlags ? kMaxValue : Bits.GetBitmaskEnum32(kMaxValue);
			kBitCount = Bits.BitCount(kBitmask);
		}
		#endregion


		#region DefaultBitIndex
		readonly int mDefaultBitIndex;
		/// <summary>The bit index assumed when one isn't provided</summary>
		public int DefaultBitIndex { get { return mDefaultBitIndex; } }
		#endregion

		public EnumBitEncoder32() : this(0) {}
		public EnumBitEncoder32(int default_bit_index)
		{
			//Contract.Requires(default_bit_index >= 0);

			mDefaultBitIndex = default_bit_index;
		}

		#region Encode
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		/// <remarks>Uses <see cref="DefaultBitIndex"/> as the bit index to start encoding at</remarks>
		//[Contracts.Pure]
		public uint BitEncode(TEnum value, uint bits) { return BitEncode(value, bits, mDefaultBitIndex); }
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <returns><paramref name="bits"/> with <paramref name="value"/> encoded into it</returns>
		//[Contracts.Pure]
		public uint BitEncode(TEnum value, uint bits, int bit_index)
		{
			//Contract.Requires(bit_index >= 0);
			//Contract.Requires(bit_index < Bits.kInt32BitCount);

			uint v = Reflection.EnumValue<TEnum>.ToUInt32(value);

			return kIsFlags ?
				Bits.BitEncodeFlags32(v, bits, bit_index, kBitmask) :
				Bits.BitEncodeEnum32(v, bits, bit_index, kBitmask);
		}
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <remarks>
		/// On return <paramref name="bits"/> has <paramref name="value"/> encoded into it and 
		/// <paramref name="bit_index"/> is incremented by the bit count of the underlying enumeration
		/// </remarks>
		//[Contracts.Pure]
		public void BitEncode(TEnum value, ref uint bits, ref int bit_index)
		{
			//Contract.Requires(bit_index >= 0);
			//Contract.Requires(bit_index < Bits.kInt32BitCount);
			//Contract.Requires((bit_index+kBitCount) < Bits.kInt32BitCount);

			uint v = Reflection.EnumValue<TEnum>.ToUInt32(value);

			bits = kIsFlags ?
				Bits.BitEncodeFlags32(v, bits, bit_index, kBitmask) : 
				Bits.BitEncodeEnum32(v, bits, bit_index, kBitmask);

			bit_index += kBitCount;
		}

		// Only added this really to ease the coding of HandleBitEncoder
		/// <summary>Bit encode an enumeration value into an unsigned integer</summary>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="bits">Bit data as an unsigned integer</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start encoding at</param>
		/// <remarks>
		/// On return <paramref name="bits"/> has <paramref name="value"/> encoded into it and 
		/// <paramref name="bit_index"/> is incremented by the bit count of the underlying enumeration
		/// </remarks>
		//[Contracts.Pure]
		public void BitEncode(TEnum value, ref ulong bits, ref int bit_index)
		{
			//Contract.Requires(bit_index >= 0);
			//Contract.Requires(bit_index < Bits.kInt64BitCount);
			//Contract.Requires((bit_index+kBitCount) < Bits.kInt64BitCount);

			ulong v = Reflection.EnumValue<TEnum>.ToUInt32(value);

			bits = kIsFlags ?
				Bits.BitEncodeFlags64(v, bits, bit_index, kBitmask) : 
				Bits.BitEncodeEnum64(v, bits, bit_index, kBitmask);

			bit_index += kBitCount;
		}
		#endregion

		#region Decode
		/// <summary>Bit decode an enumeration value from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from<</param>
		/// <returns>The enumeration value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		/// <remarks>Uses <see cref="DefaultBitIndex"/> as the bit index to start decoding at</remarks>
		//[Contracts.Pure]
		public TEnum BitDecode(uint bits) { return BitDecode(bits, mDefaultBitIndex); }
		/// <summary>Bit decode an enumeration value from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <returns>The enumeration value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		//[Contracts.Pure]
		public TEnum BitDecode(uint bits, int bit_index)
		{
			//Contract.Requires(bit_index >= 0);
			//Contract.Requires(bit_index < Bits.kInt32BitCount);

			uint v = Bits.BitDecode32(bits, bit_index, kBitmask);

			return Reflection.EnumValue<TEnum>.FromUInt32(v);
		}
		/// <summary>Bit decode an enumeration value from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <returns>The enumeration value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		/// <remarks>
		/// <paramref name="bit_index"/> is incremented by the bit count of the underlying enumeration
		/// </remarks>
		//[Contracts.Pure]
		public TEnum BitDecode(uint bits, ref int bit_index)
		{
			//Contract.Requires(bit_index >= 0);
			//Contract.Requires(bit_index < Bits.kInt32BitCount);
			//Contract.Requires((bit_index + kBitCount) < Bits.kInt32BitCount);

			uint v = Bits.BitDecode32(bits, bit_index, kBitmask);

			bit_index += kBitCount;

			return Reflection.EnumValue<TEnum>.FromUInt32(v);
		}

		// Only added this really to ease the coding of HandleBitEncoder
		/// <summary>Bit decode an enumeration value from an unsigned integer</summary>
		/// <param name="bits">Unsigned integer to decode from</param>
		/// <param name="bit_index">Index in <paramref name="bits"/> to start decoding at</param>
		/// <returns>The enumeration value as it stood before it was ever encoded into <paramref name="bits"/></returns>
		/// <remarks>
		/// <paramref name="bit_index"/> is incremented by the bit count of the underlying enumeration
		/// </remarks>
		//[Contracts.Pure]
		public TEnum BitDecode(ulong bits, ref int bit_index)
		{
			//Contract.Requires(bit_index >= 0);
			//Contract.Requires(bit_index < Bits.kInt64BitCount);
			//Contract.Requires((bit_index + kBitCount) < Bits.kInt64BitCount);

			ulong v = Bits.BitDecode64(bits, bit_index, kBitmask);

			bit_index += kBitCount;

			return Reflection.EnumValue<TEnum>.FromUInt64(v);
		}
		#endregion

		#region Endian Streaming
		/// <summary>Read a <typeparamref name="TEnum"/> value from a stream</summary>
		/// <param name="s">Stream to read from</param>
		/// <param name="value">Enum value read from the stream</param>
		/// <remarks>
		/// Uses <typeparamref name="TEnum"/>'s underlying <see cref="TypeCode"/> to 
		/// decide how big of a numeric type to read from the stream.
		/// </remarks>
		public static void Read(IO.EndianReader s, out TEnum value)
		{
			//Contract.Requires(s != null);

			uint stream_value;
			switch(kUnderlyingType)
			{
				case TypeCode.Byte:
				case TypeCode.SByte: stream_value = s.ReadByte();
					break;
				case TypeCode.Int16:
				case TypeCode.UInt16: stream_value = s.ReadUInt16();
					break;
				case TypeCode.Int32:
				case TypeCode.UInt32: stream_value = s.ReadUInt32();
					break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			value = Reflection.EnumValue<TEnum>.FromUInt32(stream_value);
		}
		/// <summary>Write a <typeparamref name="TEnum"/> value to a stream</summary>
		/// <param name="s">Stream to write to</param>
		/// <param name="value">Value to write to the stream</param>
		/// <remarks>
		/// Uses <typeparamref name="TEnum"/>'s underlying <see cref="TypeCode"/> to 
		/// decide how big of a numeric type to write to the stream.
		/// </remarks>
		public static void Write(IO.EndianWriter s, TEnum value)
		{
			//Contract.Requires(s != null);

			uint stream_value = Reflection.EnumValue<TEnum>.ToUInt32(value);
			switch (kUnderlyingType)
			{
				case TypeCode.Byte:
				case TypeCode.SByte: s.Write((byte)stream_value);
					break;
				case TypeCode.Int16:
				case TypeCode.UInt16: s.Write((ushort)stream_value);
					break;
				case TypeCode.Int32:
				case TypeCode.UInt32: s.Write(stream_value);
					break;

				default: throw new Debug.Exceptions.UnreachableException();
			}
		}
		#endregion
	};
}