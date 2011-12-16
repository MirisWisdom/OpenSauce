/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Blam
{
	/// <summary>
	/// Constant identifier for a string
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Explicit, Size = 4)]
	public struct StringID : IO.IStreamable, IComparer<StringID>, System.Collections.IComparer, IComparable<StringID>, IComparable,
		IEquatable<StringID>, IEqualityComparer<StringID>
	{
		#region GenerateIdMethod
		/// <summary>Extra method to use when generating an ID linked to a string</summary>
		/// <remarks>Index is always included in ID generation</remarks>
		[Flags]
		public enum GenerateIdMethod
		{
			/// <summary>Use string length as part of the ID</summary>
			ByLength = 1<<0,
			/// <summary>Use set identifiers as part of the ID</summary>
			BySet = 1<<1,
		};
		internal static StringID Generate(GenerateIdMethod method, short index, byte length, byte set)
		{
			if (!method.HasFlag(GenerateIdMethod.ByLength))
				length = 0;
			if (!method.HasFlag(GenerateIdMethod.BySet))
				set = 0;

			return new StringID(index, length, set);
		}
		internal static Func<short, byte, byte, StringID> GetGenerateFunc(GenerateIdMethod method)
		{
			if (method.HasFlag(GenerateIdMethod.ByLength) && method.HasFlag(GenerateIdMethod.BySet))
				return (short index, byte length, byte set) => new StringID(index, length, set);
			else if (method.HasFlag(GenerateIdMethod.ByLength))
				return (short index, byte length, byte set) => new StringID(index, length, 0);
			else if (method.HasFlag(GenerateIdMethod.BySet))
				return (short index, byte length, byte set) => new StringID(index, 0, set);
			else
				throw new Debug.Exceptions.UnreachableException(method);
		}
		#endregion

		/// <summary>Maximum value for the <see cref="StringID.Length"/> value</summary>
		public const int kMaxLength = 127;
		/// <summary>Size needed to store a string id value in a character buffer</summary>
		public const int kValueBufferSize = kMaxLength + 1;

		#region Fields
		/// <summary>
		/// Id as a 32bit integer
		/// </summary>
		[System.Runtime.InteropServices.FieldOffset(0)]
		public uint Handle;

		/// <summary>
		/// Index of the string value
		/// </summary>
		[System.Runtime.InteropServices.FieldOffset(0)]
		public short Index;

		/// <summary>
		/// String id group index
		/// </summary>
		/// <remarks>Must always be 0 in Halo 2</remarks>
		[System.Runtime.InteropServices.FieldOffset(2)]
		public byte Set;

		/// <summary>
		/// Length of the string value
		/// </summary>
		/// <remarks>Must always be 0 in Halo 3</remarks>
		[System.Runtime.InteropServices.FieldOffset(3)]
		public byte Length;
		#endregion

		#region Ctor
		/// <summary>
		/// Create a string id based on a full handle
		/// </summary>
		/// <param name="handle"></param>
		internal StringID(uint handle)
		{
			Index = 0; Set = Length = 0; // satisfy the compiler, this must come first so we don't fuck up Handle

			Handle = handle;
		}

		/// <summary>
		/// Create a string id based on a index, string length and a id group 
		/// </summary>
		/// <param name="index">String index</param>
		/// <param name="length">String length</param>
		/// <param name="set">The id group</param>
		public StringID(short index, byte length, byte set)
		{
			Handle = 0; // satisfy the compiler, this must come first so we don't fuck up the values

			Index = index;
			Set = set;
			Length = length;
		}
		#endregion

		/// <summary>
		/// Null (empty) string id value
		/// </summary>
		public static readonly StringID Null = new StringID(0, 0, 0);
		/// <summary>
		/// Invalid string id (will never reference a valid string)
		/// </summary>
		public static readonly StringID kInvalid = new StringID(uint.MaxValue);
		public static readonly IEqualityComparer<StringID> kEqualityComparer = new StringID(0, 0, 0);

		/// <summary>
		/// Sanity checks this string id
		/// </summary>
		/// <returns>True if all the values are a valid range</returns>
		/// <remarks>Logic only works for Halo 2 instances</remarks>
		public bool IsValid()
		{
			if (Index == 0) // if the index is 0, its pointing to the null string id and the entire value must be 0
				return Handle == 0;

			return	Index < 0x7FFF &&
					Set == 0 &&
					Length <= kMaxLength;
		}

		/// <summary>
		/// Sanity checks this string id
		/// </summary>
		/// <returns>True if all the values are a valid range</returns>
		public bool IsValid(BlamVersion game)
		{
			if (Index == 0) // if the index is 0, its pointing to the null string id and the entire value must be 0
				return Handle == 0;

			if ((game & BlamVersion.Halo2) != 0)
				return this.IsValid();
			else if ((game & BlamVersion.Halo3) != 0)
				return  Index < 0x7FFF &&
						Length == 0;

			return false; // all other games don't have a string id
		}

		/// <summary>Does this ID not reference anything?</summary>
		/// <remarks>Checks this value against <see cref="StringID.kInvalid"/></remarks>
		public bool IsInvalid { get { return this == StringID.kInvalid; } }
		/// <summary>Does this ID reference an empty string?</summary>
		/// <remarks>Checks this value against <see cref="StringID.Null"/></remarks>
		public bool IsNull { get { return this == StringID.Null; } }

		#region Overrides
		/// <summary>
		/// Compares two StringID objects testing their
		/// <see cref="Index"/> and <see cref="Length"/> fields for
		/// equality
		/// </summary>
		/// <param name="obj">other StringID object</param>
		/// <returns>true if both this object and <paramref name="obj"/> are equal</returns>
		public override bool Equals(object obj)
		{
			StringID s = (StringID)obj;
			return s.Index == this.Index && s.Set == this.Set && s.Length == this.Length;
		}

		/// <summary>
		/// Returns the hash code for this instance
		/// </summary>
		/// <see cref="implicit operator uint(StringID)"/>
		/// <returns>This string id object as an int</returns>
		public override int GetHashCode() { return (int)Handle; }

		/// <summary>
		/// Converts this instance to a string
		/// </summary>
		/// <returns>A string with <see cref="Index"/> and <see cref="Length"/> in brakets</returns>
		public override string ToString() { return string.Format("[{0}:{1} {2}]", Set, Index, Length); }
		#endregion

		#region Conversions
		/// <summary>
		/// Returns the string id as a uint
		/// </summary>
		/// <param name="value"></param>
		/// <returns><paramref name="value"/> as a uint</returns>
		public static implicit operator uint(StringID value) { return value.Handle; }
		/// <summary>
		/// Returns the uint as a string id
		/// </summary>
		/// <param name="value"></param>
		/// <returns><paramref name="value"/> as a string id</returns>
		public static explicit operator StringID(uint value) { return new StringID(value); }
		/// <summary>
		/// Returns the string id's index value
		/// </summary>
		/// <param name="value"></param>
		/// <returns><paramref name="value"/>'s index value</returns>
		public static explicit operator short(StringID value) { return value.Index; }
		/// <summary>
		/// Returns the string id's length value
		/// </summary>
		/// <param name="value"></param>
		/// <returns><paramref name="value"/>'s length value</returns>
		public static explicit operator byte(StringID value) { return value.Length; }
		#endregion

		#region Util
		/// <summary>
		/// Takes a string id stored in a 32 bit integer and returns the index field
		/// </summary>
		/// <param name="value"></param>
		/// <returns><see cref="Index"/> field</returns>
		public static int ToIndex(uint value)	{ return (int)(value & 0x0000FFFF); }

		/// <summary>
		/// Takes a string id stored in a 32 bit integer and returns the
		/// (Halo2, padding) (Halo3, set) field
		/// </summary>
		/// <param name="value"></param>
		/// <returns><see cref="Set"/> field</returns>
		public static int ToSet(uint value)		{ return (int)((value >> 16) & 0xFF); }

		/// <summary>
		/// Takes a string id stored in a 32 bit integer and returns the length field
		/// </summary>
		/// <param name="value"></param>
		/// <returns><see cref="Length"/> field</returns>
		public static int ToLength(uint value)	{ return (int)((value >> 24) & 0xFF); }

		/// <summary>
		/// Create a unsigned int value using explicit field values found in a string id
		/// </summary>
		/// <param name="index"></param>
		/// <param name="length"></param>
		/// <param name="set"></param>
		/// <returns></returns>
		public static uint ToHandle(short index, byte length, byte set)
		{
			return (uint)(((length & 0xFF) << 24) | ((set & 0xFF) << 16) | (index & 0x7FFF));
		}
		#endregion

		#region IStreamable Members
		/// <summary>
		/// Stream string id from a buffer
		/// </summary>
		/// <param name="s"></param>
		public void Read(BlamLib.IO.EndianReader s) { Handle = s.ReadUInt32(); }
		/// <summary>
		/// Stream string id to a buffer
		/// </summary>
		/// <param name="s"></param>
		public void Write(BlamLib.IO.EndianWriter s) { s.Write(Handle); }
		#endregion

		#region ICompare Members
		public int Compare(StringID x, StringID y)
		{
			if (x.Set == y.Set)	return x.Index - y.Index;
			else				return x.Set - y.Set;
		}

		public int CompareTo(StringID other)
		{
			if (this.Set == other.Set)	return this.Index - other.Index;
			else						return this.Set - other.Set;
		}

		public int Compare(object x, object y)	{ return Compare((StringID)x, (StringID)y); }

		public int CompareTo(object obj)		{ return CompareTo((StringID)obj); }
		#endregion

		#region IEquatable & IEqualityComparer Members
		public bool Equals(StringID x, StringID y)	{ return x.Handle == y.Handle; }
		
		public bool Equals(StringID other)			{ return Equals(this, other); }
		
		public int GetHashCode(StringID obj)		{ return obj.GetHashCode(); }
		#endregion
	};
}