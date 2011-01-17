/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam
{
	#region StringID
	/// <summary>
	/// Constant identifier for a string
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Explicit, Size = 4)]
	public struct StringID : IO.IStreamable, IComparer<StringID>, System.Collections.IComparer, IComparable<StringID>, IComparable,
		IEquatable<StringID>, IEqualityComparer<StringID>
	{
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
					Length < 129;
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
	#endregion

	#region DatumIndex
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
	#endregion

	#region ResourcePtr
#if !NO_HALO2
	/// <summary>
	/// 4 byte integer used to store the location (both offset and file) of a resource
	/// </summary>
	/// <remarks>Used in Halo2 only currently</remarks>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential, Size = 4)]
	public struct ResourcePtr : IO.IStreamable
	{
		/// <summary>
		/// All possible locations of a resource
		/// </summary>
		public enum Location : byte
		{
			/// <summary>
			/// Stored in the cache this resource pointer came from
			/// </summary>
			Internal,
			/// <summary>
			/// Stored in the mainmenu cache
			/// </summary>
			MainMenu,
			/// <summary>
			/// Stored in the shared cache
			/// </summary>
			Shared,
			/// <summary>
			/// Stored in the single player shared cache
			/// </summary>
			Campaign,
		};

		uint Ptr;

		/// <summary>
		/// Create a resource pointer based on a location and an offset
		/// </summary>
		/// <param name="loc">location of resource</param>
		/// <param name="offset">offset of resource in location</param>
		public ResourcePtr(Location loc, int offset) { Ptr = (uint)(offset | ( ((int)loc) << 30 )); }

		/// <summary>
		/// Create a resource pointer based on a value that holds both location and offset
		/// </summary>
		/// <param name="ptr">value holding location and offset</param>
		public ResourcePtr(uint ptr) { Ptr = ptr; }

		/// <summary>
		/// Get the offset of the resource
		/// </summary>
		/// Since the location is stored in the highest bit, and
		/// there is only 4 possible choices (0 - 3) in location,
		/// we use '0x3FFFFFFF' as the mask
		public int Offset { get { return (int)Ptr & 0x3FFFFFFF; } }

		/// <summary>
		/// Return the location of the resource
		/// </summary>
		public Location Map { get { return (Location)(Ptr >> 30); } }
		
		/// <summary>
		/// Returns true if the resource is in the cache the pointer is from
		/// </summary>
		public bool Internal { get { return (Location)(Ptr >> 30) == Location.Internal; } }
		/// <summary>
		/// Returns true if the resource is in the main menu cache
		/// </summary>
		public bool MainMenu { get { return (Location)(Ptr >> 30) == Location.MainMenu; } }
		/// <summary>
		/// Returns true if the resource is in the shared cache
		/// </summary>
		public bool Shared { get { return (Location)(Ptr >> 30) == Location.Shared; } }
		/// <summary>
		/// Returns true if the resource is in the shared single player cache
		/// </summary>
		public bool Campaign { get { return (Location)(Ptr >> 30) == Location.Campaign; } }

		#region Conversions
		/// <summary>
		/// Implicit cast from a signed integer to a resource pointer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>new resource pointer</returns>
		public static implicit operator ResourcePtr(int value) { return new ResourcePtr((uint)value); }
		/// <summary>
		/// Implicit cast from a unsigned integer to a resource pointer
		/// </summary>
		/// <param name="value">value to cast</param>
		/// <returns>ew resource pointer</returns>
		public static implicit operator ResourcePtr(uint value) { return new ResourcePtr(value); }
		#endregion

		#region Overrides
		/// <summary>
		/// Compares two ResourcePtr objects
		/// </summary>
		/// <param name="obj">other ResourcePtr object</param>
		/// <returns>true if both this object and <paramref name="obj"/> are equal in value</returns>
		public override bool Equals(object obj)
		{
			ResourcePtr r = (ResourcePtr)obj;
			return r.Ptr == this.Ptr;
		}
		/// <summary>
		/// Compare two resource pointers (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(ResourcePtr lhs, ResourcePtr rhs) { return lhs.Ptr == rhs.Ptr; }
		/// <summary>
		/// Compare two resource pointers (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(ResourcePtr lhs, ResourcePtr rhs) { return lhs.Ptr != rhs.Ptr; }

		/// <summary>
		/// Returns the hash code for this instance
		/// </summary>
		/// <returns>This resource pointer object as an int</returns>
		public override int GetHashCode() { return (int)this.Ptr; }

		/// <summary>
		/// Converts this instance to a string
		/// </summary>
		/// <returns>"(<see cref="Map"/> @<see cref="Offset"/>)"</returns>
		public override string ToString() { return string.Format("({0} @{1:X}", Map, Offset); }
		#endregion

		#region IStreamable Members
		/// <summary>
		/// Stream the resource pointer from a buffer
		/// </summary>
		/// <param name="input"></param>
		public void Read(IO.EndianReader input) { Ptr = input.ReadUInt32(); }
		/// <summary>
		/// Stream the resource pointer to a buffer
		/// </summary>
		/// <param name="output"></param>
		public void Write(IO.EndianWriter output) { output.Write(Ptr); }
		#endregion
	};
#endif
	#endregion

	#region MapId
#if !NO_HALO3
	/// <summary>
	/// Identifier for maps
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Explicit, Size = MapId.kSizeOf)]
	public struct MapId : IO.IStreamable, IComparer<MapId>, System.Collections.IComparer, IComparable<MapId>, IComparable
	{
		/// <summary>
		/// Size of a single instance of a map id
		/// </summary>
		public const int kSizeOf = 4 + 4;

		#region Fields
		/// <summary>
		/// Id as a 64bit integer
		/// </summary>
		[System.Runtime.InteropServices.FieldOffset(0)]
		public ulong Handle;

		/// <summary>
		/// The campaign index
		/// </summary>
		[System.Runtime.InteropServices.FieldOffset(0)]
		public int Index;

		/// <summary>
		/// The map id
		/// </summary>
		[System.Runtime.InteropServices.FieldOffset(4)]
		public int Id;
		#endregion

		/// <summary>
		/// Construct a map id
		/// </summary>
		/// <param name="index">campaign index, map id</param>
		/// <param name="map_id"></param>
		public MapId(int index, int map_id)
		{
			Handle = 0; // satisfy the compiler, this must come first so we don't fuck up the values

			Index = index;
			Id = map_id;
		}

		/// <summary>
		/// Null map id value
		/// </summary>
		public static readonly MapId Null = new MapId(-1, -1);

		/// <summary>
		/// Returns a Skip field which can be used in a tag definition for a map id
		/// </summary>
		public static TagInterface.Skip SkipField { get { return new TagInterface.Skip(MapId.kSizeOf); } }

		#region IStreamable Members
		/// <summary>
		/// Stream map id from a buffer
		/// </summary>
		/// <param name="input"></param>
		public void Read(IO.EndianReader input) { Index = input.ReadInt32(); Id = input.ReadInt32(); }
		/// <summary>
		/// Stream map id to a buffer
		/// </summary>
		/// <param name="output"></param>
		public void Write(IO.EndianWriter output) { output.Write(Index); output.Write(Id); }
		#endregion

		#region ICompare Members
		public int Compare(MapId x, MapId y)
		{
			if (x.Index == y.Index)	return x.Id - y.Id;			// if the map's are for the same campaign, then test their ids
			else					return x.Index - y.Index;	// they're not for the same campaign
		}

		public int CompareTo(MapId other)
		{
			if (this.Index == other.Index)	return this.Id - other.Id;			// if the map's are for the same campaign, then test their ids
			else							return this.Index - other.Index;	// they're not for the same campaign
		}

		public int Compare(object x, object y) { return Compare((MapId)x, (MapId)x); }

		public int CompareTo(object obj) { return CompareTo((MapId)obj); }
		#endregion

		#region Conversions
		/// <summary>
		/// Implicit cast to a integer
		/// </summary>
		/// <param name="value">map id being casted</param>
		/// <returns>the map id's <see cref="Id"/> value</returns>
		public static explicit operator int(MapId value) { return value.Id; }
		#endregion

		#region Overrides
		/// <summary>
		/// Compares two MapId objects testing their
		/// <see cref="Index"/> and <see cref="Id"/> fields for
		/// equality
		/// </summary>
		/// <param name="obj">other MapId object</param>
		/// <returns>true if both this object and <paramref name="obj"/> are equal</returns>
		public override bool Equals(object obj)
		{
			MapId d = (MapId)obj;
			return d.Handle == this.Handle;
		}
		/// <summary>
		/// Compare two map ids (equality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> == <paramref name="rhs"/></returns>
		public static bool operator ==(MapId lhs, MapId rhs) { return lhs.Handle == rhs.Handle; }
		/// <summary>
		/// Compare two map ids (inequality)
		/// </summary>
		/// <param name="lhs">left-hand value for comparison expression</param>
		/// <param name="rhs">right-hand value for comparison expression</param>
		/// <returns><paramref name="lhs"/> != <paramref name="rhs"/></returns>
		public static bool operator !=(MapId lhs, MapId rhs) { return lhs.Handle != rhs.Handle; }

		/// <summary>
		/// Returns the hash code for this instance
		/// </summary>
		/// <returns>This map id object as an int</returns>
		public override int GetHashCode() { return (this.Index << 30) | this.Id; }

		/// <summary>
		/// Converts this instance to a string
		/// </summary>
		/// <returns>"[<see cref="Index"/>/<see cref="Id"/>]"</returns>
		public override string ToString() { return string.Format("[{0}/{1}]", Index, Id); }
		#endregion
	};
#endif
	#endregion
}