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
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region bitmap_group
	#region bitmap_texture_interop_resource_reference
	[TI.Definition(1, 12)]
	public class bitmap_texture_interop_resource_reference : TI.Definition
	{
		#region bitmap_texture_interop_resource
		[TI.Definition(1, 52)]
		public class bitmap_texture_interop_resource : TI.Definition
		{
			public TI.Data BitmapData;
			public TI.Data UnknownData;
			public TI.ShortInteger Width, Height;
			public TI.ByteInteger Unknown2C, Unknown2D, Unknown2E, Unknown2F;
			public TI.LongInteger Unknown30;

			public bitmap_texture_interop_resource() : base(4)
			{
				Add(BitmapData = new TI.Data(this));
				Add(UnknownData = new TI.Data(this));
				Add(Width = new BlamLib.TagInterface.ShortInteger());
				Add(Height = new BlamLib.TagInterface.ShortInteger());
				Add(Unknown2C = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown2D = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown2E = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown2F = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown30 = new BlamLib.TagInterface.LongInteger());
			}
		};
		#endregion

		public TI.StructReference<bitmap_texture_interop_resource> Reference;

		public bitmap_texture_interop_resource_reference() : base(1)
		{
			Add(Reference = new TI.StructReference<bitmap_texture_interop_resource>(this));
		}
	};
	#endregion

	#region bitmap_texture_interleaved_interop_resource_reference
	[TI.Definition(1, 12)]
	public class bitmap_texture_interleaved_interop_resource_reference : TI.Definition
	{
		#region bitmap_texture_interleaved_interop_resource
		[TI.Definition(1, 52)]
		public class bitmap_texture_interleaved_interop_resource : TI.Definition
		{
			public TI.Data BitmapData;
			public TI.Data UnknownData;

			public TI.ShortInteger Width, Height;
			public TI.ByteInteger Unknown2C, Unknown2D, Unknown2E, Unknown2F;
			public TI.LongInteger Unknown30;

			public TI.ShortInteger Width2, Height2;
			public TI.ByteInteger Unknown38, Unknown39, Unknown3A, Unknown3B;
			public TI.LongInteger Unknown3C;

			public bitmap_texture_interleaved_interop_resource() : base(4)
			{
				Add(BitmapData = new TI.Data(this));
				Add(UnknownData = new TI.Data(this));
				Add(Width = new BlamLib.TagInterface.ShortInteger());
				Add(Height = new BlamLib.TagInterface.ShortInteger());
				Add(Unknown2C = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown2D = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown2E = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown2F = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown30 = new BlamLib.TagInterface.LongInteger());

				Add(Width2 = new BlamLib.TagInterface.ShortInteger());
				Add(Height2 = new BlamLib.TagInterface.ShortInteger());
				Add(Unknown38 = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown39 = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown3A = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown3B = new BlamLib.TagInterface.ByteInteger());
				Add(Unknown3C = new BlamLib.TagInterface.LongInteger());
			}
		};
		#endregion

		public TI.StructReference<bitmap_texture_interleaved_interop_resource> Reference;

		public bitmap_texture_interleaved_interop_resource_reference() : base(1)
		{
			Add(Reference = new TI.StructReference<bitmap_texture_interleaved_interop_resource>(this));
		}
	};
	#endregion

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

			protected override int GetDepth() { return Depth.Value; }
			protected override short GetMipmapCount() { return MipmapCount.Value; }
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
	public partial class cache_file_resource_layout_table : TI.Definition
	{
		#region compression_codec_block
		[TI.Definition(1, 16)]
		public partial class compression_codec_block : TI.Definition
		{
			public TI.Skip Guid;

			#region Ctor
			public compression_codec_block()
			{
				Add(Guid = new TI.Skip(16));
			}
			#endregion
		}
		#endregion

		#region shared_cache_block
		[TI.Definition(1, 264)]
		public partial class shared_cache_block : TI.Definition
		{
			public TI.String CachePath;
			public TI.ShortInteger Unknown100;
			public TI.Flags Flags;
			public TI.LongInteger Unknown104;

			#region Ctor
			public shared_cache_block()
			{
				Add(CachePath = new TI.String(BlamLib.TagInterface.StringType.Ascii, 256));
				Add(Unknown100 = new TI.ShortInteger());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(Unknown104 = new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		// runtime resource locations
		#region pages_block
		[TI.Definition(1, 88)]
		public partial class pages_block : TI.Definition
		{
			public TI.ShortInteger Header; // datum's salt value
			public TI.Flags Flags;
			public TI.BlockIndex CompressionCodec;

			public TI.BlockIndex SharedCache;
			public TI.ShortInteger Unknown006; // Index of something
			public TI.LongInteger BlockOffset; // (aligned to 4096 blocks)
			public TI.LongInteger BlockSizeCompressed;
			public TI.LongInteger BlockSizeUncompressed;

			// s_resource_checksum
			public TI.LongInteger Crc;
			public TI.Skip EntireBufferHash; // 7BFD8BA3F41CDBCD23065111ABA46E7D436FAC61
			public TI.Skip FirstChunkHash;
			public TI.Skip LastChunkHash;

			public TI.ShortInteger Unknown054; // ?
			public TI.ShortInteger Unknown056; // Index?

			public pages_block()
			{
				Add(Header = new TI.ShortInteger());
				Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
				Add(CompressionCodec = new TI.BlockIndex(TI.FieldType.ByteBlockIndex));

				Add(SharedCache = new TI.BlockIndex());
				Add(Unknown006 = new TI.ShortInteger());
				Add(BlockOffset = new TI.LongInteger());
				Add(BlockSizeCompressed = new TI.LongInteger());
				Add(BlockSizeUncompressed = new TI.LongInteger());

				Add(Crc = new TI.LongInteger());
				Add(EntireBufferHash = new TI.Skip(20));
				Add(FirstChunkHash = new TI.Skip(20));
				Add(LastChunkHash = new TI.Skip(20));

				Add(Unknown054 = new TI.ShortInteger());
				Add(Unknown056 = new TI.ShortInteger());
			}
		};
		#endregion

		#region cache_file_resource_layout_table_24_block
		[TI.Definition(1, 16)]
		public partial class cache_file_resource_layout_table_24_block : TI.Definition
		{
			#region block_4
			[TI.Definition(1, 16)]
			public partial class block_4 : TI.Definition
			{
				public TI.LongInteger Unknown00;
				public TI.LongInteger Unknown04;
				public TI.LongInteger Unknown08;
				public TI.LongInteger Unknown0C;

				public block_4()
				{
					Add(Unknown00 = new TI.LongInteger());
					Add(Unknown04 = new TI.LongInteger());
					Add(Unknown08 = new TI.LongInteger());
					Add(Unknown0C = new TI.LongInteger());
				}
			}
			#endregion

			public TI.LongInteger Size;
			public TI.Block<block_4> Unknown04;

			public cache_file_resource_layout_table_24_block()
			{
				Add(Size = new TI.LongInteger());
				Add(Unknown04 = new TI.Block<block_4>(this));
			}
		}
		#endregion

		#region page_segment_block
		[TI.Definition(1, 16)]
		public partial class page_segment_block : TI.Definition // http://en.wikipedia.org/wiki/Segmentation_(memory)
		{
			public TI.BlockIndex RequiredPageIndex;
			public TI.BlockIndex OptionalPageIndex;
			public TI.LongInteger RequiredSegmentOffset;
			public TI.LongInteger OptionalSegmentOffset;
			public TI.LongInteger DatumIndexBlock24; // seems like only sounds use this...

			public page_segment_block()
			{
				Add(RequiredPageIndex = new TI.BlockIndex());
				Add(OptionalPageIndex = new TI.BlockIndex());
				Add(RequiredSegmentOffset = new TI.LongInteger());
				Add(OptionalSegmentOffset = new TI.LongInteger());
				Add(DatumIndexBlock24 = new TI.LongInteger());
			}
		};
		#endregion

		public TI.Block<compression_codec_block> CompressionCodecs;
		public TI.Block<shared_cache_block> SharedCaches;
		public TI.Block<pages_block> Pages; // 18
		public TI.Block<cache_file_resource_layout_table_24_block> Block24; // 24
		public TI.Block<page_segment_block> PageSegments; // 30

		public cache_file_resource_layout_table()
		{
			Add(CompressionCodecs = new TI.Block<compression_codec_block>(this, 0)); // 1?
			Add(SharedCaches = new TI.Block<shared_cache_block>(this, 0)); // 3?
			Add(Pages = new TI.Block<pages_block>(this, 0));
			Add(Block24 = new TI.Block<cache_file_resource_layout_table_24_block>(this, 0));
			Add(PageSegments = new TI.Block<page_segment_block>(this, 0));
		}
	};

	[TI.TagGroup((int)TagGroups.Enumerated.play, 1, 60)]
	public class cache_file_resource_layout_table_group : TI.Definition
	{
		public TI.Struct<cache_file_resource_layout_table> ResourceLayoutTable;

		public cache_file_resource_layout_table_group()
		{
			Add(ResourceLayoutTable = new TI.Struct<cache_file_resource_layout_table>(this));
		}
	};
	#endregion

	enum resource_fixup_type
	{
		Invalid,

		Data,	// fixup is to data located in the 'resource definition' tag data
		CacheRequired,	// fixup is to data located in the cache file resource page
		CacheOptional,

		Count,
	};

	#region cache_file_resource_gestalt
	[TI.TagGroup((int)TagGroups.Enumerated.zone, 1, 532)]
	public partial class cache_file_resource_gestalt_group : TI.Definition
	{
		#region resource_type_block
		[TI.Definition(1, 28)]
		public partial class resource_type_block : TI.Definition
		{
			public TI.Skip Guid;
			public TI.ShortInteger Unknown010;
			public TI.ShortInteger Unknown012;
			public TI.ShortInteger Unknown014;
			public TI.ShortInteger Unknown016;
			public TI.StringId Name;

			#region Ctor
			public resource_type_block()
			{
				Add(Guid = new TI.Skip(16));
				Add(Unknown010 = new TI.ShortInteger());
				Add(Unknown012 = new TI.ShortInteger());
				Add(Unknown014 = new TI.ShortInteger());
				Add(Unknown016 = new TI.ShortInteger());
				Add(Name = new TI.StringId());
			}
			#endregion
		};
		#endregion

		#region resource_structure_type_block
		[TI.Definition(1, 20)]
		public partial class resource_structure_type_block : TI.Definition
		{
			public TI.Skip Guid;
			public TI.StringId Name;

			#region Ctor
			public resource_structure_type_block()
			{
				Add(Guid = new TI.Skip(16));
				Add(Name = new TI.StringId());
			}
			#endregion
		};
		#endregion

		#region cache_file_resource_gestalt_tag_resource_block
		[TI.Definition(1, 64)]
		public partial class cache_file_resource_gestalt_tag_resource_block : TI.Definition
		{
			#region resource_fixup_block
			[TI.Definition(1, 8)]
			public partial class resource_fixup_block : TI.Definition
			{
				public TI.LongInteger BlockOffset; // [offset in tag data sub-buffer]
				public TI.LongInteger Address; // fix-up address (either in the data sub-buffer, or a cache-resource offset)

				#region Ctor
				public resource_fixup_block()
					: base(2)
				{
					Add(BlockOffset = new TI.LongInteger());
					Add(Address = new TI.LongInteger());
				}
				#endregion
			}
			#endregion

			#region resource_definition_fixup_block
			[TI.Definition(1, 8)]
			public partial class resource_definition_fixup_block : TI.Definition
			{
				public TI.LongInteger Offset; // offset in the data sub-buffer
				public TI.BlockIndex StructureTypeIndex;

				#region Ctor
				public resource_definition_fixup_block()
				{
					Add(Offset = new TI.LongInteger());
					Add(StructureTypeIndex = new TI.BlockIndex(BlamLib.TagInterface.FieldType.LongBlockIndex));
				}
				#endregion
			}
			#endregion

			public TI.TagReference Reference;
			public TI.ShortInteger Header;
			public TI.BlockIndex ResourceType;
			public TI.Flags Flags; // ?
			public TI.LongInteger BlockOffset, BlockSize, UnknownOffset; // (in the tag data field)
			public TI.ShortInteger Unknown020; // if this is <= 0, this block is ignored (and probably invalid)
			public TI.ShortInteger SegmentIndex; // 5th tag block in cache_file_resource_layout_table, page_segment_block
			// eg, if this is a structure bsp resource, this will point to the main
			// tag block in the definition buffer
			public TI.LongInteger DefinitionOffset; // resource definition offset, uses fixup ptr scheme
			public TI.Block<resource_fixup_block> ResourceFixups;
			public TI.Block<resource_definition_fixup_block> ResourceDefinitionFixups;

			#region Ctor
			public cache_file_resource_gestalt_tag_resource_block()
			{
				Add(Reference = new TI.TagReference(this));
				Add(Header = new TI.ShortInteger());
				Add(ResourceType = new TI.BlockIndex(BlamLib.TagInterface.FieldType.ByteBlockIndex));
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
				Add(BlockOffset = new TI.LongInteger());
				Add(BlockSize = new TI.LongInteger());
				Add(UnknownOffset = new TI.LongInteger());
				Add(Unknown020 = new TI.ShortInteger());
				Add(SegmentIndex = new TI.ShortInteger());
				Add(DefinitionOffset = new TI.LongInteger());
				Add(ResourceFixups = new TI.Block<resource_fixup_block>(this, 0));
				Add(ResourceDefinitionFixups = new TI.Block<resource_definition_fixup_block>(this, 0));
			}
			#endregion
		};
		#endregion

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

		#region cache_file_resource_gestalt_100_block
		[TI.Definition(1, 36)]
		public partial class cache_file_resource_gestalt_100_block : TI.Definition
		{
			public TI.StringId Name;
			public TI.LongInteger Unknown04,
				Unknown08,
				Unknown0C,
				Unknown10,
				Unknown14,
				Unknown18,
				Unknown1C;
			public TI.BlockIndex PrevZoneSet;

			public cache_file_resource_gestalt_100_block() : base(9)
			{
				Add(Name = new TI.StringId());
				Add(Unknown04 = new TI.LongInteger());

				Add(Unknown08 = new TI.LongInteger());
				Add(Unknown0C = new TI.LongInteger());

				Add(Unknown10 = new TI.LongInteger());
				Add(Unknown14 = new TI.LongInteger());

				Add(Unknown18 = new TI.LongInteger());

				Add(Unknown1C = new TI.LongInteger()); // seems to be the same value as the the long after the name field

				Add(PrevZoneSet = new TI.BlockIndex(BlamLib.TagInterface.FieldType.LongBlockIndex));
			}
		}
		#endregion

		#region cache_file_resource_gestalt_164_block
		[TI.Definition(1, 20)]
		public partial class cache_file_resource_gestalt_164_block : TI.Definition
		{
			public TI.LongInteger Unknown00,
				Unknown04,
				Unknown08,
				Unknown0C,
				Unknown10;

			public cache_file_resource_gestalt_164_block() : base(5)
			{
				Add(Unknown00 = new TI.LongInteger());
				Add(Unknown04 = new TI.LongInteger());
				Add(Unknown08 = new TI.LongInteger());
				Add(Unknown0C = new TI.LongInteger());
				Add(Unknown10 = new TI.LongInteger());
			}
		}
		#endregion

		#region cache_file_resource_gestalt_1DC_block
		[TI.Definition(1, 8)]
		public partial class cache_file_resource_gestalt_1DC_block : TI.Definition
		{
			public TI.ShortInteger ThisIndex; // The index of this block element
			public TI.ShortInteger Count;
			public TI.BlockIndex BlockIndex;

			public cache_file_resource_gestalt_1DC_block() : base(3)
			{
				Add(ThisIndex = new TI.ShortInteger());
				Add(Count = new TI.ShortInteger());
				Add(BlockIndex = new TI.BlockIndex(TI.FieldType.LongBlockIndex)); // cache_file_resource_gestalt_1D0_block
			}
		}
		#endregion

		#region cache_file_resource_gestalt_1E8_block
		[TI.Definition(1, 4)]
		public partial class cache_file_resource_gestalt_1E8_block : TI.Definition
		{
			public TI.ShortInteger Unknown00; // index?
			public TI.ShortInteger Unknown02; // index?

			public cache_file_resource_gestalt_1E8_block() : base(2)
			{
				Add(Unknown00 = new TI.ShortInteger());
				Add(Unknown02 = new TI.ShortInteger());
			}
		}
		#endregion

		#region cache_file_resource_gestalt_1F4_block
		[TI.Definition(1, 8)]
		public partial class cache_file_resource_gestalt_1F4_block : TI.Definition
		{
			public TI.ShortInteger Unknown00; // something like length
			public TI.ShortInteger Unknown02; // something like start index, block index to cache_file_resource_gestalt_1E8_block

			public TI.ShortInteger Unknown04; // something like length
			public TI.ShortInteger Unknown06; // something like start index, block index to cache_file_resource_gestalt_1E8_block

			public cache_file_resource_gestalt_1F4_block() : base(4)
			{
				Add(Unknown00 = new TI.ShortInteger());
				Add(Unknown02 = new TI.ShortInteger());
				Add(Unknown04 = new TI.ShortInteger());
				Add(Unknown06 = new TI.ShortInteger());
			}
		}
		#endregion

		#region cache_file_resource_gestalt_200_block
		[TI.Definition(1, 12)]
		public partial class cache_file_resource_gestalt_200_block : TI.Definition
		{
			public TI.LongInteger TagIndex; // DatumIndex
			public TI.LongInteger Unknown08;
			public TI.LongInteger Unknown0C; // 0x0000FF00: BSP index; 0x000000FF: index

			public cache_file_resource_gestalt_200_block() : base(3)
			{
				Add(TagIndex = new TI.LongInteger());
				Add(Unknown08 = new TI.LongInteger());
				Add(Unknown0C = new TI.LongInteger());
			}
		}
		#endregion

		#region Fields
		public TI.Enum CacheType;
		public TI.Flags Flags;
		public TI.Block<resource_type_block> ResourceTypes;
		public TI.Block<resource_structure_type_block> ResourceStructureTypes;
		public TI.Struct<cache_file_resource_layout_table> ResourceLayoutTable;
		public TI.Block<cache_file_resource_gestalt_tag_resource_block> TagResources;
		public TI.Block<cache_file_resource_gestalt_64_block> Block64,
			Block70, Block7C, Block88, Block94, BlockA0, BlockAC,
			BlockB8, BlockC4, BlockD0, BlockDC;

		public TI.Block<cache_file_resource_gestalt_100_block> Block100;
		public TI.Block<field_block<TI.TagReference>> BspReferences;

		public TI.Data ResourceDefinitionData;

		public TI.Block<cache_file_resource_gestalt_164_block> Block164;

		public TI.Block<field_block<TI.LongInteger>> Block1D0;
		public TI.Block<cache_file_resource_gestalt_1DC_block> Block1DC;
		public TI.Block<cache_file_resource_gestalt_1E8_block> Block1E8;
		public TI.Block<cache_file_resource_gestalt_1F4_block> Block1F4;
		public TI.Block<cache_file_resource_gestalt_200_block> Block200;

		public TI.Skip MapId;
		#endregion

		public cache_file_resource_gestalt_group()
		{
			// sound_resource_definition [0x?]
				// tag_data data

			Add(CacheType = new TI.Enum());
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
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