//using Contracts = System.Diagnostics.Contracts;
//using Contract = System.Diagnostics.Contracts.Contract;
using Interop = System.Runtime.InteropServices;

namespace BlamLib//KSoft
{
	/// <summary>Stack friendly bit encoder for dealing with handle generation or reading</summary>
	[Interop.StructLayout(Interop.LayoutKind.Explicit, Size=HandleBitEncoder.kSizeOf)]
	public struct HandleBitEncoder
	{
		public const int kSizeOf = sizeof(ulong) + sizeof(int);

		[Interop.FieldOffset(0)] ulong m64;
		[Interop.FieldOffset(0)] uint  m32;
		[Interop.FieldOffset(8)] int   mBitIndex;

		public HandleBitEncoder(uint initial_bits)
		{
			m64 = 0;
			m32 = 0;
			mBitIndex = 0;

			m32 = initial_bits;
		}
		public HandleBitEncoder(ulong initial_bits)
		{
			m64 = 0;
			m32 = 0;
			mBitIndex = 0;

			m64 = initial_bits;
		}

		/// <summary>How many bits have actually been consumed by the handle data</summary>
		public int UsedBitCount { get { return mBitIndex; } }

		/// <summary>Get the 32-bit handle value</summary>
		/// <returns></returns>
		public uint GetHandle32() { return m32; }
		/// <summary>Get the 64-bit handle value</summary>
		/// <returns></returns>
		public ulong GetHandle64() { return m64; }
		/// <summary>Get the entire handle's value represented in 32-bits</summary>
		/// <returns></returns>
		public uint GetCombinedHandle()
		{
			uint hi = IntegerMath.GetHighBits(m64);

			// this order allows a user to XOR again with GetHandle32 to get 
			// the upper 32-bit values of m64
			return hi ^ m32;
		}


		void VerifyBitIndex(int advance_bit_count)
		{
			if (mBitIndex + advance_bit_count >= Bits.kInt64BitCount)
				throw new System.ArgumentOutOfRangeException("bit_index", mBitIndex + advance_bit_count,
					"bit_index is or will be greater than or equal to Bits.kInt64BitCount");
		}

		/// <summary>Clear the internal state of the encoder</summary>
		public void Reset()
		{
			m64 = 0;
			m32 = 0; // just to be safe
			mBitIndex = 0;
		}

		#region Encode
		/// <summary>Encode an enumeration value using an enumeration encoder object</summary>
		/// <typeparam name="TEnum">Enumeration type to encode</typeparam>
		/// <param name="value">Enumeration value to encode</param>
		/// <param name="encoder">Encoder for <typeparamref name="TEnum"/> objects</param>
		public void Encode32<TEnum>(TEnum value, EnumBitEncoder32<TEnum> encoder) where TEnum : struct
		{
			//Contract.Requires<System.ArgumentNullException>(encoder != null);

			encoder.BitEncode(value, ref m64, ref mBitIndex);
		}

		/// <summary>Bit encode a value into this handle</summary>
		/// <param name="value">Value to encode</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		public void Encode32(uint value, uint bit_mask)
		{
			//Contract.Requires<System.ArgumentException>(bit_mask != 0);

			Bits.BitEncodeEnum64(value, ref m64, ref mBitIndex, bit_mask);
		}
		/// <summary>Bit encode a value into this handle</summary>
		/// <param name="value">Value to encode</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		public void Encode64(ulong value, ulong bit_mask)
		{
			//Contract.Requires<System.ArgumentException>(bit_mask != 0);

			Bits.BitEncodeEnum64(value, ref m64, ref mBitIndex, bit_mask);
		}
		#endregion

		#region Decode
		/// <summary>Decode an enumeration value using an enumeration encoder object</summary>
		/// <typeparam name="TEnum">Enumeration type to decode</typeparam>
		/// <param name="value">Enumeration value decoded from this handle</param>
		/// <param name="decoder">Encoder for <typeparamref name="TEnum"/> objects</param>
		public void Decode32<TEnum>(out TEnum value, EnumBitEncoder32<TEnum> decoder) where TEnum : struct
		{
			//Contract.Requires<System.ArgumentNullException>(decoder != null);

			value = decoder.BitDecode(m64, ref mBitIndex);
		}

		/// <summary>Bit decode a value from this handle</summary>
		/// <param name="value">Value decoded from this handle</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		public void Decode32(out uint value, uint bit_mask)
		{
			//Contract.Requires<System.ArgumentException>(bit_mask != 0);

			value = (uint)Bits.BitDecode64(m64, ref mBitIndex, bit_mask);
		}
		/// <summary>Bit decode a value from this handle</summary>
		/// <param name="value">Value decoded from this handle</param>
		/// <param name="bit_mask">Masking value for <paramref name="value"/></param>
		public void Decode64(out ulong value, ulong bit_mask)
		{
			//Contract.Requires<System.ArgumentException>(bit_mask != 0);

			value = Bits.BitDecode64(m64, ref mBitIndex, bit_mask);
		}
		#endregion

		#region Overrides
		public override bool Equals(object obj)
		{
			if(obj is HandleBitEncoder)
			{
				var o = (HandleBitEncoder)obj;

				return mBitIndex == o.mBitIndex &&
					m64 == o.m64;
			}

			return false;
		}
		public override int GetHashCode()	{ return (int)GetCombinedHandle(); }
		/// <summary>"[{<see cref="GetHandle64()"/>} @ {CurrentBitIndex}]</summary>
		/// <returns></returns>
		/// <remarks>Handle value is formatted to a 16-character hex string</remarks>
		public override string ToString()
		{
			return string.Format("[{0} @ {1}]", m64.ToString("X16"), mBitIndex.ToString());
		}
		#endregion
	};
}