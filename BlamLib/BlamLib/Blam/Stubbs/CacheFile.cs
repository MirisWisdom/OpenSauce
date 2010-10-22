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
			s.ReadInt32(); // version, should be 5

			fileLength = s.ReadInt32();

			int xbox = s.ReadInt32(); // Xbox only field

			offsetToIndex = s.ReadInt32();
			s.ReadInt32(); // stream size

			s.ReadInt32(); s.ReadInt32();

			name = s.ReadTagString();

			build = s.ReadTagString(); // Xbox only field. Always '400'
			cacheType = (CacheType)s.ReadInt32();
			s.ReadInt32(); // CRC

			s.Seek((485 * sizeof(int)) + sizeof(uint), System.IO.SeekOrigin.Current);


			CacheFile cf = s.Owner as CacheFile;
			if (xbox != 0) cf.EngineVersion = BlamVersion.Stubbs_Xbox;
			else cf.EngineVersion = BlamVersion.Stubbs_PC; // no way to tell when its mac...
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