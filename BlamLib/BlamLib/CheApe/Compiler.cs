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

namespace BlamLib.CheApe
{
	public abstract class Compiler : IO.IStreamable, IDisposable
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
					foreach (uint tempPos in offsets)
					{
						stream.PositionUnsigned = tempPos;
						stream.WritePointer(address);
					}
				else
					Debug.LogFile.WriteLine("LocationWriteback: failed to writeback! Address was not set, {0} memory locations will be null!", 
						offsets.Count.ToString());

				stream.Position = orgPos;
			}
			#endregion
		};

		#region Constants
		protected const int kDefaultAlignment = 16;
		#endregion

		#region Header
		public sealed class CacheFileHeader : IO.IStreamable
		{
			#region Constants
			const int kVersion = 2;
			const int kSizeOf =
				4 + // head
				4 + // Version
				4 + // EngineSignature;
				4 + // pad32;
				4 + // BaseAddress
				4 + // DataOffset
				4 + // StringPoolSize
				4 + // StringPoolAddress

				4 + // ScriptFunctionsCount
				4 + // ScriptFunctionsAddress
				4 + // ScriptGlobalsCount
				4 + // ScriptGlobalsAddress

				4 + // FixupCount
				4 + // FixupAddress
				4 + // pad32;
				4 + // pad32;

				4; // tail
			public const int kMaxSize = 2048;
			const int kPadSize = CacheFileHeader.kMaxSize - CacheFileHeader.kSizeOf;
			public static readonly byte[] Padding = new byte[CacheFileHeader.kPadSize];

			static readonly TagInterface.TagGroup kHalo1Signature = new TagInterface.TagGroup("blm1", BlamVersion.Halo1_CE.ToString());
			static readonly TagInterface.TagGroup kHalo2Signature = new TagInterface.TagGroup("blm2", BlamVersion.Halo2_PC.ToString());
			#endregion

			TagInterface.TagGroup engineSignature;

			#region BaseAddress
			uint baseAddress;
			/// <summary>
			/// 
			/// </summary>
			public uint BaseAddress
			{
				get { return baseAddress; }
				set { baseAddress = value; }
			}
			#endregion

			#region DataOffset
			int dataOffset;
			/// <summary>
			/// 
			/// </summary>
			public int DataOffset
			{
				get { return dataOffset; }
				set { dataOffset = value; }
			}
			#endregion

			#region StringPoolSize
			int stringPoolSize;
			/// <summary>
			/// 
			/// </summary>
			public int StringPoolSize
			{
				get { return stringPoolSize; }
				set { stringPoolSize = value; }
			}
			#endregion

			#region StringPoolAddress
			uint stringPoolAddress;
			/// <summary>
			/// 
			/// </summary>
			public uint StringPoolAddress
			{
				get { return stringPoolAddress; }
				set { stringPoolAddress = value; }
			}
			#endregion


			#region ScriptFunctionsCount
			int scriptFunctionsCount = 0;
			/// <summary>
			/// 
			/// </summary>
			public int ScriptFunctionsCount
			{
				get { return scriptFunctionsCount; }
				set { scriptFunctionsCount = value; }
			}
			#endregion

			#region ScriptFunctionsAddress
			uint scriptFunctionsAddress = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint ScriptFunctionsAddress
			{
				get { return scriptFunctionsAddress; }
				set { scriptFunctionsAddress = value; }
			}
			#endregion

			#region ScriptGlobalsCount
			int scriptGlobalsCount = 0;
			/// <summary>
			/// 
			/// </summary>
			public int ScriptGlobalsCount
			{
				get { return scriptGlobalsCount; }
				set { scriptGlobalsCount = value; }
			}
			#endregion

			#region ScriptGlobalsAddress
			uint scriptGlobalsAddress = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint ScriptGlobalsAddress
			{
				get { return scriptGlobalsAddress; }
				set { scriptGlobalsAddress = value; }
			}
			#endregion


			#region FixupCount
			int fixupCount = 0;
			/// <summary>
			/// 
			/// </summary>
			public int FixupCount
			{
				get { return fixupCount; }
				set { fixupCount = value; }
			}
			#endregion

			#region FixupAddress
			uint fixupAddress = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint FixupAddress
			{
				get { return fixupAddress; }
				set { fixupAddress = value; }
			}
			#endregion

			internal CacheFileHeader(CacheFileHeader old_state)
			{
				if (old_state == null) throw new ArgumentNullException("old_state");

				this.engineSignature = old_state.engineSignature;
			}
			internal CacheFileHeader(BlamVersion engine)
			{
				switch (engine.ToBuild())
				{
					case BlamBuild.Halo1: engineSignature = kHalo1Signature; break;
					case BlamBuild.Halo2: engineSignature = kHalo2Signature; break;
				}
			}

			#region IStreamable Members
			public void Read(IO.EndianReader stream) { throw new NotSupportedException(); }

			public void Write(IO.EndianWriter stream)
			{
				Blam.MiscGroups.head.Write(stream);
				stream.Write(kVersion);
				engineSignature.Write(stream);
				stream.Write(uint.MinValue);
				stream.Write(baseAddress);
				stream.Write(dataOffset);
				stream.Write(stringPoolSize);
				stream.Write(stringPoolAddress);

				stream.Write(scriptFunctionsCount);
				stream.Write(scriptFunctionsAddress);
				stream.Write(scriptGlobalsCount);
				stream.Write(scriptGlobalsAddress);

				stream.Write(fixupCount);
				stream.Write(fixupAddress);
				stream.Write(uint.MinValue);
				stream.Write(uint.MinValue);

				stream.Write(Padding);
				Blam.MiscGroups.tail.Write(stream);
			}
			#endregion
		};
		#endregion

		public abstract class Object : IO.IStreamable
		{
			#region IStreamable Members
			public virtual void Read(IO.EndianReader stream) { throw new Exception("The method or operation is not implemented."); }
			public abstract void Write(IO.EndianWriter stream);
			#endregion
		};

		/// <summary>
		/// _enum type;
		/// int16 type_data;
		/// void* address[3];
		/// void* definition[3];
		/// PAD32;
		/// </summary>
		internal sealed class Fixup : Object
		{
			enum InternalFixupType : short
			{
				None,
				ReplaceMemory,
				ReplacePointer,
				ReplaceTagField,
			};

			public const int Size = 2 + 2 + (4*3) + (4*3) + 4;

			Import.Fixup fixup = null;
			public void Reset(Import.Fixup def) { fixup = def; }
			public Fixup() { }
			public Fixup(Import.Fixup def) { fixup = def; }

			InternalFixupType DetermineFixupType()
			{
				switch (fixup.Type)
				{
					case Import.FixupType.String:
					case Import.FixupType.Memory:
						return InternalFixupType.ReplaceMemory;

					case Import.FixupType.Pointer:
						return InternalFixupType.ReplacePointer;

					case Import.FixupType.Field:
						return InternalFixupType.ReplaceTagField;

					default: return InternalFixupType.None;
				}
			}

			short DetermineTypeData()
			{
				switch (fixup.Type)
				{
					case Import.FixupType.String:
					case Import.FixupType.Memory:
						return (short)fixup.DefinitionLength;

// 					case Import.FixupType.Pointer:
// 						return 0;

					case Import.FixupType.Field:
						return (short)fixup.Field.TypeIndex;

					default: return 0;
				}
			}

			#region IStreamable Members
			void PlatformRamToStream(Compiler comp,
				IO.EndianWriter stream, 
				uint platform_reference,
				out uint result_reference)
			{
				switch (fixup.Type)
				{
					case Import.FixupType.String:
					case Import.FixupType.Memory:
						result_reference = stream.BaseAddress + stream.PositionUnsigned;
						comp.RamToStream(stream, platform_reference, fixup.DefinitionLength);
						break;

					case Import.FixupType.Pointer:
						result_reference = platform_reference;
						break;

					default:
						result_reference = uint.MaxValue;
						break;
				}
			}

			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;
				uint data_pos_guerilla, data_pos_tool, data_pos_sapien;

				InternalFixupType real_type = DetermineFixupType();

				if (real_type == InternalFixupType.ReplaceTagField)
				{
					data_pos_guerilla =
					data_pos_tool =
					data_pos_sapien =
						stream.BaseAddress + stream.PositionUnsigned;
					IField f = comp.ConstructField(fixup.Field);
					f.Write(stream);
				}
				else if(!fixup.IsPlatformIndependent())
				{
					// if the platform address is NULL, the definition is 
					// assumed to be undefined (and thus not processed) 
					// for the target platform

					if (fixup.AddressGuerilla > 0)
						PlatformRamToStream(comp, stream, 
							fixup.DefinitionGuerilla, out data_pos_guerilla);
					else
						data_pos_guerilla = 0;

					if (fixup.AddressTool > 0)
						PlatformRamToStream(comp, stream, 
							fixup.DefinitionTool, out data_pos_tool);
					else
						data_pos_tool = 0;

					if (fixup.AddressSapien > 0)
						PlatformRamToStream(comp, stream, 
							fixup.AddressSapien, out data_pos_sapien);
					else
						data_pos_sapien = 0;
				}
				else
				{
					PlatformRamToStream(comp, stream, 
						fixup.ToPointer(), out data_pos_guerilla);

					data_pos_sapien = data_pos_tool =
						data_pos_guerilla;
				}

				comp.MarkLocationFixup(fixup.Name, stream, true);
				stream.Write((short)real_type);
				stream.Write(DetermineTypeData());
				stream.Write(fixup.AddressGuerilla);
				stream.Write(fixup.AddressTool);
				stream.Write(fixup.AddressSapien);
				stream.Write(data_pos_guerilla);
				stream.Write(data_pos_tool);
				stream.Write(data_pos_sapien);

				stream.Write(uint.MinValue);
			}
			#endregion
		};

		// Fixup utilities for Field fixups
		internal interface IField : IO.IStreamable
		{
			void Reset(Import.Field def);
		};

		internal abstract IField ConstructField(Import.Field def);

		#region Tag Interface
		/// <summary>
		/// long length;
		/// cstring* elements;
		/// void* definition
		/// </summary>
		internal sealed class StringList : Object
		{
			public const int Size = 4 * 3;

			Import.StringList stringList = null;
			public void Reset(Import.StringList def) { stringList = def; }
			public StringList() { }
			public StringList(Import.StringList def) { stringList = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				uint elementsAddress = stream.PositionUnsigned;
				// write string list's element's addresses
				foreach (uint i in stringList.Elements) stream.Write(i);

				comp.MarkLocationFixup(stringList.Name, stream, true);

				stream.Write(stringList.Elements.Count);
				stream.WritePointer(elementsAddress);
				stream.Write((int)0);
			}
			#endregion
		};

		/// <summary>
		/// long flags;
		/// tag group_tag;
		/// tag* group_tag_list;
		/// </summary>
		internal sealed class TagReference : Object
		{
			public const int Size = 4 * 3;

			Import.TagReference tagRef = null;
			public void Reset(Import.TagReference def) { tagRef = def; }
			public TagReference() { }
			public TagReference(Import.TagReference def) { tagRef = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				uint elementsAddress = 0;
				int flags = (
						(tagRef.IsNonResolving ? 1<<1 : 0)
					);

				if (tagRef.Elements.Count > 1)
				{
					elementsAddress = stream.PositionUnsigned;
					foreach (string i in tagRef.Elements)
						stream.WriteTag(i);

					comp.MarkLocationFixup(tagRef.Name, stream, true);
					stream.Write(flags);
					stream.Write((int)-1);
					stream.WritePointer(elementsAddress);
				}
				else
				{
					comp.MarkLocationFixup(tagRef.Name, stream, true);
					stream.Write(flags);
					stream.WriteTag(tagRef.Elements[0]);
					stream.Write((int)0);
				}
			}
			#endregion
		};

		/// <summary>
		/// cstring name;
		/// long flags;
		/// long max_size;
		/// void* proc_byte_swap;
		/// </summary>
		internal class TagData : Object
		{
			public const int Size = 4 * 4;

			protected Import.TagData tagData;
			public void Reset(Import.TagData def) { tagData = def; }
			public TagData() { }
			public TagData(Import.TagData def) { tagData = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;
				comp.MarkLocationFixup(tagData.Name, stream, false);

				int flags = (
						(tagData.IsNeverStreamed ? 1 << 0 : 0) |
						(tagData.IsTextData ? 1 << 1 : 0) |
						(tagData.IsDebugData ? 1 << 2 : 0)
					);

				stream.Write(tagData.Name);
				stream.Write(flags);
				stream.Write(tagData.MaxSize);
				stream.Write((int)0); // should come back later and write the address of the proc via a lookup table from a xml or text file
			}
			#endregion
		};
		#endregion



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
			ms.Read(buffer, (int)reference, length);

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
					folder + System.IO.Path.GetFileNameWithoutExtension(filename) + ".tag_groups.map",
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
			int align = 16 - ((MemoryStream.Length + Strings.Size) % 16);
			Strings.Write(MemoryStream);
			if (align != 16)
				MemoryStream.Write(new byte[align]);
		}

		protected void FixupsToMemoryStream()
		{
			uint base_address = OwnerState.Definition.MemoryInfo.BaseAddress;

			Head.FixupCount = OwnerState.Importer.Fixups.Count;
			if (Head.FixupCount > 0)
			{
				Fixup f = new Fixup();
				Head.FixupAddress = base_address + MemoryStream.PositionUnsigned;
				foreach (Import.Fixup fu in OwnerState.Importer.Fixups.Values)
				{
					AddLocationFixup(fu.Name, MemoryStream, true);
					MemoryStream.Write((int)0);
				}
				foreach (Import.Fixup fu in OwnerState.Importer.Fixups.Values)
				{
					f.Reset(fu);
					f.Write(MemoryStream);
				}

				AlignMemoryStream(Compiler.kDefaultAlignment);
			}
		}

		protected void EnumerationsToMemoryStream()
		{
			StringList sl = new StringList();
			foreach (Import.StringList slist in OwnerState.Importer.Enums.Values)
			{
				sl.Reset(slist);
				sl.Write(MemoryStream);
			}

			foreach (Import.StringList slist in OwnerState.Importer.Flags.Values)
			{
				sl.Reset(slist);
				sl.Write(MemoryStream);
			}
		}

		protected void TagReferencesToMemoryStream()
		{
			TagReference tr = new TagReference();
			foreach (Import.TagReference tagr in OwnerState.Importer.References.Values)
			{
				tr.Reset(tagr);
				tr.Write(MemoryStream);
			}
		}

		protected void TagDataToMemoryStream()
		{
			TagData td = new TagData();
			foreach (Import.TagData tagd in OwnerState.Importer.Data.Values)
			{
				td.Reset(tagd);
				td.Write(MemoryStream);
			}
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