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

namespace BlamLib.Blam.Halo1
{
	public enum Version : int
	{
		Xbox = 5,
		PC = 7,
		PC_Demo = 0x36373530, // '0576'
		CE = 609,
	};

	// xbox 0x7FC5A000 (index buffer address?)
	// xbox 0x803A5FFC vertices and triangles address

	#region Header
	/// <summary>
	/// Halo 1 implementation of the <see cref="Blam.CacheHeader"/>
	/// </summary>
	public sealed class CacheHeader : Blam.CacheHeader
	{
		public override void Read(BlamLib.IO.EndianReader s)
		{
			int pos = 0;

			Blam.CacheFile.ValidateHeader(s, 0x800);
			s.Seek(4);

			Halo1.Version ver = (Halo1.Version)(version = s.ReadInt32());
			fileLength = s.ReadInt32();
			if (fileLength < s.Length) throw new Debug.ExceptionLog("Compressed map editing not available yet!");

			s.ReadInt32();

			if (ver == Halo1.Version.PC_Demo)
			{
				pos = s.Position;
				s.Position = 0x5EC;
				offsetToIndex = s.ReadInt32();
				s.Position = pos;
			}
			else
				offsetToIndex = s.ReadInt32();

			tagBufferSize = s.ReadInt32();

			s.ReadInt32(); s.ReadInt32();

			if (ver == Halo1.Version.PC_Demo)
			{
				pos = s.Position;
				s.Position = 0x5E8;
				name = s.ReadTagString();
				s.Position = pos;
			}
			else
				name = s.ReadTagString();

			build = s.ReadTagString();
			cacheType = (CacheType)s.ReadInt32();
			s.ReadInt32(); // CRC

			s.ReadInt32();
			s.Seek((484 * sizeof(int)) + sizeof(uint), System.IO.SeekOrigin.Current);

			CacheFile cache = s.Owner as CacheFile;
			if (ver == Halo1.Version.Xbox)										cache.EngineVersion = BlamVersion.Halo1_Xbox;
			else if (ver == Halo1.Version.PC || ver == Halo1.Version.PC_Demo)	cache.EngineVersion = BlamVersion.Halo1_PC;
			else if (ver == Halo1.Version.CE)									cache.EngineVersion = BlamVersion.Halo1_CE;
		}
	};
	#endregion

	#region Index
	/// <summary>
	/// Halo 1 implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public abstract class CacheIndexBase : Blam.CacheIndex
	{
		#region VertexBufferCount
		private int vertexBufferCount;
		/// <summary>
		/// 
		/// </summary>
		public int VertexBufferCount	{ get { return vertexBufferCount; } }
		#endregion

		#region VertexBufferOffset
		private uint vertexBufferAddress;
		private int vertexBufferOffset;
		/// <summary>
		/// 
		/// </summary>
		public int VertexBufferOffset { get { return vertexBufferOffset; } }
		#endregion

		#region IndexBufferCount
		private int indexBufferCount;
		/// <summary>
		/// 
		/// </summary>
		public int IndexBufferCount { get { return indexBufferCount; } }
		#endregion

		#region IndexBufferOffset
		private uint indexBufferAddress;
		private int indexBufferOffset;
		/// <summary>
		/// 
		/// </summary>
		public int IndexBufferOffset { get { return indexBufferOffset; } }
		#endregion

		#region Tags
		protected CacheItemBase[] items;
		public override Blam.CacheIndex.Item[] Tags { get { return items; } }

		public CacheItemBase this[int index] { get { return items[index]; } }
		#endregion

		public override void Read(BlamLib.IO.EndianReader s)
		{
			CacheFile cache = s.Owner as CacheFile;
			bool isPC = cache.EngineVersion.IsPc();

			#region base address
			Managers.BlamDefinition bdef = Program.GetManager(cache.EngineVersion);

			cache.AddressMask = bdef[cache.EngineVersion].CacheTypes.BaseAddress
				- (uint)cache.Header.OffsetToIndex;
			#endregion

			address = s.ReadUInt32();
			scenario.Read(s);
			s.ReadInt32(); // CRC
			tagCount = s.ReadInt32();
			vertexBufferCount = s.ReadInt32();
			vertexBufferAddress = s.ReadUInt32(); vertexBufferOffset = (int)(vertexBufferAddress - cache.AddressMask);
			indexBufferCount = s.ReadInt32();
			indexBufferAddress = s.ReadUInt32(); indexBufferOffset = (int)(indexBufferAddress - cache.AddressMask);
			if (isPC) s.ReadInt32(); // total vertex and index buffer size
			s.ReadInt32(); // 'tags'

			tagsOffset = s.PositionUnsigned;

			ReadTagInstances(s, cache);

			InitializeBspTags(s, cache);
		}

		protected abstract void ReadTagInstances(IO.EndianReader s, Blam.CacheFile cache);

		void InitializeBspTags(BlamLib.IO.EndianReader s, Blam.CacheFile cache)
		{
			s.Seek(items[0].Offset + 1444, System.IO.SeekOrigin.Begin);
			bspTags = new Item[s.ReadInt32()];
			uint sbsp_offset = s.ReadPointer();

			s.Seek(sbsp_offset, System.IO.SeekOrigin.Begin);
			DatumIndex di = new DatumIndex();
			CacheItemBase item = null;
			for (int x = 0; x < bspTags.Length; x++)
			{
				s.Seek(28, System.IO.SeekOrigin.Current);
				di.Read(s);
				item = items[di.Index];
				bspTags[x] = item as CacheItemBase;
				s.Seek(-32, System.IO.SeekOrigin.Current);

				item.Offset = s.ReadInt32() + 24; // don't count the header
				item.Size = s.ReadInt32();
				cache.BspAddressMasks.Add(s.ReadUInt32() - (uint)item.Offset); // won't count the header
				item.BspIndex = bspCount++;
				s.Seek(20, System.IO.SeekOrigin.Current);
			}
		}
	};

	/// <summary>
	/// Halo 1 implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public sealed class CacheIndex : CacheIndexBase
	{
		protected override void ReadTagInstances(IO.EndianReader s, Blam.CacheFile cache)
		{
			// Read the tag index items
			items = new CacheItem[tagCount];

			for (int x = 0; x < tagCount; x++)
				(items[x] = new CacheItem()).Read(s);

			// Read the tag filename strings
			foreach (CacheItem ci in items)
				ci.ReferenceName = cache.References.AddOptimized(ci.GroupTag, s.ReadCString());
		}
	};
	#endregion

	#region Item
	/// <summary>
	/// Halo 1 engine base implementation of the <see cref="Blam.CacheIndex.Item"/>
	/// </summary>
	public abstract class CacheItemBase : Blam.CacheIndex.Item
	{
		#region Group Parents
		protected TagInterface.TagGroup groupParent1;
		/// <summary>
		/// The first parent tag group of this tag
		/// </summary>
		public TagInterface.TagGroup GroupParent1	{ get { return groupParent1; } }

		protected TagInterface.TagGroup groupParent2;
		/// <summary>
		/// The second parent tag group of this tag
		/// </summary>
		public TagInterface.TagGroup GroupParent2	{ get { return groupParent2; } }
		#endregion
	};

	/// <summary>
	/// Halo 1 implementation of the <see cref="Blam.CacheIndex.Item"/>
	/// </summary>
	public sealed class CacheItem : CacheItemBase
	{
		#region Null
		public static readonly CacheItem Null = new CacheItem();
		static CacheItem()
		{
			Null.referenceName = DatumIndex.Null;
			Null.offset = -1;
			Null.tagNameOffset = Null.address = 0xFFFFFFFF;
			Null.size = -1;
			Null.datum = DatumIndex.Null;
			Null.groupTag = Null.groupParent1 = Null.groupParent2 = TagInterface.TagGroup.Null;
			Null.bspIndex = -1;
			Null.location = BlamLib.Blam.CacheIndex.ItemLocation.Unknown;
		}
		#endregion

		public override void Read(BlamLib.IO.EndianReader s)
		{
			GameDefinition gd = Program.Halo1.Manager;

			GroupTagInt = s.ReadUInt32();
			groupTag = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(GroupTagInt));
			IO.ByteSwap.SwapUDWord(ref GroupTagInt);

			uint group_tag_int = s.ReadUInt32();
			if (group_tag_int != uint.MaxValue)
				groupParent1 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(group_tag_int));
			else
				groupParent1 = TagInterface.TagGroup.Null;

			group_tag_int = s.ReadUInt32();
			if (group_tag_int != uint.MaxValue)
				groupParent2 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(group_tag_int));
			else
				groupParent2 = TagInterface.TagGroup.Null;

			datum.Read(s);
			tagNameOffset = s.ReadPointer();
			
			address = s.ReadUInt32(); // external index for pc\ce, if its a bitmap, ptr if its a sound or anything else
			if(address > s.BaseAddress) offset = (int)(address - s.BaseAddress);
			this.location = (CacheIndex.ItemLocation)s.ReadInt32(); // bool_in_data_file
			
			s.ReadInt32();
		}
	};
	#endregion

	#region File
	/// <summary>
	/// Halo 1 engine base implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public abstract class CacheFileBase : Blam.CacheFile
	{
		#region Index
		protected Halo1.CacheIndexBase cacheIndex = null;
		public override BlamLib.Blam.CacheIndex Index { get { return cacheIndex; } }

		public Halo1.CacheIndexBase IndexHalo1 { get { return cacheIndex; } }
		#endregion

		protected CacheFileBase(string map_name)
		{
			InputStream = new IO.EndianReader(map_name, IO.EndianState.Little, this);
			if (!CacheIsReadonly(map_name))
				OutputStream = new IO.EndianWriter(map_name, IO.EndianState.Little, this);
		}

		protected CacheFileBase(int pid)
		{
			isMemoryMap = true;
		}

		public override void Read(BlamLib.IO.EndianReader s)
		{
			if (!isMemoryMap)	Header.Read(s);

			if (!isMemoryMap)	s.Seek(Header.OffsetToIndex, System.IO.SeekOrigin.Begin);
			else				s.Seek(Program.GetManager(engineVersion)[engineVersion].CacheTypes.BaseAddress, System.IO.SeekOrigin.Begin);

			base.InitializeReferenceManager(s.FileName);
			base.InitializeTagIndexManager();

			cacheIndex.Read(s);
		}
	};

	/// <summary>
	/// Halo 1 implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public sealed class CacheFile : CacheFileBase
	{
		#region Header
		Halo1.CacheHeader cacheHeader = null;
		public override BlamLib.Blam.CacheHeader Header { get { return cacheHeader; } }

		public Halo1.CacheHeader HeaderHalo1 { get { return cacheHeader; } }
		#endregion

		public CacheFile(string map_name) : base(map_name)
		{
			cacheHeader = new CacheHeader();
			cacheIndex = new CacheIndex();
		}

		public CacheFile(int pid) : base(pid)
		{
			cacheHeader = new CacheHeader();
			cacheIndex = new CacheIndex();
		}
	};
	#endregion

	#region Data Cache
	public enum DataCacheType : int
	{
		/// <summary>
		/// bitmap.map
		/// </summary>
		Bitmap = 0x1,

		/// <summary>
		/// sound.map
		/// </summary>
		Sound,

		/// <summary>
		/// loc.map
		/// </summary>
		/// <remarks>
		/// ustr
		/// font
		/// hmt 
		/// </remarks>
		Loc,
	};

	/// <summary>
	/// Halo 1 data cache implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public sealed class DataIndex : Blam.CacheIndex
	{
		#region Type
		DataCacheType type;
		/// <summary>
		/// 
		/// </summary>
		public DataCacheType Type
		{
			get { return type; }
			set { type = value; }
		}
		#endregion

		#region File Table Offset
		int fileTableOffset;
		/// <summary>
		/// 
		/// </summary>
		public int FileTableOffset
		{
			get { return fileTableOffset; }
			set { fileTableOffset = value; }
		}
		#endregion

		#region Tag Offset Table Offset
		int tagOffsetTableOffset;
		/// <summary>
		/// 
		/// </summary>
		public int TagOffsetTableOffset
		{
			get { return tagOffsetTableOffset; }
			set { tagOffsetTableOffset = value; }
		}
		#endregion

		#region Tags
		private DataItem[] items;
		public override Blam.CacheIndex.Item[] Tags { get { return items; } }

		public DataItem this[int index]
		{
			get { return items[index]; }
			set { items[index] = value; }
		}
		#endregion

		public override void Read(BlamLib.IO.EndianReader s)
		{
			DataCacheFile cache = s.Owner as DataCacheFile;

			type = (DataCacheType)s.ReadInt32();
			fileTableOffset = s.ReadInt32();
			tagOffsetTableOffset = s.ReadInt32();
			tagCount = s.ReadInt32();
			items = new DataItem[tagCount];

			TagInterface.TagGroup tgroup = null;
			switch(type)
			{
				case DataCacheType.Bitmap: tgroup = TagGroups.bitm; break;
				case DataCacheType.Sound: tgroup = TagGroups.snd_; break;
				case DataCacheType.Loc:
					break;
			}

			s.Seek(FileTableOffset, System.IO.SeekOrigin.Begin);
			DataItem item = null;
			for (int x = 0; x < TagCount; x++)
			{
				item = new DataItem();
				items[x] = item;
				item.GroupTag = tgroup;
				item.ReferenceName = cache.References.AddOptimized(item.GroupTag, s.ReadCString());
			}

			s.Seek(tagOffsetTableOffset, System.IO.SeekOrigin.Begin);
			for (int x = 0; x < TagCount; x++)
				items[x].Read(s);
		}
	};

	/// <summary>
	/// Halo 1 data cache implementation of the <see cref="Blam.CacheIndex.Item"/>
	/// </summary>
	public sealed class DataItem : Blam.CacheIndex.Item
	{
		public override void Read(BlamLib.IO.EndianReader s)
		{
			tagNameOffset = s.ReadUInt32();
			size = s.ReadInt32();
			offset = s.ReadInt32();
		}
	};

	/// <summary>
	/// Halo 1 data cache implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	/// <remarks>Data cache does not support <see cref="Blam.CacheHeader"/></remarks>
	public abstract class DataCacheFile : Blam.CacheFile
	{
		#region Header
		/// <summary>
		/// Returns null
		/// </summary>
		public override BlamLib.Blam.CacheHeader Header { get { return null; } }
		#endregion

		#region Index
		Halo1.DataIndex cacheIndex = null;
		public override BlamLib.Blam.CacheIndex Index { get { return cacheIndex; } }

		public Halo1.DataIndex IndexHalo1 { get { return cacheIndex; } }
		#endregion

		private bool SharableReferencePc(string path)
		{
			if (SharableReference(path, Program.Halo1.PcBitmaps))		ShareCacheStreams(this, Program.Halo1.PcBitmaps);
			else if (SharableReference(path, Program.Halo1.PcSounds))	ShareCacheStreams(this, Program.Halo1.PcBitmaps);
			else return false;

			return true;
		}

		private bool SharableReferenceCe(string path)
		{
			if (SharableReference(path, Program.Halo1.CeBitmaps))		ShareCacheStreams(this, Program.Halo1.CeBitmaps);
			else if (SharableReference(path, Program.Halo1.CeSounds))	ShareCacheStreams(this, Program.Halo1.CeSounds);
			else if (SharableReference(path, Program.Halo1.CeLoc))		ShareCacheStreams(this, Program.Halo1.CeLoc);
			else return false;

			return true;
		}

		protected DataCacheFile(BlamVersion engine, string path)
		{
			if (!SharableReferencePc(path) && !SharableReferenceCe(path))
			{
				InputStream = new IO.EndianReader(path, IO.EndianState.Little, this);
				if (!CacheIsReadonly(path))
					OutputStream = new IO.EndianWriter(path, IO.EndianState.Little, this);
			}

			cacheIndex = new DataIndex();

			engineVersion = engine;
		}

		#region I\O
		internal override void ReadResourceCache()	{ base.Read(); }

		public override void Read(BlamLib.IO.EndianReader s)
		{
			base.InitializeReferenceManager(s.FileName);

			cacheIndex.Read(s);
		}

		public byte[] GetData(int offset, int size)
		{
			InputStream.Seek(offset, System.IO.SeekOrigin.Begin);
			return InputStream.ReadBytes(size);
		}
		#endregion
	};

	/// <summary>
	/// Halo 1 bitmap data cache implementation
	/// </summary>
	public sealed class BitmapCacheFile : DataCacheFile
	{
		public BitmapCacheFile(BlamVersion engine, string path) : base(engine, path) { }
	};
	/// <summary>
	/// Halo 1 sound data cache implementation
	/// </summary>
	public sealed class SoundCacheFile : DataCacheFile
	{
		public SoundCacheFile(BlamVersion engine, string path) : base(engine, path) { }
	};
	/// <summary>
	/// Halo 1 localization data cache implementation
	/// </summary>
	public sealed class LocCacheFile : DataCacheFile
	{
		public LocCacheFile(BlamVersion engine, string path) : base(engine, path) { }
	};
	#endregion
}