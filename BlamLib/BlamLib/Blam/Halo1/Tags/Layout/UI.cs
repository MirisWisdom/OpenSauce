/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region input_device_defaults
	public partial class input_device_defaults_group
	{
		public input_device_defaults_group() : base(4)
		{
			Add(DeviceType = new TI.Enum());
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(DeviceId = new TI.Data(this));
			Add(Profile = new TI.Data(this));
		}
	};
	#endregion

	#region multiplayer_scenario_description
	public partial class multiplayer_scenario_description_group
	{
		#region scenario_description_block
		public partial class scenario_description_block
		{
			public scenario_description_block() : base(4)
			{
				Add(DescriptiveBitmap = new TI.TagReference(this, TagGroups.bitm));
				Add(DisplayedMapName = new TI.TagReference(this, TagGroups.ustr));
				Add(ScenarioTagDirPath = new TI.String());
				Add(new TI.Pad(4));
			}
		};
		#endregion

		public multiplayer_scenario_description_group() : base(1)
		{
			Add(Scenarios = new TI.Block<scenario_description_block>(this, 32));
		}
	};
	#endregion

	#region ui_widget_definition
	public partial class ui_widget_definition_group
	{
		#region game_data_input_references_block
		public partial class game_data_input_references_block
		{
			public game_data_input_references_block() : base(2)
			{
				Add(Function = new TI.Enum());
				Add(new TI.Pad(2 + 32));
			}
		};
		#endregion

		#region event_handler_references_block
		public partial class event_handler_references_block
		{
			public event_handler_references_block() : base(6)
			{
				Add(Flags = new TI.Flags());
				Add(EventType = new TI.Enum());
				Add(Function = new TI.Enum());
				Add(WidgetTag = new TI.TagReference(this, TagGroups.DeLa));
				Add(SoundEffect = new TI.TagReference(this, TagGroups.snd_));
				Add(Script = new TI.String());
			}
		};
		#endregion

		#region search_and_replace_reference_block
		public partial class search_and_replace_reference_block
		{
			public search_and_replace_reference_block() : base(2)
			{
				Add(SearchString = new TI.String());
				Add(ReplaceFunction = new TI.Enum());
			}
		};
		#endregion

		#region conditional_widget_reference_block
		public partial class conditional_widget_reference_block
		{
			public conditional_widget_reference_block() : base(5)
			{
				Add(WidgetTag = new TI.TagReference(this, TagGroups.DeLa));
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(CustomControllerIndex = new TI.ShortInteger());
				Add(new TI.Pad(26));
			}
		};
		#endregion

		#region child_widget_reference_block
		public partial class child_widget_reference_block
		{
			public child_widget_reference_block() : base(7)
			{
				Add(WidgetTag = new TI.TagReference(this, TagGroups.DeLa));
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(CustomControllerIndex = new TI.ShortInteger());
				Add(VerticalOffset = new TI.ShortInteger());
				Add(HorizontalOffset = new TI.ShortInteger());
				Add(new TI.Pad(22));
			}
		};
		#endregion

		public ui_widget_definition_group() : base(33)
		{
			Add(WidgetType = new TI.Enum());
			Add(ControllerIndex = new TI.Enum());
			Add(Name = new TI.String());
			Add(Bounds = new TI.Rectangle2D());
			Add(Flags = new TI.Flags());
			Add(MillisecondsToAutoClose = new TI.LongInteger());
			Add(MillisecondsAutoCloseFadeTime = new TI.LongInteger());
			Add(BackgroundBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(GameDataInputs = new TI.Block<game_data_input_references_block>(this, 64));
			Add(EventHandlers = new TI.Block<event_handler_references_block>(this, 32));
			Add(SearchAndReplaceFunctions = new TI.Block<search_and_replace_reference_block>(this, 32));
			Add(new TI.Pad(128));

			Add(TextBoxLabel = new TI.TagReference(this, TagGroups.ustr));
			Add(TextBoxFont = new TI.TagReference(this, TagGroups.font));
			Add(TextBoxColor = new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(TextBoxJustification = new TI.Enum());
			Add(TextBoxFlags = new TI.Flags());
			Add(new TI.Pad(12));
			Add(TextBoxStringListIndex = new TI.ShortInteger());
			Add(TextBoxHorizOffset = new TI.ShortInteger());
			Add(TextBoxVertOffset = new TI.ShortInteger());
			Add(new TI.Pad(26 + 2));

			Add(ListItemsFlags = new TI.Flags());
			Add(SpinnerListHeaderBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(SpinnerListFooterBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(SpinnerListHeaderBounds = new TI.Rectangle2D());
			Add(SpinnerListFooterBounds = new TI.Rectangle2D());
			Add(new TI.Pad(32));

			Add(ColumnListExtendedDescriptionWidget = new TI.TagReference(this, TagGroups.DeLa));
			Add(new TI.Pad(32 + 256));

			Add(ConditionalWidgets = new TI.Block<conditional_widget_reference_block>(this, 32));
			Add(new TI.Pad(128 + 128));
			Add(ChildWidgets = new TI.Block<child_widget_reference_block>(this, 32));
		}
	};
	#endregion

	#region virtual_keyboard
	public partial class virtual_keyboard_group
	{
		#region virtual_key_block
		public partial class virtual_key_block
		{
			public virtual_key_block() : base(12)
			{
				Add(KeyboardKey = new TI.Enum());
				Add(CharacterLowerCase = new TI.ShortInteger());
				Add(CharacterShift = new TI.ShortInteger());
				Add(CharacterCaps = new TI.ShortInteger());
				Add(CharacterSymbols = new TI.ShortInteger());
				Add(CharacterShiftCaps = new TI.ShortInteger());
				Add(CharacterShiftSymbols = new TI.ShortInteger());
				Add(CharacterCapsSymbols = new TI.ShortInteger());
				Add(BackgroundBitmapUnselected = new TI.TagReference(this, TagGroups.bitm));
				Add(BackgroundBitmapSelected = new TI.TagReference(this, TagGroups.bitm));
				Add(BackgroundBitmapActive = new TI.TagReference(this, TagGroups.bitm));
				Add(BackgroundBitmapSticky = new TI.TagReference(this, TagGroups.bitm));
			}
		};
		#endregion

		public virtual_keyboard_group() : base(4)
		{
			Add(DisplayFont = new TI.TagReference(this, TagGroups.font));
			Add(BackgroundBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(SpecialKeyLabelsStringList = new TI.TagReference(this, TagGroups.ustr));
			Add(VirtualKeys = new TI.Block<virtual_key_block>(this, 44));
		}
	};
	#endregion
}