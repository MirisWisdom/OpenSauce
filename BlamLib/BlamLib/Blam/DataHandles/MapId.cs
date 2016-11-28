/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Blam
{
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
}