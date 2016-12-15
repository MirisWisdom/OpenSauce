/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using Interop = System.Runtime.InteropServices;

namespace BlamLib.Blam//KSoft.Data
{
	[Interop.StructLayout(Interop.LayoutKind.Explicit, Size = StringIdDesc.kSizeOf)]
	public struct StringIdDesc : 
		IEquatable<StringIdDesc>, IEqualityComparer<StringIdDesc>
	{
		public const int kSizeOf = sizeof(uint);

		public static StringIdDesc Null = new StringIdDesc { mHandle = 0 };

		public static readonly IEqualityComparer<StringIdDesc> kEqualityComparer = Null;

		#region GenerateIdMethod
		/// <summary>Extra method to use when generating an ID linked to a string</summary>
		/// <remarks>Index is always included in ID generation</remarks>
		[Flags]
		public enum GenerateIdMethod
		{
			/// <summary>Use string length as part of the ID</summary>
			ByLength = 1 << 0,
			/// <summary>Use set identifiers as part of the ID</summary>
			BySet = 1 << 1,

			/// <remarks>2 bits</remarks>
			//[Obsolete(EnumBitEncoderBase.kObsoleteMsg, true)]
			kAll = ByLength | BySet,
		};
		#endregion

		#region Constants
		const int kBitCountIndexBits =					Bits.kInt32BitShift; // <= 32
		static readonly int kMaxIndexBits = (int)		Bits.BitCountToMask32(kBitCountIndexBits) - 1;
		const int kBitCountSetBits =					Bits.kByteBitShift + 1; // <= 8
		static readonly int kMaxSetBits = (int)			Bits.BitCountToMask32(kBitCountSetBits) - 1;
		const int kBitCountLengthBits =					Bits.kByteBitShift + 1; // <= 8
		static readonly int kMaxLengthBits = (int)		Bits.BitCountToMask32(kBitCountLengthBits) - 1;

		/// <remarks>ONLY PUBLIC FOR USE IN CODE CONTRACTS</remarks>
		//[Contracts.Pure]
		public static bool ValidateIndexBitCount(int index)		{ return index >= 0 && index <= kMaxIndexBits; }
		/// <remarks>ONLY PUBLIC FOR USE IN CODE CONTRACTS</remarks>
		//[Contracts.Pure]
		public static bool ValidateSetBitCount(int set)			{ return set >= 0 && set <= kMaxSetBits; }
		/// <remarks>ONLY PUBLIC FOR USE IN CODE CONTRACTS</remarks>
		//[Contracts.Pure]
		public static bool ValidateLengthBitCount(int length)	{ return length >= 0 && length <= kMaxLengthBits; }

		// Bit index for data that represents a StringId (eg, length)
		const int kBitCountDataBitIndex =				Bits.kInt32BitShift; // < 32

		const int kIndexShift =							0;
		static readonly uint kIndexMask =				Bits.BitCountToMask32(kBitCountIndexBits);

		const int kSetShift =							kIndexShift + kBitCountIndexBits;
		static readonly uint kSetMask =					Bits.BitCountToMask32(kBitCountSetBits);

		const int kLengthShift =						kSetShift + kBitCountSetBits;
		static readonly uint kLengthMask =				Bits.BitCountToMask32(kBitCountLengthBits);


		const int kSidIndexBitIndexShift =				kLengthShift + kBitCountLengthBits;
		static readonly uint kSidIndexBitIndexMask =	Bits.BitCountToMask32(kBitCountDataBitIndex);

		const int kSidSetBitIndexShift =				kSidIndexBitIndexShift + kBitCountDataBitIndex;
		static readonly uint kSidSetBitIndexMask =		Bits.BitCountToMask32(kBitCountDataBitIndex);

		const int kSidLengthBitIndexShift =				kSidSetBitIndexShift + kBitCountDataBitIndex;
		static readonly uint kSidLengthBitIndexMask =	Bits.BitCountToMask32(kBitCountDataBitIndex);

		// Comes last as we let the Enum's BitEncoder take care of bit count and mask calculations
		const int kIdMethodShift =						kSidLengthBitIndexShift + kBitCountDataBitIndex;
		#endregion

		#region Internal Value
		[Interop.FieldOffset(0)] uint mHandle;

		void Initialize(GenerateIdMethod method, int index_bc, int set_bc, int length_bc)
		{
			var encoder = new HandleBitEncoder();
			encoder.Encode32((uint)index_bc, kIndexMask);
			encoder.Encode32((uint)set_bc, kSetMask);
			encoder.Encode32((uint)length_bc, kLengthMask);
			{
				uint index = 0;
				encoder.Encode32(index, kSidIndexBitIndexMask);		index += (uint)index_bc;
				encoder.Encode32(index, kSidSetBitIndexMask);		index += (uint)set_bc;
				encoder.Encode32(index, kSidLengthBitIndexMask);
			}
			encoder.Encode32(method, TypeExtensions.BitEncoders.GenerateIdMethod);
			
			mHandle = encoder.GetHandle32();
		}
		#endregion

		public StringIdDesc(GenerateIdMethod method, int index_bit_count, int set_bit_count, int length_bit_count)
		{
			//Contract.Requires(ValidateIndexBitCount(index_bit_count));
			//Contract.Requires(ValidateSetBitCount(set_bit_count));
			//Contract.Requires(ValidateLengthBitCount(length_bit_count));

			mHandle = 0; // satisfy the compiler, this must come first so we don't fuck up the values

			Initialize(method, index_bit_count, set_bit_count, length_bit_count);
		}

		public int IndexBitCount		{ get { return (int)Bits.BitDecode32(mHandle, kIndexShift, kIndexMask); } }
		public int SetBitCount			{ get { return (int)Bits.BitDecode32(mHandle, kSetShift, kSetMask); } }
		public int LengthBitCount		{ get { return (int)Bits.BitDecode32(mHandle, kLengthShift, kLengthMask); } }
		public GenerateIdMethod Method	{ get { return TypeExtensions.BitEncoders.GenerateIdMethod.BitDecode(mHandle, kIdMethodShift); } }

		#region Internal util
		internal uint IndexBitMask	{ get { return Bits.BitCountToMask32(IndexBitCount); } }
		internal uint SetBitMask	{ get { return Bits.BitCountToMask32(SetBitCount); } }
		internal uint LengthBitMask	{ get { return Bits.BitCountToMask32(LengthBitCount); } }

		internal int IndexBitIndex	{ get { return (int)Bits.BitDecode32(mHandle, kSidIndexBitIndexShift, kSidIndexBitIndexMask); } }
		internal int SetBitIndex	{ get { return (int)Bits.BitDecode32(mHandle, kSidSetBitIndexShift, kSidSetBitIndexMask); } }
		internal int LengthBitIndex	{ get { return (int)Bits.BitDecode32(mHandle, kSidLengthBitIndexShift, kSidLengthBitIndexMask); } }

		//[Contracts.Pure]
		internal bool ValidateIndex(int index)		{ return index >= 0 && index < (1 << IndexBitCount); }
		//[Contracts.Pure]
		internal bool ValidateSet(int set)			{ return set >= 0 && set < (1 << SetBitCount); }
		//[Contracts.Pure]
		internal bool ValidateLength(int length)	{ return length >= 0 && length < (1 << LengthBitCount); }

		/// <summary>Size needed to store a string id value in a character buffer</summary>
		internal int ValueBufferSize { get { return 1<<LengthBitCount; } }
		#endregion

		#region Overrides
		public override bool Equals(object obj)
		{
			if (obj is StringIdDesc)
				return this.mHandle == ((StringIdDesc)obj).mHandle;

			return false;
		}
		public override int GetHashCode() { return (int)mHandle; }
		#endregion

		#region ITagElementTextStreamable Members
		public static void Serialize(IO.XmlStream s/*, System.IO.FileAccess stream_mode*/, ref StringIdDesc desc)
		{
			int index_bc = desc.IndexBitCount;
			int set_bc = desc.SetBitCount;
			int length_bc = desc.LengthBitCount;
			GenerateIdMethod method = desc.Method;

			s.ReadAttribute("method", ref method);
			s.ReadAttribute("indexBitCount", 10, ref index_bc);
			s.ReadAttribute("setBitCount", 10, ref set_bc);
			s.ReadAttribute("lengthBitCount", 10, ref length_bc);

			//if (stream_mode == System.IO.FileAccess.Read)
			{
				//Contract.Assert(ValidateIndexBitCount(index_bc));
				//Contract.Assert(ValidateSetBitCount(set_bc));
				//Contract.Assert(ValidateLengthBitCount(length_bc));

				desc = new StringIdDesc(method, index_bc, set_bc, length_bc);
			}
		}
		#endregion

		#region Equality Members
		public bool Equals(StringIdDesc x, StringIdDesc y)	{ return x.mHandle == y.mHandle; }

		public bool Equals(StringIdDesc other)			{ return Equals(this, other); }

		public int GetHashCode(StringIdDesc obj)		{ return obj.GetHashCode(); }
		#endregion

		//[Contracts.Pure]
		public StringId Generate(int index, int length, int set)
		{
			var method = Method;

			if (!method.HasFlag(GenerateIdMethod.ByLength))
				length = 0;
			if (!method.HasFlag(GenerateIdMethod.BySet))
				set = 0;

			return new StringId(this, index, length, set);
		}
	};
}