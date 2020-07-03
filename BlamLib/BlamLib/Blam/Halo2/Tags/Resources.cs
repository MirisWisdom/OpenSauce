/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region bitmap
	[TI.TagGroup((int)TagGroups.Enumerated.bitm, 7, 112)]
	public partial class bitmap_group : TI.Definition
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

				public bitmap_group_sprite_block() : base(7)
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

			public bitmap_group_sequence_block() : base(5)
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
		[TI.Definition(2, 116)]
		public partial class bitmap_data_block : Bitmaps.bitmap_data_block
		{
			#region Fields
			public TI.ByteInteger Depth;
			public TI.Flags MoreFlags;

			public TI.ShortInteger MipmapCount;
			public TI.ShortInteger LowDetailMipmapCount;

			public TI.LongInteger[] Offsets = new TI.LongInteger[3];
			public TI.LongInteger[] Sizes = new TI.LongInteger[3];

			public TI.LongInteger OwnerTagIndex;
			#endregion

			#region Ctor
			public bitmap_data_block() : base(23)
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
				Add(MipmapCount = new TI.ShortInteger());
				Add(LowDetailMipmapCount = new TI.ShortInteger());
				Add(PixelsOffset = new TI.LongInteger());
				
				Add(Offsets[0] = new TI.LongInteger());
				Add(Offsets[1] = new TI.LongInteger());
				Add(Offsets[2] = new TI.LongInteger());
				Add(new TI.Skip(4 + 4 + 4));

				Add(Sizes[0] = new TI.LongInteger());
				Add(Sizes[1] = new TI.LongInteger());
				Add(Sizes[2] = new TI.LongInteger());
				Add(new TI.Skip(4 + 4 + 4));

				Add(new TI.Skip(
					4 + 4 + 
					4 // this is special...
					));
				Add(OwnerTagIndex = new TI.LongInteger());
				Add(new TI.Skip(
					20 +
					4
					));
			}
			#endregion
		};
		#endregion

		#region Fields
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
		public TI.Enum ForceFormat;

		public TI.Block<bitmap_group_sequence_block> Sequences;
		public TI.Block<bitmap_data_block> Bitmaps;

		public TI.ByteInteger ColorCompressionQuality;
		public TI.ByteInteger AlphaCompressionQuality;
		public TI.ByteInteger Overlap;
		public TI.Enum ColorSubsampling;
		#endregion

		#region Ctor
		public bitmap_group() : base(25)
		{
			Add(Type = new TI.Enum());
			Add(Format = new TI.Enum());
			Add(Usage = new TI.Enum());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));

			Add(DetailFadeFactor = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(SharpenAmount = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(BumpHeight = new TI.Real());

			Add(SpriteBudgetSize = new TI.Enum());
			Add(SpriteBudgetCount = new TI.ShortInteger());

			Add(ColorPlateWidth = new TI.ShortInteger());
			Add(ColorPlateHeight = new TI.ShortInteger());
			Add(CompressedColorPlateData = new TI.Data(this));
			Add(ProcessedPixelData = new TI.Data(this, BlamLib.TagInterface.DataType.Bitmap));

			Add(BlurFilterSize = new TI.Real());
			Add(AlphaBias = new TI.Real());
			Add(MipmapCount = new TI.ShortInteger());

			Add(SpriteUsage = new TI.Enum());
			Add(SpriteSpacing = new TI.ShortInteger());
			Add(ForceFormat = new TI.Enum());

			Add(Sequences = new TI.Block<bitmap_group_sequence_block>(this, 256));
			Add(Bitmaps = new TI.Block<bitmap_data_block>(this, 65536));

			AddPc(ColorCompressionQuality = new TI.ByteInteger());
			AddPc(AlphaCompressionQuality = new TI.ByteInteger());
			AddPc(Overlap = new TI.ByteInteger());
			AddPc(ColorSubsampling = new TI.Enum(BlamLib.TagInterface.FieldType.ByteEnum));
		}
		#endregion
	};
	#endregion

	#region multilingual_unicode_string_list
	[TI.TagGroup((int)TagGroups.Enumerated.unic, 2, 68)]
	public partial class multilingual_unicode_string_list_group : TI.Definition
	{
		#region multilingual_unicode_string_reference_block
		[TI.Definition(1, 40)]
		public class multilingual_unicode_string_reference_block : TI.Definition
		{
			#region Fields
			public TI.StringId StringId;
			public TI.LongInteger[] LanguageOffsets;
			#endregion

			#region Ctor
			public multilingual_unicode_string_reference_block() : base(10)
			{
				Add(StringId = new TI.StringId());

				LanguageOffsets = new TI.LongInteger[(int)LanguageType.kMax];
				for (int x = 0; x < LanguageOffsets.Length; x++)
					Add(LanguageOffsets[x] = new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<multilingual_unicode_string_reference_block> StringRefs;
		public TI.Data StringData;
		public TI.LongInteger[] LanguageHandles;
		#endregion

		#region Ctor
		public multilingual_unicode_string_list_group() : base(11)
		{
			Add(StringRefs = new TI.Block<multilingual_unicode_string_reference_block>(this, 9216));
			Add(StringData = new TI.Data(this));

			LanguageHandles = new TI.LongInteger[(int)LanguageType.kMax];
			for (int x = 0; x < LanguageHandles.Length; x++)
				Add(LanguageHandles[x] = new TI.LongInteger());

			// Alpha layout (stored in the actual tag data)):
			// tag block [languages]
				// tag block [strings]
					// string id [name]
					// tag data [string data]
		}
		#endregion
	};
	#endregion

	#region tag_database_entry_reference_block
	[TI.Definition(1, 4)]
	public class tag_database_entry_reference_block : TI.Definition
	{
		#region Fields
		public TI.BlockIndex EntryReference;
		#endregion

		#region Ctor
		public tag_database_entry_reference_block() : base(1)
		{
			Add(EntryReference = new TI.BlockIndex(TI.FieldType.LongBlockIndex)); // 1 tag_database_entry_block
		}
		#endregion
	}
	#endregion

	#region tag_database_entry_block
	[TI.Definition(1, 276)]
	public class tag_database_entry_block : TI.Definition
	{
		#region Fields
		public TI.String Name;
		public TI.Tag GroupTag;
		public TI.LongInteger Flags;
		public TI.Block<tag_database_entry_reference_block> ChildIds;
		#endregion

		#region Ctor
		public tag_database_entry_block() : base(4)
		{
			Add(Name = new TI.String(TI.StringType.Ascii, 256));
			Add(GroupTag = new TI.Tag());
			Add(Flags = new TI.LongInteger());
			Add(ChildIds = new TI.Block<tag_database_entry_reference_block>(this, 65535));
		}
		#endregion
	};
	#endregion

	#region tag_database
	[TI.TagGroup((int)TagGroups.Enumerated.tag_, 2, 12)]
	public class tag_database_group : TI.Definition, Managers.ITagDatabase
	{
		#region Fields
		public TI.Block<tag_database_entry_block> Entries;
		#endregion

		#region Ctor
		public tag_database_group()
			: base(1)
		{
			Add(Entries = new TI.Block<tag_database_entry_block>(this, 65535));
		}
		#endregion

		public bool IsEmpty { get { return Entries.Count == 0; } }
	};
	#endregion
}