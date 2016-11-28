/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using CT = BlamLib.Blam.Cache.Tags;

namespace BlamLib.Blam.Halo3.Tags
{
	#region bitmap_group
	[TI.TagGroup((int)TagGroups.Enumerated.bitm, 8, 164)]
	public class bitmap_group : TI.Definition
	{
		#region bitmap_group_sequence_block
		[TI.Definition(1, 64)]
		public class bitmap_group_sequence_block : TI.Definition
		{
			#region bitmap_group_sprite_block
			[TI.Definition(1, 32)]
			public class bitmap_group_sprite_block : TI.Definition
			{
				#region Fields
				public TI.BlockIndex BitmapIndex;
				TI.Real Left;
				TI.Real Right;
				TI.Real Top;
				TI.Real Bottom;
				TI.RealPoint2D RegistrationPoint;
				#endregion

				public bitmap_group_sprite_block()
				{
					Add(BitmapIndex = new TI.BlockIndex());
					Add(new TI.Pad(2 + 4));
					Add(Left = new TI.Real());
					Add(Right = new TI.Real());
					Add(Top = new TI.Real());
					Add(Bottom = new TI.Real());
					Add(RegistrationPoint = new TI.RealPoint2D());
				}
			};
			#endregion

			#region Fields
			public TI.String Name;
			public TI.BlockIndex FirstBitmapIndex;
			public TI.ShortInteger BitmapCount;
			public TI.Block<bitmap_group_sprite_block> Sprites;
			#endregion

			public bitmap_group_sequence_block()
			{
				Add(Name = new TI.String());
				Add(FirstBitmapIndex = new TI.BlockIndex());
				Add(BitmapCount = new TI.ShortInteger());
				Add(new TI.Pad(16));
				Add(Sprites = new TI.Block<bitmap_group_sprite_block>(this, 64));
			}
		};
		#endregion

		#region bitmap_data_block
		[TI.Definition(3, 48)]
		public class bitmap_data_block : Bitmaps.bitmap_data_block
		{
			#region Fields
			public TI.ByteInteger Depth;
			public TI.Flags MoreFlags;

			public TI.ByteInteger MipmapCount;
			//public TI.ShortInteger LowDetailMipmapCount;
			#endregion

			#region Ctor
			public bitmap_data_block() : base(14)
			{
				Add(Signature = new TI.Tag());
				Add(Width = new TI.ShortInteger());
				Add(Height = new TI.ShortInteger());
				Add(Depth = new TI.ByteInteger());
				Add(MoreFlags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
				Add(Type = new TI.Enum());
				Add(Format = new TI.Enum());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(RegistrationPoint = new TI.Point2D());
				Add(MipmapCount = new TI.ByteInteger());
				Add(new TI.Skip(1 + //LowDetailMipmapCount?
					1 + // (just interleaved?) resource index
					1)); //
				//Add(LowDetailMipmapCount = new TI.ShortInteger());
				Add(PixelsOffset = new TI.LongInteger());

				Add(new TI.Skip(4 + // pixels size
					4 + 4));

				Add(new TI.LongInteger());

				Add(new TI.Skip(4));
			}
			#endregion

			public override int GetDepth() { return Depth.Value; }
			public override short GetMipmapCount() { return MipmapCount.Value; }
		};
		#endregion

		#region bitmap_cache_resource_data_block
		[TI.Definition(1, 8)]
		public class bitmap_cache_resource_data_block : TI.Definition
		{
			public TI.LongInteger ResourceIndex;

			#region Ctor
			public bitmap_cache_resource_data_block()
			{
				Add(ResourceIndex = new TI.LongInteger());
				Add(new TI.Skip(4));
			}
			#endregion
		};
		#endregion

		#region Fields
		public TI.Enum Type;
		public TI.Enum Format;
		public TI.Enum Usage;
		public TI.Flags Flags;

		//public TI.Real DetailFadeFactor;
		//public TI.Real SharpenAmount;
		//public TI.Real BumpHeight;

		//public TI.Enum SpriteBudgetSize;
		//public TI.ShortInteger SpriteBudgetCount;

		//public TI.ShortInteger ColorPlateWidth;
		//public TI.ShortInteger ColorPlateHeight;
		//public TI.Data CompressedColorPlateData;
		//public TI.Data ProcessedPixelData;

		//public TI.Real BlurFilterSize;
		//public TI.Real AlphaBias;
		//public TI.ShortInteger MipmapCount;

		//public TI.Enum SpriteUsage;
		//public TI.ShortInteger SpriteSpacing;
		//public TI.Enum ForceFormat;

		public TI.Block<bitmap_group_sequence_block> Sequences;
		public TI.Block<bitmap_data_block> Bitmaps;

		public TI.Block<bitmap_cache_resource_data_block> Resources, InterleavedResources;
		#endregion

		#region Ctor
		public bitmap_group()
		{
			Add(Type = new TI.Enum());
			Add(Format = new TI.Enum());
			Add(Usage = new TI.Enum());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));

			//Add(DetailFadeFactor = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			//Add(SharpenAmount = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			//Add(BumpHeight = new TI.Real());

			//Add(SpriteBudgetSize = new TI.Enum());
			//Add(SpriteBudgetCount = new TI.ShortInteger());

			//Add(ColorPlateWidth = new TI.ShortInteger());
			//Add(ColorPlateHeight = new TI.ShortInteger());
			//Add(CompressedColorPlateData = new TI.Data(this));
			//Add(ProcessedPixelData = new TI.Data(this, BlamLib.TagInterface.DataType.Bitmap));

			//Add(BlurFilterSize = new TI.Real());
			//Add(AlphaBias = new TI.Real());
			//Add(MipmapCount = new TI.ShortInteger());

			//Add(SpriteUsage = new TI.Enum());
			//Add(SpriteSpacing = new TI.ShortInteger());
			//Add(ForceFormat = new TI.Enum());
			Add(new TI.Skip(76));

			// TODO: BLOCK STRUCTURE VERIFICATION
			Add(Sequences = new TI.Block<bitmap_group_sequence_block>(this, 256));
			Add(Bitmaps = new TI.Block<bitmap_data_block>(this, 65536));
			Add(new TI.Skip(32));
			Add(Resources = new TI.Block<bitmap_cache_resource_data_block>(this, 65536));
			Add(InterleavedResources = new TI.Block<bitmap_cache_resource_data_block>(this, 65536));
		}
		#endregion
	};
	#endregion

	#region cache_file_resource_layout_table
	[TI.Struct((int)StructGroups.Enumerated.play, 1, 60)]
	public partial class cache_file_resource_layout_table : CT.cache_file_resource_layout_table
	{
		public cache_file_resource_layout_table() : base(5)
		{
			Add(CompressionCodecs = new TI.Block<compression_codec_block>(this, 0)); // 1?
			Add(SharedCaches = new TI.Block<shared_cache_block>(this, 0)); // 3?
			Add(Pages = new TI.Block<pages_block>(this, 0));
			Add(Block24 = new TI.Block<cache_file_resource_layout_table_24_block>(this, 0));
			Add(PageSegments = new TI.Block<page_segment_block>(this, 0));
		}
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
		#region cache_file_resource_gestalt_64_block
		[TI.Definition(1, 120)]
		public partial class cache_file_resource_gestalt_64_block : TI.Definition
		{
			public TI.LongInteger Unknown030,
				Unknown034,
				Unknown038,
				Unknown03C,
				Unknown040;
			public TI.StringId Unknown044;

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
				Add(Unknown044 = new TI.StringId()); // transition zone name related
				Add(TI.UnknownPad.BlockHalo3); // 0x48 [0x14] bsp related? has same count has the bsp count for the cache
					// dword * 5
				Add(TI.UnknownPad.BlockHalo3); // 0x54 [4]
					// dword
				Add(TI.UnknownPad.BlockHalo3); // 0x60 [4]
					// dword
				Add(TI.UnknownPad.BlockHalo3); // 0x6C unused?
			}
		}
		#endregion

		#region Fields
		public TI.Struct<cache_file_resource_layout_table> ResourceLayoutTable;

		public TI.Block<cache_file_resource_gestalt_64_block> Block64,
			Block70, Block7C, Block88, Block94, BlockA0, BlockAC,
			BlockB8, BlockC4, BlockD0, BlockDC;

		public TI.Block<field_block<TI.TagReference>> BspReferences;

		public TI.Block<field_block<TI.LongInteger>> Block1D0;
		#endregion

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