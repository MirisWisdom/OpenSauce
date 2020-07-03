/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Stubbs
{
	#region Header
	/// <summary>
	/// Stubbs implementation of the <see cref="Blam.CacheHeader"/>
	/// </summary>
	public sealed class CacheHeader : Blam.CacheHeader
	{
		public override void Read(BlamLib.IO.EndianReader s)
		{
			Blam.CacheFile.ValidateHeader(s, 0x800);
			
			s.Seek(4);
			version = s.ReadInt32();
			if (version != 5) throw new InvalidCacheFileException(s.FileName);

			fileLength = s.ReadInt32();

			int xbox = s.ReadInt32(); // Xbox only field

			offsetToIndex = s.ReadInt32();
			s.ReadInt32(); // stream size

			s.ReadInt32(); s.ReadInt32();

			name = s.ReadTagString();

			build = s.ReadTagString(); // Xbox only field. Always '400'
			cacheType = (CacheType)s.ReadInt16();
			s.ReadInt16();
			s.ReadInt32(); // CRC

			s.Seek((485 * sizeof(int)) + sizeof(uint), System.IO.SeekOrigin.Current);


			CacheFile cf = s.Owner as CacheFile;
			if (xbox != 0)
				cf.EngineVersion = BlamVersion.Stubbs_Xbox;
			else // no way to tell when it's mac, which just seems to use PC maps anyway (byte swaps everything when map is loaded)
				cf.EngineVersion = BlamVersion.Stubbs_PC;
		}
	};
	#endregion

	#region Index
	/// <summary>
	/// Stubbs implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public sealed class CacheIndex : Halo1.CacheIndexBase
	{
		protected override void ReadTagInstances(IO.EndianReader s, Blam.CacheFile cache)
		{
			// Read the tag index items
			items = new CacheItem[tagCount];

			for (int x = 0; x < tagCount; x++)
				(items[x] = new CacheItem()).Read(s);

			// Read the tag filename strings
			foreach (Stubbs.CacheItem ci in items)
				ci.ReferenceName = cache.References.AddOptimized(ci.GroupTag, s.ReadCString());
		}
	};
	#endregion

	#region Item
	/// <summary>
	/// Stubbs implementation of the <see cref="Blam.CacheIndex.Item"/>
	/// </summary>
	public sealed class CacheItem : Halo1.CacheItemBase
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
			GameDefinition gd = Program.Stubbs.Manager;

			GroupTagInt = s.ReadUInt32();
			groupTag = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(GroupTagInt));
			Debug.Assert.If(groupTag != null, "{0}", new string(TagInterface.TagGroup.FromUInt(GroupTagInt)));
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

			address = s.ReadUInt32();
			offset = (int)(address - s.BaseAddress);

			s.ReadInt32();
			s.ReadInt32();
		}
	};
	#endregion

	#region File
	/// <summary>
	/// Stubbs implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public sealed class CacheFile : Halo1.CacheFileBase
	{
		#region Header
		Stubbs.CacheHeader cacheHeader = null;
		public override BlamLib.Blam.CacheHeader Header { get { return cacheHeader; } }

		public Stubbs.CacheHeader HeaderStubbs { get { return cacheHeader; } }
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
}