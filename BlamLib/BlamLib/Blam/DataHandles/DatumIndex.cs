/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Blam
{
	/// <summary>
	/// Generic indexer used for the Blam engine's data array objects.
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Explicit, Size = DatumIndex.kSizeOf)]
	public struct DatumIndex : IO.IStreamable, IComparer<DatumIndex>, System.Collections.IComparer, IComparable<DatumIndex>, IComparable,
		IEquatable<DatumIndex>, IEqualityComparer<DatumIndex>
	{
		/// <summary>
		/// Size of a single instance of a datum index
		/// </summary>
		public const int kSizeOf = 2 + 2;

		public const short ScriptDatum = -7821; // 0xE373
		public const short TagDatum = -7820; // 0xE174

		#region Fields
		/// <summary>
		/// Id as a 32bit integer
		/// </summary>
		[System.Runtime.InteropServices.FieldOffset(0)]
		public uint Handle;

		/// <summary>
		/// The absolute index
		/// </summary>
		/// <remarks>We have to use unsigned short as things like script nodes go above 0x7FFF</remarks>
		[System.Runtime.InteropServices.FieldOffset(0)]
		public ushort Index;

		/// <summary>
		/// The 'salt' of this index, based
		/// off the name of the data array that stores
		/// the datum this indexer is used for
		/// </summary>
		/// <remarks>Decrements as <see cref="Index"/> increments</remarks>
		[System.Runtime.InteropServices.FieldOffset(2)]
		public short Salt;
		#endregion

		/// <summary>
		/// Create a datum index based on an absolute index and a salt
		/// </summary>
		/// <param name="index">absolute index</param>
		/// <param name="datum">'salted' index</param>
		public DatumIndex(ushort index, short datum)
		{
			Handle = 0; // satisfy the compiler, this must come first so we don't fuck up the values

			Index = index;
			Salt = datum;
		}

		/// <summary>
		/// Null datum index value
		/// </summary>
		public static readonly DatumIndex Null = new DatumIndex(ushort.MaxValue, -1);
		public static readonly IEqualityComparer<DatumIndex> kEqualityComparer = new DatumIndex(ushort.MaxValue, -1);

		/// <summary>Does this handle not reference anything?</summary>
		/// <remarks>Checks this value against <see cref="DatumIndex.Null"/></remarks>
		public bool IsNull { get { return this == DatumIndex.Null; } }

		/// <summary>
		/// Returns a Skip field which can be used in a tag definition for a datum index
		/// </summary>
		public static TagInterface.Skip SkipField { get { return new TagInterface.Skip(DatumIndex.kSizeOf); } }

		#region IStreamable Members
		/// <summary>
		/// Stream the datum index from a buffer
		/// </summary>
		/// <param name="input"></param>
		public void Read(IO.EndianReader input) { Handle = input.ReadUInt32(); }
		/// <summary>
		/// Stream the datum index to a buffer
		/// </summary>
		/// <param name="output"></param>
		public void Write(IO.EndianWriter output) { output.Write(Handle); }
		#endregion

		#region ICompare Members
		public int Compare(DatumIndex x, DatumIndex y)	{ return x.Index - y.Index; }

		public int CompareTo(DatumIndex other)			{ return this.Index - other.Index; }

		public int Compare(object x, object y)			{ return Compare((DatumIndex)x, (DatumIndex)y); }

		public int CompareTo(object obj)				{ return CompareTo((DatumIndex)obj); }
		#endregion

		#region IEquatable & IEqualityComparer Members
		/// <summary>
		/// Compares two <see cref="DatumIndex"/> objects testing their
		/// <see cref="Index"/> and <see cref="Salt"/> fields for equality
		/// </summary>
		/// <param name="x">left-hand value for comparison expression</param>
		/// <param name="y">right-hand value for comparison expression</param>
		/// <returns>true if both <paramref name="x"/> and <paramref name="y"/> are equal</returns>
		public bool Equals(DatumIndex x, DatumIndex y)	{ return x.Handle == y.Handle; }
		/// <summary>
		/// Compares this to another <see cref="DatumIndex"/> object testing 
		/// their <see cref="Index"/> and <see cref="Salt"/> fields for equality
		/// </summary>
		/// <param name="other">other <see cref="DatumIndex"/> object</param>
		/// <returns>true if both this object and <paramref name="obj"/> are equal</returns>
		public bool Equals(DatumIndex other)			{ return Equals(this, other); }
		/// <summary>
		/// Returns the hash code for this instance
		/// </summary>
		/// <see cref="implicit operator int(DatumIndex)"/>
		/// <returns>This datum index object as an int</returns>
		public int GetHashCode(DatumIndex obj)			{ return obj.GetHashCode(); }
		#endregion

		#region Conversions
		/// <summary>
		/// Explicit cast to a short integer
		/// </summary>
		/// <param name="value">datum index being cast</param>
		/// <returns>the datum's index</returns>
		public static explicit operator ushort(DatumIndex value) { return value.Index; }

		/// <summary>
		/// Implicit cast to a integer
		/// </summary>
		/// <param name="value">datum index being casted</param>
		/// <returns>the datum index as a integer</returns>
		public static implicit operator int(DatumIndex value)
		{
			return (int)((value.Salt << 24) & 0xff000000) |
						((value.Salt << 8) & 0x00ff0000) |
						((value.Index >> 8) & 0x0000ff00) |
						((value.Index >> 24) & 0x000000ff);
		}

		/// <summary>
		/// Implicit cast from a integer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new datum index</returns>
		public static implicit operator DatumIndex(int value)	{ return new DatumIndex((ushort)(value & 0x0000FFFF), (short)(value >> 16)); }
		#endregion

		#region Overrides
		/// <summary>
		/// Compares two DatumIndex objects testing their
		/// <see cref="Index"/> and <see cref="Salt"/> fields for equality
		/// </summary>
		/// <param name="obj">other DatumIndex object</param>
		/// <returns>true if both this object and <paramref name="obj"/> are equal</returns>
		public override bool Equals(object obj)
		{
			DatumIndex d = (DatumIndex)obj;
			return d.Handle == this.Handle;
		}
		/// <summary>
		/// Compare two datum indexes (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(DatumIndex lhs, DatumIndex rhs) { return lhs.Handle == rhs.Handle; }
		/// <summary>
		/// Compare two datum indexes (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(DatumIndex lhs, DatumIndex rhs) { return lhs.Handle != rhs.Handle; }

		/// <summary>
		/// Returns the hash code for this instance
		/// </summary>
		/// <see cref="implicit operator int(DatumIndex)"/>
		/// <returns>This datum index object as an int</returns>
		public override int GetHashCode() { return this; }

		/// <summary>
		/// Converts this instance to a string
		/// </summary>
		/// <returns>A string with <see cref="Index"/> and <see cref="Salt"/> in brakets</returns>
		public override string ToString() { return string.Format("[{0} {1}]", Index.ToString("X4"), Salt.ToString("X4")); }
		#endregion

		#region Util
		/// <summary>
		/// Takes a datum index stored in a 32 bit integer and returns the index field
		/// </summary>
		/// <param name="datum"></param>
		/// <returns><see cref="Index"/> field</returns>
		public static int ToIndex(int datum) { return datum & 0xFFFF; }

		/// <summary>
		/// Takes a datum index stored in a 32 bit integer and returns the salted
		/// index field
		/// </summary>
		/// <param name="datum"></param>
		/// <returns><see cref="Salt"/> field</returns>
		public static int ToIdentifer(int datum) { return (int)(datum & 0xFFFF0000); }
		#endregion
	};
}