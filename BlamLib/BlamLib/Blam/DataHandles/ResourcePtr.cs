/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Blam
{
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
}