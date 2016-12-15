/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.CheApe
{
	public abstract partial class Compiler : IO.IStreamable, IDisposable
	{
		public sealed class LocationWriteback : IO.IStreamable
		{
			#region Offsets
			List<uint> offsets = new List<uint>();
			/// <summary>
			/// List of offsets to write the value [address] to
			/// </summary>
			public List<uint> Offsets	{ get { return offsets; } }
			#endregion

			#region Address
			uint address;
			/// <summary>
			/// Address which will be written to the [offsets] in a stream
			/// </summary>
			public uint Address
			{
				get { return address; }
				set { address = value; }
			}
			#endregion

			public LocationWriteback() {}
			public LocationWriteback(uint address) { this.address = address; }

			#region IStreamable Members
			public void Read(IO.EndianReader stream) { throw new Exception("The method or operation is not implemented."); }

			public void Write(IO.EndianWriter stream)
			{
				int orgPos = stream.Position;

				if (address != 0)
				{
					if (offsets.Count == 0)
					{
						string name = (stream.Owner as Compiler).GetLocationName(this);
						Debug.LogFile.WriteLine("LocationWriteback: unused address! There are no references to '{0}'", name);
					}
					else foreach (uint tempPos in offsets)
					{
						stream.PositionUnsigned = tempPos;
						stream.WritePointer(address);
					}
				}
				else
				{
					string name = (stream.Owner as Compiler).GetLocationName(this);
					Debug.LogFile.WriteLine("LocationWriteback: failed to writeback! '{0}'s address was not set, {1} memory locations will be null!",
						name, offsets.Count.ToString());
				}

				stream.Position = orgPos;
			}
			#endregion
		};

		#region Constants
		protected const int kDefaultAlignment = 16;
		const int kStringPoolAlignment = 16;
		#endregion

		public abstract class Object : IO.IStreamable
		{
			#region IStreamable Members
			public virtual void Read(IO.EndianReader stream) { throw new NotImplementedException(); }
			public abstract void Write(IO.EndianWriter stream);
			#endregion
		};

		#region Exports
		internal bool AddExport(Import.Object obj)
		{
			return false;
		}
		#endregion

		// Fixup utilities for Field fixups
		internal interface IField : IO.IStreamable
		{
			void Reset(Import.Field def);
		};

		internal abstract IField ConstructField(Import.Field def);



		protected CacheFileHeader Head;
		protected IO.EndianWriter MemoryStream;

		internal ProjectState OwnerState;
		internal Util.StringPool Strings;
		internal Util.StringPool DebugStrings;

		#region RAM Memory
		protected IO.EndianWriter RamMemory;

		internal uint RamAddMemory(string data, out int length)
		{
			uint offset = RamMemory.PositionUnsigned;

			byte[] bytes = Util.ByteStringToArray(data);
			RamMemory.Write(bytes);
			length = bytes.Length;

			//int align = 4 - (RamMemory.Length % 4);
			//if (align != 4) RamMemory.Write(new byte[align]);

			return offset;
		}

		internal uint RamAddString(string str, out int length)
		{
			uint offset = RamMemory.PositionUnsigned;

			RamMemory.Write(str, true);
			length = str.Length + 1;

			//int align = 4 - (RamMemory.Length % 4);
			//if (align != 4) RamMemory.Write(new byte[align]);

			return offset;
		}

		internal void RamToStream(IO.EndianWriter stream, uint reference, int length)
		{
			var ms = RamMemory.BaseStream as System.IO.MemoryStream;

			byte[] buffer = new byte[length];
			ms.Seek(reference, System.IO.SeekOrigin.Begin);
			ms.Read(buffer, 0, length);

			stream.Write(buffer);

			int align = 4 - (stream.Length % 4);
			if (align != 4) stream.Write(new byte[align]);
		}
		#endregion

		protected Compiler(BlamVersion engine)
		{
			Head = new CacheFileHeader(engine);
			MemoryStream = null;
			OwnerState = null;

			DebugStrings = new Util.StringPool(true);
			RamMemory = new BlamLib.IO.EndianWriter(new System.IO.MemoryStream(1024), this);
		}

		public virtual void Reset()
		{
			Dispose();

			Head = new CacheFileHeader(Head);

			DebugStrings = new Util.StringPool(true);
			RamMemory = new BlamLib.IO.EndianWriter(new System.IO.MemoryStream(1024), this);

			Locations.Clear();
		}

		public virtual void Dispose()
		{
			if(MemoryStream != null)
			{
				MemoryStream.Dispose();
				MemoryStream = null;
			}

			if(RamMemory != null)
			{
				RamMemory.Dispose();
				RamMemory = null;
			}
		}

		#region Writebacks
		Dictionary<string, LocationWriteback> Locations = new Dictionary<string, LocationWriteback>();

		protected string GetLocationName(LocationWriteback lwb)
		{
			foreach (var kv in Locations)
				if (object.ReferenceEquals(kv.Value, lwb))
					return kv.Key;

			throw new Debug.Exceptions.UnreachableException(string.Format("CheApe: Unknown LocationWriteback @{0} ({1})", 
				lwb.Address.ToString("X8"), lwb.Offsets.Count.ToString()));
		}

		/// <summary>
		/// Uses <paramref name="string_pool_entry"/> to get a string to use as a key in <see cref="Locations"/> and 
		/// uses the current <see cref="IO.EndianStream.Position"/> in <paramref name="stream"/> as the Address of an 
		/// <see cref="LocationWriteback"/>
		/// </summary>
		/// <param name="string_pool_entry">Valid entry in <see cref="Compiler.Strings"/></param>
		/// <param name="stream">cache stream</param>
		/// <param name="use_debug">Use debug string pool</param>
		protected void MarkLocationFixup(uint string_pool_entry, IO.EndianWriter stream, bool use_debug)
		{
			string name = !use_debug ? this.Strings[string_pool_entry] : this.DebugStrings[string_pool_entry];

			MarkLocationFixup(name, stream);
		}

		/// <summary>
		/// Uses <paramref name="string_pool_entry"/> to get a string to use as a key in <see cref="Locations"/> and 
		/// uses the current <see cref="IO.EndianStream.Position"/> in <paramref name="stream"/> to create a new entry in a 
		/// <see cref="LocationWriteback"/>'s offsets list
		/// </summary>
		/// <param name="string_pool_entry">Valid entry in <see cref="Compiler.Strings"/></param>
		/// <param name="stream">cache stream</param>
		/// <param name="use_debug">Use debug string pool</param>
		protected void AddLocationFixup(uint string_pool_entry, IO.EndianWriter stream, bool use_debug)
		{
			string name = !use_debug ? this.Strings[string_pool_entry] : this.DebugStrings[string_pool_entry];

			AddLocationFixup(name, stream);
		}

		/// <summary>
		/// Uses <paramref name="name"/> as a key in <see cref="Locations"/> and uses the current <see cref="IO.EndianStream.Position"/> 
		/// in <paramref name="stream"/> as the Address of an <see cref="LocationWriteback"/>
		/// </summary>
		/// <param name="name">Location lookup key</param>
		/// <param name="stream">cache stream</param>
		protected void MarkLocationFixup(string name, IO.EndianWriter stream)
		{
			if (!this.Locations.ContainsKey(name))
				this.Locations.Add(name, new LocationWriteback());
			this.Locations[name].Address = stream.PositionUnsigned;
		}

		/// <summary>
		/// Uses <paramref name="name"/> as a key in <see cref="Locations"/> and  uses the current <see cref="IO.EndianStream.Position"/> 
		/// in <paramref name="stream"/> to create a new entry in a <see cref="LocationWriteback"/>'s offsets list
		/// </summary>
		/// <param name="name">Location lookup key</param>
		/// <param name="stream">cache stream</param>
		protected void AddLocationFixup(string name, IO.EndianWriter stream)
		{
			if (!this.Locations.ContainsKey(name))
				this.Locations.Add(name, new LocationWriteback());
			this.Locations[name].Offsets.Add(stream.PositionUnsigned);
		}
		#endregion

		#region IStreamable Members
		public virtual void Read(IO.EndianReader stream) { throw new Exception("The method or operation is not implemented."); }
		public abstract void Write(IO.EndianWriter stream);
		#endregion

		public void Write(string folder, string filename)
		{
			using (IO.EndianWriter ew = new IO.EndianWriter(
					folder + System.IO.Path.GetFileNameWithoutExtension(filename) + ".CheApe.map",
					BlamLib.IO.EndianState.Little, this, true)
					)
			{
				Write(ew);
			}
		}

		#region Compiler Write helpers
		protected void AlignMemoryStream(int k_alignment)
		{
			int align = k_alignment - (MemoryStream.Length % k_alignment);
			if (align != k_alignment) MemoryStream.Write(new byte[align]);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		/// <remarks>Caller must call the returned memory stream's <c>Close</c> method</remarks>
		protected System.IO.MemoryStream InitializeMemoryStream()
		{
			System.IO.MemoryStream mem = new System.IO.MemoryStream(); // the memory stream for writing all the data to a fixed base address
			MemoryStream = new IO.EndianWriter(mem, this); // Endian stream wrapper for memory stream
			MemoryStream.BaseAddress = OwnerState.Definition.MemoryInfo.BaseAddress;

			return mem;
		}

		protected void StringPoolToMemoryStream()
		{
			int align = kStringPoolAlignment - ((MemoryStream.Length + Strings.Size) % kStringPoolAlignment);
			Strings.Write(MemoryStream);
			if (align != kStringPoolAlignment)
				MemoryStream.Write(new byte[align]);
		}
		

		/// <summary>
		/// fix all the pointers for things like tag blocks, etc
		/// </summary>
		protected void PostprocessWritebacks()
		{
			foreach (LocationWriteback lwb in Locations.Values)
				lwb.Write(MemoryStream);
		}

		protected void PostprocessHeaderThenStream(IO.EndianWriter stream, uint string_pool_base_address)
		{
			Head.BaseAddress = OwnerState.Definition.MemoryInfo.BaseAddress;
			Head.DataOffset = Compiler.CacheFileHeader.kMaxSize; // currently we have the data coming after the header
			Head.StringPoolSize = Strings.Size;
			Head.StringPoolAddress = string_pool_base_address;

			Head.Write(stream); // write the Header to the cache file
		}
		#endregion
	};
}