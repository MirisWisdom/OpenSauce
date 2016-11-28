/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region sandbox_text_value_pair_definition
	[TI.TagGroup((int)TagGroups.Enumerated.jmrq, 1, 12)]
	public class sandbox_text_value_pair_definition_group : TI.Definition
	{
		#region sandbox_text_value_pair_value_block
		[TI.Definition(1, 16)]
		public class sandbox_text_value_pair_value_block : TI.Definition
		{
			#region sandbox_text_value_pair_reference_block
			[TI.Definition(1, 20)]
			public class sandbox_text_value_pair_reference_block : TI.Definition
			{
				#region Fields
				public TI.Flags Flags;
				public TI.Enum Type;
				public TI.LongInteger Integer;
				public TI.StringId StringId;
				public TI.StringId Name, Description;
				#endregion

				public sandbox_text_value_pair_reference_block()
				{
					Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
					Add(Type = new TI.Enum(TI.FieldType.ByteEnum));
					Add(TI.Pad.Word);
					Add(Integer = new TI.LongInteger());
					Add(StringId = new TI.StringId());
					Add(Name = new TI.StringId());
					Add(Description = new TI.StringId());
				}
			};
			#endregion

			#region Fields
			public TI.StringId Name;
			public TI.Block<sandbox_text_value_pair_reference_block> Values;
			#endregion

			public sandbox_text_value_pair_value_block()
			{
				Add(Name = new TI.StringId());
				Add(Values = new TI.Block<sandbox_text_value_pair_reference_block>(this, 32));
			}
		};
		#endregion

		#region Fields
		public TI.Block<sandbox_text_value_pair_value_block> Values;
		#endregion

		public sandbox_text_value_pair_definition_group()
		{
			Add(Values = new TI.Block<sandbox_text_value_pair_value_block>(this, 32));
		}
	};
	#endregion

	#region text_value_pair_definition
	[TI.TagGroup((int)TagGroups.Enumerated.sily, 2, 24)]
	public class text_value_pair_definition_group : TI.Definition
	{
		#region text_value_pair_reference_block
		[TI.Definition(2, 20)]
		public class text_value_pair_reference_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.Enum Type;
			public TI.LongInteger Integer;
			public TI.StringId StringId;
			public TI.StringId Name, Description;
			#endregion

			public text_value_pair_reference_block()
			{
				Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
				Add(Type = new TI.Enum(TI.FieldType.ByteEnum));
				Add(TI.Pad.Word);
				Add(Integer = new TI.LongInteger());
				Add(StringId = new TI.StringId());
				Add(Name = new TI.StringId());
				Add(Description = new TI.StringId());
			}
		};
		#endregion

		#region Fields
		public TI.Enum Parameter;
		public TI.StringId Title, Description;
		public TI.Block<text_value_pair_reference_block> Values;
		#endregion

		public text_value_pair_definition_group()
		{
			Add(Parameter = new TI.Enum(TI.FieldType.LongEnum)); // 0x26C options, last 0x15 are templated options and can't be get\set
			Add(Title = new TI.StringId());
			Add(Description = new TI.StringId());
			Add(Values = new TI.Block<text_value_pair_reference_block>(this, 32));
		}
	};
	#endregion

	#region user_interface_shared_globals_definition
	[TI.TagGroup((int)TagGroups.Enumerated.wigl, 2, 652)]
	public class user_interface_shared_globals_definition_group : TI.Definition
	{
		#region ui_message_category_block
		[TI.Definition(2, 16)]
		public class ui_message_category_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ui_message_category_block()
			{
				Add(/*category name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*default button = */ new TI.Enum(TI.FieldType.ByteEnum));
				// TODO: Enum?
				Add(new TI.Skip(2));
				Add(/*default title = */ new TI.StringId());
				Add(/*default message = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region ui_error_category_block
		[TI.Definition(2, 40)]
		public class ui_error_category_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ui_error_category_block()
			{
				Add(/*category name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*default button = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(1));
				Add(/*default title = */ new TI.StringId());
				Add(/*default message = */ new TI.StringId());
				Add(/*default ok = */ new TI.StringId());
				Add(/*default cancel = */ new TI.StringId());
				Add(/*? = */ new TI.StringId());
				Add(/*? = */ new TI.StringId());
				Add(/*? = */ new TI.StringId());
				// TODO: IDK
				Add(/*? = */ new TI.ShortInteger());
				Add(/*? = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public user_interface_shared_globals_definition_group()
		{
			Add(/*inc. text update period = */ new TI.ShortInteger());
			Add(/*inc. text block character = */ new TI.ShortInteger());
			Add(/*near clip plane distance = */ new TI.Real());
			Add(/*projection plane distance = */ new TI.Real());
			Add(/*far clip plane distance = */ new TI.Real());
			Add(/*unicode string list tag = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*damage strings = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*main menu music = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*music fade time = */ new TI.LongInteger());
			//RealArgbColor
			//RealArgbColor
			// tag block [0x14]
				// string id [name]
				// RealArgbColor [color]
			// tag block [0x40]
				// RealArgbColor
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
			// tag reference [uise] [default sounds]
			Add(/*messages = */ new TI.Block<ui_message_category_block>(this, 0));
			Add(/*errors = */ new TI.Block<ui_error_category_block>(this, 100));
			// tag block [0x10] data sources
				// tag reference [dsrc]
		}
		#endregion
	};
	#endregion
}