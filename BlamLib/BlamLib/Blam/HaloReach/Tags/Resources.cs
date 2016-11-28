/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using CT = BlamLib.Blam.Cache.Tags;

namespace BlamLib.Blam.HaloReach.Tags
{
	#region cache_file_resource_layout_table
	[TI.Struct((int)StructGroups.Enumerated.play, 1, 60)]
	public class cache_file_resource_layout_table : Halo3.Tags.cache_file_resource_layout_table
	{
		public cache_file_resource_layout_table() : base() { }
	};

	[TI.TagGroup((int)TagGroups.Enumerated.play, 1, 60)]
	public class cache_file_resource_layout_table_group : CT.cache_file_resource_layout_table_group
	{
		public TI.Struct<cache_file_resource_layout_table> ResourceLayoutTable;

		public cache_file_resource_layout_table_group() : base(1)
		{
			Add(ResourceLayoutTable = new TI.Struct<cache_file_resource_layout_table>(this));
		}

		public override CT.cache_file_resource_layout_table GetResourceLayoutTable() { return ResourceLayoutTable.Value; }
	};
	#endregion

	#region cache_file_resource_gestalt
	[TI.TagGroup((int)TagGroups.Enumerated.zone, 1, 532)]
	public partial class cache_file_resource_gestalt_group : CT.cache_file_resource_gestalt_group
	{
		// TODO: haven't completely verified the child block structures for changes yet
		#region cache_file_resource_gestalt_64_block
		[TI.Definition(2, 160)]
		public partial class cache_file_resource_gestalt_64_block : TI.Definition
		{
			public TI.LongInteger Unknown030,
				Unknown034,
				Unknown038,
				Unknown03C,
				Unknown040,
				Unknown044;

			public cache_file_resource_gestalt_64_block() : base(14)
			{
				Add(TI.UnknownPad.BlockHalo3); // 0x00 [4]
					// dword
				Add(TI.UnknownPad.BlockHalo3); // 0x0C unused?
				Add(TI.UnknownPad.BlockHalo3); // 0x18 [4]
					// dword
				Add(TI.UnknownPad.BlockHalo3); // 0x24 [4]
					// dword
				Add(Unknown030 = new TI.LongInteger()); // Resource location (page) offset
				Add(Unknown034 = new TI.LongInteger()); // ''
				Add(Unknown038 = new TI.LongInteger()); // ''
				Add(Unknown03C = new TI.LongInteger()); // ''
				Add(Unknown040 = new TI.LongInteger()); // ''
				Add(Unknown044 = new TI.LongInteger()); // ''
				Add(TI.UnknownPad.BlockHalo3); // 0x48 [0x14] bsp related? has same count has the bsp count for the cache
					// dword * 5

				// I think the following two are inserted fields
				Add(TI.UnknownPad.BlockHalo3); // 0x54 ?
				Add(TI.UnknownPad.BlockHalo3); // 0x60 ?

				Add(TI.UnknownPad.BlockHalo3); // 0x6C [4]
					// dword
				Add(TI.UnknownPad.BlockHalo3); // 0x78 [4]
					// dword
				Add(TI.UnknownPad.BlockHalo3); // 0x84 unused?
				Add(new TI.UnknownPad(16)); // 0x90 ?
			}
		}
		#endregion

		public TI.Struct<cache_file_resource_layout_table> ResourceLayoutTable;

		public TI.Block<cache_file_resource_gestalt_64_block> Block64,
			Block70, Block7C, Block88, Block94, BlockA0, BlockAC,
			BlockB8, BlockC4, BlockD0, BlockDC;

		public TI.Block<field_block<TI.TagReference>> BspReferences;

		public TI.Block<field_block<TI.LongInteger>> Block1D0;

		public cache_file_resource_gestalt_group() : base(41)
		{
			Add(CacheType = new TI.Enum());
			Add(Flags = TI.Flags.Word);
			/*0x04*/Add(ResourceTypes = new TI.Block<resource_type_block>(this, 0));
			/*0x10*/Add(ResourceStructureTypes = new TI.Block<resource_structure_type_block>(this, 0));
			/*0x1C*/Add(ResourceLayoutTable = new TI.Struct<cache_file_resource_layout_table>(this));
			/*0x58*/Add(TagResources = new TI.Block<cache_file_resource_gestalt_tag_resource_block>(this, 0));
			/*0x64*/Add(Block64 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // general 
			/*0x70*/Add(Block70 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] global
			/*0x7C*/Add(Block7C = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78]? attached?
			/*0x88*/Add(Block88 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] unattached // same structure as tag block two fields above
			/*0x94*/Add(Block94 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] dvd_forbidden // ''
			/*0xA0*/Add(BlockA0 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] dvd_always_streaming // ''
			/*0xAC*/Add(BlockAC = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] bsp zones // '' -
			/*0xB8*/Add(BlockB8 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] bsp zones // ''
			/*0xC4*/Add(BlockC4 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] bsp zones // ''
			/*0xD0*/Add(BlockD0 = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] ? (ex. '030la_highway') // ''
			/*0xDC*/Add(BlockDC = new TI.Block<cache_file_resource_gestalt_64_block>(this, 0)); // tag block [0x78] zone sets // ''
			/*0xE8*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0xF4*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x100*/Add(Block100 = new TI.Block<cache_file_resource_gestalt_100_block>(this, 0));
			/*0x10C*/Add(BspReferences = new TI.Block<field_block<TI.TagReference>>(this, 0));
			/*0x118*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			/*0x124*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			/*0x130*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			/*0x13C*/Add(ResourceDefinitionData = new TI.Data(this)); // tag data
			/*0x150*/Add(new TI.UnknownPad(20)); // unknown [0x14]
				// dword
				// long
				// long
				// long
				// long (related to the UnknownOffset in 'cache_file_resource_gestalt_tag_resource_block', size of the buffer it refers to)
			/*0x164*/Add(Block164 = new TI.Block<cache_file_resource_gestalt_164_block>(this, 0));
			/*0x170*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x17C*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x188*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x194*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x1A0*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x1AC*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x1B8*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x1C4*/Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			/*0x1D0*/Add(Block1D0 = new TI.Block<field_block<TI.LongInteger>>(this, 0)); // datum index in nature
			/*0x1DC*/Add(Block1DC = new TI.Block<cache_file_resource_gestalt_1DC_block>(this, 0));
			/*0x1E8*/Add(Block1E8 = new TI.Block<cache_file_resource_gestalt_1E8_block>(this, 0));
			/*0x1F4*/Add(Block1F4 = new TI.Block<cache_file_resource_gestalt_1F4_block>(this, 0));
			/*0x200*/Add(Block200 = new TI.Block<cache_file_resource_gestalt_200_block>(this, 0));
			Add(MapId = Blam.MapId.SkipField); // id used for mapinfo files

			//BspReferences.Definition.Value.ResetReferenceGroupTag(TagGroups.sbsp);
		}
	};
	#endregion
}