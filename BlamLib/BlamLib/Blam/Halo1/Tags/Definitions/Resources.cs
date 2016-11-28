/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	[TagInterface.Definition(1, 8)]
	public partial class predicted_resource_block : Cache.predicted_resource_block
	{
	};

	#region bitmap
	[TI.TagGroup((int)TagGroups.Enumerated.bitm, 7, 108)]
	public partial class bitmap_group : TI.Definition
	{
		#region bitmap_group_sequence_block
		[TI.Definition(-1, 64)]
		public partial class bitmap_group_sequence_block : TI.Definition
		{
			#region bitmap_group_sprite_block
			[TI.Definition(-1, 32)]
			public partial class bitmap_group_sprite_block : TI.Definition
			{
				public TI.ShortInteger BitmapIndex; // ShortBlockIndex what what
				public TI.Real Left;
				public TI.Real Right;
				public TI.Real Top;
				public TI.Real Bottom;
				public TI.RealPoint2D RegistrationPoint;
			};
			#endregion

			public TI.String Name;
			public TI.ShortInteger FirstBitmapIndex;
			public TI.ShortInteger BitmapCount;
			public TI.Block<bitmap_group_sprite_block> Sprites;
		};
		#endregion

		#region bitmap_data_block
		[TI.Definition(-1, 48)]
		public partial class bitmap_data_block : Bitmaps.bitmap_data_block
		{
			public TI.ShortInteger Depth;

			public TI.ShortInteger MipmapCount;

			public TI.LongInteger HardwareFormat;
			public TI.LongInteger BaseAddress;
		};
		#endregion

		public TI.Enum Type;
		public TI.Enum Format;
		public TI.Enum Usage;
		public TI.Flags Flags;

		public TI.Real DetailFadeFactor;
		public TI.Real SharpenAmount;
		public TI.Real BumpHeight;

		public TI.Enum SpriteBudgetSize;
		public TI.ShortInteger SpriteBudgetCount;

		public TI.ShortInteger ColorPlateWidth;
		public TI.ShortInteger ColorPlateHeight;
		public TI.Data CompressedColorPlateData;
		public TI.Data ProcessedPixelData;

		public TI.Real BlurFilterSize;
		public TI.Real AlphaBias;
		public TI.ShortInteger MipmapCount;

		public TI.Enum SpriteUsage;
		public TI.ShortInteger SpriteSpacing;

		public TI.Block<bitmap_group_sequence_block> Sequences;
		public TI.Block<bitmap_data_block> Bitmaps;
	};
	#endregion

	#region string_list
	[TI.TagGroup((int)TagGroups.Enumerated.str_, 1, 12)]
	public partial class string_list_group : TI.Definition
	{
		#region string reference
		[TI.Definition(-1, 20)]
		public partial class string_reference_block : TI.Definition
		{
			public string_reference_block() : base(1)
			{
				Add(/*string = */ new TI.Data(this, TI.DataType.String));
			}
		}
		#endregion

		public TI.Block<string_reference_block> StringReferences;

		public string_list_group() : base(1)
		{
			Add(StringReferences = new TI.Block<string_reference_block>(this, 200));
		}
	};
	#endregion

	#region tag_collection
	[TI.TagGroup((int)TagGroups.Enumerated.tagc, 1, 12)]
	public partial class tag_collection_group : TI.Definition
	{
		// tag_reference_block, field_block<TI.TagReference>

		public TI.Block<field_block<TI.TagReference>> References;

		public tag_collection_group() : base(1)
		{
			Add(References = new TI.Block<field_block<TI.TagReference>>(this, 200));

			//References.Definition.Value.ResetGroupTag(TI.TagGroup.Null);
		}
	};
	#endregion

	#region unicode_string_list
	[TI.TagGroup((int)TagGroups.Enumerated.ustr, 1, 12)]
	public partial class unicode_string_list_group : TI.Definition
	{
		#region string reference
		[TI.Definition(-1, 20)]
		public partial class string_reference_block : TI.Definition
		{
			public string_reference_block() : base(1)
			{
				Add(/*string = */ new TI.Data(this, TI.DataType.Unicode));
			}
		}
		#endregion

		public TI.Block<string_reference_block> StringReferences;

		public unicode_string_list_group() : base(1)
		{
			Add(StringReferences = new TI.Block<string_reference_block>(this, 800));
		}
	};
	#endregion


	#region tag_database_entry_reference_block
	[TI.Definition(1, 4)]
	public partial class tag_database_entry_reference_block : TI.Definition
	{
		public TI.BlockIndex EntryReference;
	}
	#endregion

	#region tag_database_entry_block
	[TI.Definition(1, 68)]
	public partial class tag_database_entry_block : TI.Definition
	{
		public TI.Data Name;
		public TI.Tag GroupTag;					// In native code we treat this field and the one below it as a 64-bit handle
		public TI.LongInteger HandleDataHigh;
		public TI.LongInteger Flags;
		public TI.Block<tag_database_entry_reference_block> ChildIds;
		public TI.Block<tag_database_entry_reference_block> ReferenceIds;
	};
	#endregion

	#region tag_database
	[TI.TagGroup((int)TagGroups.Enumerated.tag_, 1, 36)]
	public partial class tag_database_group : TI.Definition, Managers.ITagDatabase
	{
		public TI.Block<tag_database_entry_block> Entries;
	};
	#endregion
}