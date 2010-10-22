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
﻿using System.Collections.Generic;

namespace BlamLib.Blam.Cache
{
	/// <summary>
	/// The type of shared cache this is
	/// </summary>
	public enum SharedType : short
	{
		Beta = -2, // Halo 3 beta didn't have this, so we use this value for all beta maps
		/// <summary>
		/// Not a shared cache file
		/// </summary>
		None = -1,

		MainMenu = 0,
		Shared,
		Campaign,
	};
};

namespace BlamLib.Blam.Cache
{
	/// <summary>
	/// Cache Header interface for 3rd generation engines (namely for Xbox 360)
	/// </summary>
	public abstract class CacheHeaderGen3 : CacheHeaderGen2
	{
		#region Flags
		protected Util.Flags flags = new Util.Flags(0);
		/// <summary>
		/// 
		/// </summary>
		public Util.Flags Flags { get { return flags; } }
		#endregion

		#region TagIndexAddress
		protected uint tagIndexAddress;
		/// <summary>
		/// Address in memory of the tag index
		/// </summary>
		public uint TagIndexAddress { get { return tagIndexAddress; } }
		#endregion

		#region MemoryBufferOffset
		protected int memoryBufferOffset;
		/// <summary>
		/// Offset of the start of the buffer that is to be read into memory
		/// </summary>
		public int MemoryBufferOffset { get { return memoryBufferOffset; } }
		#endregion

		#region MemoryBufferSize
		protected int memoryBufferSize;
		/// <summary>
		/// Size of the buffer to be read into memory
		/// </summary>
		public int MemoryBufferSize { get { return memoryBufferSize; } }
		#endregion


		#region BaseAddress
		protected uint baseAddress;
		/// <summary>
		/// 
		/// </summary>
		public uint BaseAddress
		{
			get { return baseAddress; }
			set { baseAddress = value; }
		}
		#endregion

		#region XdkVersion
		protected int xdkVersion;
		/// <summary>
		/// 
		/// </summary>
		public int XdkVersion	{ get { return xdkVersion; } }
		#endregion

		public struct Partition
		{
			public uint BaseAddress;
			public int Size;

			public int Offset;
		};
		#region MemoryPartitions
		protected Partition[] memoryPartitions;
		/// <summary>
		/// 
		/// </summary>
		public Partition[] MemoryPartitions	{ get { return memoryPartitions; } }

		public int GetMemoryPartitionFromAddress(uint address)
		{
			int x = 0;
			foreach(var p in memoryPartitions)
			{
				if (address >= p.BaseAddress && address < (p.BaseAddress + p.Size))
					return x;
				x++;
			}

			if (address < memoryPartitions[0].BaseAddress)
				return -1;
			else if (address > memoryPartitions[memoryPartitions.Length - 1].BaseAddress)
				return -2;
			else throw new Debug.Exceptions.UnreachableException();
		}
		#endregion

		#region Cache Interop
		/// <summary>
		/// Cache section interop data
		/// </summary>
		public class SectionInterop : IO.IStreamable
		{
			/// <summary>
			/// Virtual address of this section (relative to a root base-address, 
			/// refer to "See Also" section)
			/// </summary>
			/// <seealso cref="CacheInterop.ResourceBaseAddress"/>
			/// <seealso cref="CacheInterop.RuntimeBaseAddress"/>
			public uint VirtualAddress;
			/// <summary>
			/// Total size of this section
			/// </summary>
			public int Size;


			/// <summary>
			/// Offset of this section in the cache
			/// </summary>
			public int CacheOffset;
			/// <summary>
			/// Mask to apply to VirtualAddresses that reference a segment of 
			/// this section
			/// </summary>
			public uint AddressMask;

			internal void InitializeCacheOffset(int cache_offset)
			{
				CacheOffset = cache_offset;

				if (VirtualAddress != 0)
					AddressMask = VirtualAddress - (uint)cache_offset;
			}

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				VirtualAddress = s.ReadUInt32();
				Size = s.ReadInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(VirtualAddress);
				s.Write(Size);
			}
			#endregion
		};
		/// <summary>
		/// Cache interop information for dealing with data in the cache sections
		/// </summary>
		public class CacheInterop : IO.IStreamable
		{
			/// <summary>
			/// Base address for Resource section
			/// </summary>
			/// <remarks>Meaningless for release builds.
			/// 
			/// Unused (zero) in pure shared caches.
			/// 
			/// Represents the sole "base" address that all section 
			/// <see cref="SectionInterop.VirtualAddress">VirtualAddress</see>s are relative to.
			/// 
			/// EXCEPT in ANY cache build which is not <see cref="CacheType.Multiplayer">
			/// Multiplayer</see>. Shared, campaign, etc doesn't have the <see cref="RuntimeBaseAddress"/> 
			/// follow after this address.
			/// </remarks>
			public uint ResourceBaseAddress;
			/// <summary>
			/// Total size of the Debug section buffer
			/// </summary>
			// This value may actually represent the 'release build data' offset but in 
			// the end we'd just be arguing the semantics of value names. In the end it's 
			// all how we use the values to perform our calculations, and since we don't 
			// have access to any other build types (ie, tag, profile) this is the best name 
			// one can derive
			public int DebugSectionSize;
			/// <summary>
			/// Base address for runtime (Debug, Tag and Language pack) sections
			/// </summary>
			/// <remarks>Meaningless for release builds. Unused (zero) in pure shared caches.</remarks>
			public uint RuntimeBaseAddress;
			/// <summary></summary>
			/// <remarks>Meaningless for release builds.
			/// 
			/// Unused (zero) in pure shared caches.
			/// 
			/// Possibly this was used for a shared-resource-database section? Just a wild guess
			/// </remarks>
			public uint UnknownBaseAddress;
			SectionInterop[] sections;

			public CacheInterop()
			{
				sections = new SectionInterop[(int)CacheSectionType.kMax];
				for (int x = 0; x < sections.Length; x++)
					sections[x] = new SectionInterop();
			}

			public SectionInterop this[CacheSectionType type] { get {
				int index = (int)type;

				return sections[index];
			} }

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				ResourceBaseAddress = s.ReadUInt32();
				DebugSectionSize = s.ReadInt32();
				RuntimeBaseAddress = s.ReadUInt32();
				UnknownBaseAddress = s.ReadUInt32();

				foreach (var sec in sections) sec.Read(s);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(ResourceBaseAddress);
				s.Write(DebugSectionSize);
				s.Write(RuntimeBaseAddress);
				s.Write(UnknownBaseAddress);

				foreach (var sec in sections) sec.Write(s);
			}
			#endregion

			public void PostprocessForCacheRead(int cache_header_sizeof)
			{
				int cache_offset = cache_header_sizeof;

				var type = CacheSectionType.Debug;
				var section = this[type];
				section.InitializeCacheOffset(cache_offset);

				int release_offset = DebugSectionSize; // offset for release build data
				for(++type; type < CacheSectionType.kMax; type++,
					release_offset += section.Size)
				{
					section = this[type];
					if (section.Size == 0) continue;

					section.InitializeCacheOffset(release_offset);
				}
			}
		};
		protected CacheInterop cacheInterop = new CacheInterop();

		public CacheInterop Interop { get { return cacheInterop; } }
		#endregion


		public System.Guid CompressionGuid;

		#region SharedType
		protected Cache.SharedType sharedType = Cache.SharedType.None;
		/// <summary>
		/// 
		/// </summary>
		public Cache.SharedType SharedType { get { return sharedType; } }
		#endregion
	};


	/// <summary>
	/// Cache Index interface for 3rd generation engines (namely for Xbox 360)
	/// </summary>
	public abstract class CacheIndexGen3 : CacheIndexGen2
	{
		#region GroupTags
		protected Cache.CacheItemGroupTagGen3[] groupTags;
		public Cache.CacheItemGroupTagGen3[] GroupTags { get { return groupTags; } }
		#endregion

		#region DependentTags
		public DatumIndex[] DependentDatums;

		protected uint dependentTagsOffset;
		protected uint dependentTagsAddress;
		public uint DependentTagsAddress	{ get { return dependentTagsAddress; } }

		protected int dependentTagsCount;
		public int DependentTagsCount	{ get { return dependentTagsCount; } }
		#endregion

		#region Tags
		protected Cache.CacheItemGen3[] items;
		public override Blam.CacheIndex.Item[] Tags { get { return items; } }

		public Cache.CacheItemGen3 this[int index]
		{
			get { return items[index]; }
//			set { items[index] = value; }
		}
		#endregion

		protected void ReadDependents(BlamLib.IO.EndianReader s)
		{
			DependentDatums = new DatumIndex[dependentTagsCount];
			s.Seek(dependentTagsOffset, System.IO.SeekOrigin.Begin);
			for (int x = 0; x < dependentTagsCount; x++)
			{
				s.Seek(sizeof(uint), System.IO.SeekOrigin.Current); // group tag
				DependentDatums[x].Read(s);
			}
		}

		protected void ReadTagInstances(BlamLib.IO.EndianReader s)
		{
			items = new Cache.CacheItemGen3[tagCount];
			s.Seek(tagsOffset, System.IO.SeekOrigin.Begin);
			for (int x = 0; x < tagCount; x++)
				(items[x] = new Cache.CacheItemGen3()).Read(s);
		}
	};


	/// <summary>
	/// Cache Tag instance interface for 3rd generation engines (namely for Xbox 360)
	/// </summary>
	public class CacheItemGen3 : Blam.CacheIndex.Item
	{
		#region Null
		public static readonly CacheItemGen3 Null = new CacheItemGen3();
		static CacheItemGen3()
		{
			Null.referenceName = DatumIndex.Null;
			Null.offset = -1;
			Null.tagNameOffset = Null.address = 0xFFFFFFFF;
			Null.size = -1;
			Null.datum = DatumIndex.Null;
			Null.groupTag = TagInterface.TagGroup.Null;
			Null.bspIndex = -1;
			Null.location = BlamLib.Blam.CacheIndex.ItemLocation.Unknown;
		}
		#endregion

		public short GroupIndex;

		public override void Read(BlamLib.IO.EndianReader s)
		{
			GroupIndex = (short)(datum.Index = s.ReadUInt16());
			datum.Salt = (short)s.ReadUInt16();
			address = s.ReadUInt32();
			offset = (int)(address - s.BaseAddress);
			size = 0;
		}

		internal void Fixup(ushort index) { this.datum.Index = index; }
	};


	public abstract class CacheItemGroupTagGen3 : CacheItemGroupTagGen2
	{
		public StringID Name;

		public override string ToString()
		{
			return string.Format("{0}\t{1}\t{2}\t{3}",
				GroupTag1.TagToString(),
				GroupTag2.TagToString(),
				GroupTag3.TagToString(),
				Name.Index.ToString("X"));
		}

		public override void Write(BlamLib.IO.EndianWriter s)
		{
			base.Write(s);
			
			Name.Write(s);
		}
	};


	public abstract class CacheFileGen3 : CacheFileGen2
	{
		protected override void OutputExtraTagIndexInfo(System.IO.StreamWriter s)
		{
			var index_gen3 = Index as CacheIndexGen3;

			s.WriteLine("Tag Groups:");
			foreach (var cigt in index_gen3.GroupTags)
				s.WriteLine("\t{0}", cigt.ToString());

			s.WriteLine();

			s.WriteLine("Dependent Tag Instances:");
			foreach (BlamLib.Blam.DatumIndex di in index_gen3.DependentDatums)
				s.WriteLine("\t0x{0}\t{1}", di.Handle.ToString("X"), di.ToString());
		}

		protected override void OutputExtraTagInstanceInfo(System.IO.StreamWriter s)
		{
			var header_gen3 = Header as CacheHeaderGen3;
			int x;

			var partition_list = new List<Blam.Cache.CacheItemGen3>[header_gen3.MemoryPartitions.Length + 2];
			for (x = 0; x < partition_list.Length; x++)
				partition_list[x] = new List<Blam.Cache.CacheItemGen3>();

			var items = Index.Tags as Blam.Cache.CacheItemGen3[];

			foreach (var item in items)
			{
				if (item.IsEmpty) continue;

				int p = header_gen3.GetMemoryPartitionFromAddress(item.Address);
				if (p == -1)
					p = partition_list.Length - 2;
				else if (p == -2)
					p = partition_list.Length - 1;
				partition_list[p].Add(item);
			}

			x = 0;
			foreach(var pl in partition_list)
			{
				if(pl.Count == 0)
				{
					x++;
					continue;
				}

				if (x == partition_list.Length - 2)		s.WriteLine("LessThan		{0}", header_gen3.MemoryPartitions[0].BaseAddress.ToString("X8"));
				else if (x == partition_list.Length - 1)s.WriteLine("GreaterThan	{0}", header_gen3.MemoryPartitions[header_gen3.MemoryPartitions.Length-1].BaseAddress.ToString("X8"));
				else									s.WriteLine("Partition		{0}: {1} to {2} ({3})", x.ToString(),
					header_gen3.MemoryPartitions[x].BaseAddress.ToString("X8"),
					(header_gen3.MemoryPartitions[x].BaseAddress + header_gen3.MemoryPartitions[x].Size).ToString("X8"),
					header_gen3.MemoryPartitions[x].Size.ToString("X8"));

				foreach(var item in pl)
				{
					s.WriteLine("\t{0}\t{1}\t{2}", item.Datum.Handle.ToString("X8"),
						item.GroupTag.TagToString(),
						GetReferenceName(item));
				}
				x++;
			}
		}
	};
}