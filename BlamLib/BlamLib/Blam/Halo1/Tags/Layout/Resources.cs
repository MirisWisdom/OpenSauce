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
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	public partial class predicted_resource_block
	{
		public predicted_resource_block() : base(3)
		{
			Add(Type = new TI.Enum());
			Add(ResourceIndex = new TI.ShortInteger());
			Add(TagIndex = new TI.LongInteger());
		}
	};

	#region bitmap
	public partial class bitmap_group
	{
		#region bitmap_group_sequence_block
		public partial class bitmap_group_sequence_block
		{
			#region bitmap_group_sprite_block
			public partial class bitmap_group_sprite_block
			{
				public bitmap_group_sprite_block() : base(7)
				{
					Add(BitmapIndex = new TI.ShortInteger());
					Add(new TI.Pad(2 + 4));
					Add(Left = new TI.Real());
					Add(Right = new TI.Real());
					Add(Top = new TI.Real());
					Add(Bottom = new TI.Real());
					Add(RegistrationPoint = new TI.RealPoint2D());
				}
			};
			#endregion

			public bitmap_group_sequence_block() : base(5)
			{
				Add(Name = new TI.String());
				Add(FirstBitmapIndex = new TI.ShortInteger());
				Add(BitmapCount = new TI.ShortInteger());
				Add(Sprites = new TI.Block<bitmap_group_sprite_block>(this, 64));
				Add(new TI.Pad(16));
			}
		};
		#endregion

		#region bitmap_data_block
		public partial class bitmap_data_block
		{
			public bitmap_data_block() : base(14)
			{
				Add(Signature = new TI.Tag());
				Add(Width = new TI.ShortInteger());
				Add(Height = new TI.ShortInteger());
				Add(Depth = new TI.ShortInteger());
				Add(Type = new TI.Enum());
				Add(Format = new TI.Enum());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(RegistrationPoint = new TI.Point2D());
				Add(MipmapCount = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(PixelsOffset = new TI.LongInteger());
				Add(new TI.Skip(12));
				Add(HardwareFormat = new TI.LongInteger());
				Add(BaseAddress = new TI.LongInteger());
			}
		};
		#endregion

		public bitmap_group() : base(21)
		{
			Add(Type = new TI.Enum());
			Add(Format = new TI.Enum());
			Add(Usage = new TI.Enum());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));

			Add(DetailFadeFactor = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(SharpenAmount = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(BumpHeight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));

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
			Add(new TI.Pad(2));

			Add(Sequences = new TI.Block<bitmap_group_sequence_block>(this, 256));
			Add(Bitmaps = new TI.Block<bitmap_data_block>(this, 2048));
		}
	};
	#endregion


	#region tag_database_entry_reference_block
	public partial class tag_database_entry_reference_block
	{
		public tag_database_entry_reference_block() : base(1)
		{
			Add(EntryReference = new TI.BlockIndex(TI.FieldType.LongBlockIndex)); // tag_database_entry_block
		}
	};
	#endregion

	#region tag_database_entry_block
	public partial class tag_database_entry_block
	{
		public tag_database_entry_block() : base(4)
		{
			Add(Name = new TI.Data(this, TI.DataType.String));
			Add(GroupTag = new TI.Tag());
			Add(HandleDataHigh = new TI.LongInteger());
			Add(Flags = new TI.LongInteger());
			Add(ChildIds = new TI.Block<tag_database_entry_reference_block>(this, 65535));
			Add(ReferenceIds = new TI.Block<tag_database_entry_reference_block>(this, 65535));

			Add(new TI.Pad(12));
		}
	};
	#endregion

	#region tag_database
	public partial class tag_database_group : TI.Definition
	{
		public tag_database_group() : base(1)
		{
			Add(Entries = new TI.Block<tag_database_entry_block>(this, 65535));

			Add(new TI.Pad(24));
		}
	};
	#endregion
}