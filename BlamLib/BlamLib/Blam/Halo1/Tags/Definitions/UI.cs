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
	[TI.TagGroup((int)TagGroups.Enumerated.devc, 1, 44)]
	public partial class input_device_defaults_group : TI.Definition
	{
		public TI.Enum DeviceType;
		private TI.Flags Flags;
		public TI.Data DeviceId, Profile;
	};
	#endregion

	#region multiplayer_scenario_description
	[TI.TagGroup((int)TagGroups.Enumerated.mply, 1, 12)]
	public partial class multiplayer_scenario_description_group : TI.Definition
	{
		#region scenario_description_block
		[TI.Definition(-1, 68)]
		public partial class scenario_description_block : TI.Definition
		{
			public TI.TagReference DescriptiveBitmap;
			public TI.TagReference DisplayedMapName;
			public TI.String ScenarioTagDirPath;
		};
		#endregion

		public TI.Block<scenario_description_block> Scenarios;
	};
	#endregion

	#region ui_widget_collection
	[TI.TagGroup((int)TagGroups.Enumerated.Soul, 1, 12)]
	public partial class ui_widget_collection_group : TI.Definition
	{
		// ui_widget_references_block, field_block<TI.TagReference>

		public TI.Block<field_block<TI.TagReference>> Definitions;

		public ui_widget_collection_group() : base(1)
		{
			Add(Definitions = new TI.Block<field_block<TI.TagReference>>(this, 32));

			//Definitions.Definition.Value.ResetReferenceGroupTag(TagGroups.DeLa);
		}
	};
	#endregion

	#region ui_widget_definition
	[TI.TagGroup((int)TagGroups.Enumerated.DeLa, 1, 1004)]
	public partial class ui_widget_definition_group : TI.Definition
	{
		#region game_data_input_references_block
		[TI.Definition(-1, 36)]
		public partial class game_data_input_references_block : TI.Definition
		{
			public TI.Enum Function;
		};
		#endregion

		#region event_handler_references_block
		[TI.Definition(-1, 72)]
		public partial class event_handler_references_block : TI.Definition
		{
			public TI.Flags Flags;
			public TI.Enum EventType;
			public TI.Enum Function;
			public TI.TagReference WidgetTag;
			public TI.TagReference SoundEffect;
			public TI.String Script;
		};
		#endregion

		#region search_and_replace_reference_block
		[TI.Definition(-1, 34)]
		public partial class search_and_replace_reference_block : TI.Definition
		{
			public TI.String SearchString;
			public TI.Enum ReplaceFunction;
		};
		#endregion

		#region conditional_widget_reference_block
		[TI.Definition(-1, 80)]
		public partial class conditional_widget_reference_block : TI.Definition
		{
			public TI.TagReference WidgetTag;
			public TI.String Name;
			public TI.Flags Flags;
			public TI.ShortInteger CustomControllerIndex;
		};
		#endregion

		#region child_widget_reference_block
		[TI.Definition(-1, 80)]
		public partial class child_widget_reference_block : TI.Definition
		{
			public TI.TagReference WidgetTag;
			public TI.String Name;
			public TI.Flags Flags;
			public TI.ShortInteger CustomControllerIndex;
			public TI.ShortInteger VerticalOffset;
			public TI.ShortInteger HorizontalOffset;
		};
		#endregion

		public TI.Enum WidgetType;
		public TI.Enum ControllerIndex;
		public TI.String Name;
		public TI.Rectangle2D Bounds;
		public TI.Flags Flags;
		public TI.LongInteger MillisecondsToAutoClose;
		public TI.LongInteger MillisecondsAutoCloseFadeTime;
		public TI.TagReference BackgroundBitmap;
		public TI.Block<game_data_input_references_block> GameDataInputs;
		public TI.Block<event_handler_references_block> EventHandlers;
		public TI.Block<search_and_replace_reference_block> SearchAndReplaceFunctions;

		public TI.TagReference TextBoxLabel;
		public TI.TagReference TextBoxFont;
		public TI.RealColor TextBoxColor;
		public TI.Enum TextBoxJustification;
		public TI.Flags TextBoxFlags;
		public TI.ShortInteger TextBoxStringListIndex;
		public TI.ShortInteger TextBoxHorizOffset;
		public TI.ShortInteger TextBoxVertOffset;

		public TI.Flags ListItemsFlags;
		public TI.TagReference SpinnerListHeaderBitmap;
		public TI.TagReference SpinnerListFooterBitmap;
		public TI.Rectangle2D SpinnerListHeaderBounds;
		public TI.Rectangle2D SpinnerListFooterBounds;

		public TI.TagReference ColumnListExtendedDescriptionWidget;
		public TI.Block<conditional_widget_reference_block> ConditionalWidgets;
		public TI.Block<child_widget_reference_block> ChildWidgets;
	};
	#endregion

	#region virtual_keyboard
	[TI.TagGroup((int)TagGroups.Enumerated.vcky, 2, 60)]
	public partial class virtual_keyboard_group : TI.Definition
	{
		#region virtual_key_block
		[TI.Definition(-1, 80)]
		public partial class virtual_key_block : TI.Definition
		{
			public TI.Enum KeyboardKey;
			public TI.ShortInteger CharacterLowerCase, CharacterShift, CharacterCaps, CharacterSymbols,
				CharacterShiftCaps, CharacterShiftSymbols, CharacterCapsSymbols;
			public TI.TagReference BackgroundBitmapUnselected, BackgroundBitmapSelected, BackgroundBitmapActive,
				BackgroundBitmapSticky;
		};
		#endregion

		public TI.TagReference DisplayFont, BackgroundBitmap, SpecialKeyLabelsStringList;
		public TI.Block<virtual_key_block> VirtualKeys;
	};
	#endregion
}