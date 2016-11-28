/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
//using Contracts = System.Diagnostics.Contracts;
//using Contract = System.Diagnostics.Contracts.Contract;
using Interop = System.Runtime.InteropServices;

namespace BlamLib.Blam
{
	using GenerateIdMethod = StringIdDesc.GenerateIdMethod;

	/// <summary>Constant identifier for a code string</summary>
	[Interop.StructLayout(Interop.LayoutKind.Explicit, Size=StringId.kSizeOf)]
	//[System.ComponentModel.TypeConverter(typeof(StringIdConverter))]
	public struct StringId : //IO.IStreamable,//IO.IEndianStreamable,
		IComparer<StringId>, IComparable<StringId>,
		System.Collections.IComparer, IComparable,
		IEquatable<StringId>, IEqualityComparer<StringId>
	{
		public const int kSizeOf = sizeof(uint) + StringIdDesc.kSizeOf;

		#region Fields
		/// <summary>Id as a 32-bit integer</summary>
		[Interop.FieldOffset(0)] uint mHandle;

		/// <summary>Index of the string value</summary>
		public int Index	{ get { return (int)Bits.BitDecode32(mHandle, mDesc.IndexBitIndex, mDesc.IndexBitMask); } }
		/// <summary>String id group index</summary>
		public int Set		{ get { return (int)Bits.BitDecode32(mHandle, mDesc.SetBitIndex, mDesc.SetBitMask); } }
		/// <summary>Length of the string value</summary>
		/// <remarks>Not used in release platforms</remarks>
		public int Length	{ get { return (int)Bits.BitDecode32(mHandle, mDesc.LengthBitIndex, mDesc.LengthBitMask); } }

		[Interop.FieldOffset(4)] StringIdDesc mDesc;
		internal StringIdDesc Description { get { return mDesc; } }

		void Initialize(int index, int length, int set)
		{
			var encoder = new HandleBitEncoder();
			encoder.Encode32((uint)index, mDesc.IndexBitMask);
			encoder.Encode32((uint)set, mDesc.SetBitMask);
			encoder.Encode32((uint)length, mDesc.LengthBitMask);

			mHandle = encoder.GetHandle32();
		}
		#endregion

		#region Ctor
		/// <summary>Create a string id based on a full handle</summary>
		/// <param name="handle"></param>
		internal StringId(StringIdDesc desc, uint handle)
		{
			mHandle = handle;
			mDesc = desc;
		}

		/// <summary>Create a string id based on a index, string length and a id group</summary>
		/// <param name="index">String index</param>
		/// <param name="length">String length</param>
		/// <param name="set">The id group</param>
		internal StringId(StringIdDesc desc, int index, int length, int set)
		{
			//Contract.Requires(desc.ValidateIndex(index));
			//Contract.Requires(desc.ValidateLength(length));
			//Contract.Requires(desc.ValidateSet(set));

			mHandle = 0; // satisfy the compiler, this must come first so we don't fuck up the values

			mDesc = desc;
			Initialize(index, length, set);
		}
		#endregion

		/// <summary>Null string id value</summary>
		public static readonly StringId Null = new StringId(StringIdDesc.Null, 0);
		/// <summary>Invalid string id (will never reference a valid string)</summary>
		public static readonly StringId kInvalid = new StringId(StringIdDesc.Null, uint.MaxValue);

		public static readonly IEqualityComparer<StringId> kEqualityComparer = Null;

		/// <summary>Sanity checks this string id's values</summary>
		//[Contracts.Pure]
		public bool IsValid { get {
			if (Set == 0 && Index == 0) // if the index is 0, its pointing to the null string id and the entire value must be 0
				return mHandle == 0;

			return !mDesc.Equals(StringIdDesc.Null);
		} }

		/// <summary>Does this ID not reference anything?</summary>
		/// <remarks>Checks this value against <see cref="StringId.kInvalid"/></remarks>
		public bool IsInvalid { get { return this == StringId.kInvalid; } }
		/// <summary>Does this ID reference an empty string?</summary>
		/// <remarks>Checks this value against <see cref="StringId.Null"/></remarks>
		public bool IsNull { get { return this == StringId.Null; } }

		#region Overrides
		public override bool Equals(object obj)
		{
			if (obj is StringId)
				return this.mHandle == ((StringId)obj).mHandle;

			return false;
		}

		/// <summary>Returns the hash code for this instance</summary>
		/// <returns>This string id object as an int</returns>
		public override int GetHashCode() { return (int)mHandle; }

		/// <summary>Converts this instance to a string</summary>
		/// <returns>[{Set}:{Index} {Length}]</returns>
		public override string ToString() { return string.Format("[{0}:{1} {2}]", Set, Index, Length); }
		#endregion

		#region IEndianStreamable Members
		/// <summary>Stream an string id from a buffer</summary>
		/// <param name="s"></param>
		//public void Read(IO.EndianReader s)	{ mHandle = s.ReadUInt32(); }
		public void Read(IO.EndianReader s, StringIdDesc desc)
		{
			mHandle = s.ReadUInt32();
			mDesc = desc;
		}
		/// <summary>Stream an string id to a buffer</summary>
		/// <param name="s"></param>
		public void Write(IO.EndianWriter s) { s.Write(mHandle); }
		#endregion

		#region ICompare Members
		public int Compare(StringId x, StringId y)
		{
			if (x.Set == y.Set)	return x.Index - y.Index;
			else				return x.Set - y.Set;
		}

		public int CompareTo(StringId other) { return Compare(this, other); }

		int System.Collections.IComparer.Compare(object x, object y)
		{
			return Compare((StringId)x, (StringId)y);
		}

		int IComparable.CompareTo(object obj)
		{
			return Compare(this, (StringId)obj);
		}
		#endregion

		#region Equality Members
		public bool Equals(StringId x, StringId y)	{ return x.mHandle == y.mHandle; }

		public bool Equals(StringId other)			{ return Equals(this, other); }

		public int GetHashCode(StringId obj)		{ return obj.GetHashCode(); }
		#endregion

		#region Conversions
		/// <summary>Returns the string id as a uint</summary>
		/// <param name="value"></param>
		/// <returns><paramref name="value"/> as a uint</returns>
		public static implicit operator uint(StringId value) { return value.mHandle; }

		/// <summary>Returns the string id as a uint</summary>
		/// <returns></returns>
		public uint ToUInt32() { return mHandle; }
		#endregion
	};
}