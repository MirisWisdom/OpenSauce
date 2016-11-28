/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region global_hud_interface_element
	[TI.Struct(-1, -1, 80)]
	public partial class global_hud_interface_element_struct : TI.Definition
	{
		public TI.Point2D AnchorOffset;
		public TI.Real WidthScale, HeightScale;
		public TI.Flags ScalingFlags;
		public TI.TagReference Bitmap;
		public TI.Color DefaultColor, FlashingColor;
		public TI.Real FlashPeriod, FlashDelay;
		public TI.ShortInteger NumberOfFlashes;
		public TI.Flags FlashFlags;
		public TI.Real FlashLength;
		public TI.Color DisabledColor;
	};
	#endregion

	#region global_hud_element
	[TI.Struct(-1, -1, 64)]
	public partial class global_hud_element_struct : TI.Definition
	{
		public TI.Point2D AnchorOffset;
		public TI.Real WidthScale, HeightScale;
		public TI.Flags ScalingFlags;
		public TI.Color DefaultColor, FlashingColor;
		public TI.Real FlashPeriod, FlashDelay;
		public TI.ShortInteger NumberOfFlashes;
		public TI.Flags FlashFlags;
		public TI.Real FlashLength;
		public TI.Color DisabledColor;
	};
	#endregion

	#region global_hud_meter
	[TI.Struct(-1, -1, 104)]
	public partial class global_hud_meter_struct : TI.Definition
	{
		public TI.Point2D AnchorOffset;
		public TI.Real WidthScale, HeightScale;
		public TI.Flags ScalingFlags;
		public TI.TagReference Bitmap;
		public TI.Color ColorAtMeterMin, ColorAtMeterMax, FlashColor, EmptyColor;
		public TI.Flags Flags;
		public TI.ByteInteger MinMeterValue;
		public TI.ShortInteger SequenceIndex;
		public TI.ByteInteger AlphaMul, AlphaBias;
		public TI.ShortInteger ValueScale;
		public TI.Real Opacity, Translucency;
		public TI.Color DisabledColor;
	};
	#endregion

	#region global_hud_color
	[TI.Struct(-1, -1, 28)]
	public partial class global_hud_color_struct : TI.Definition
	{
		public TI.Color DefaultColor;
		public TI.Color FlashingColor;
		public TI.Real FlashPeriod;
		public TI.Real FlashDelay;
		public TI.ShortInteger NumberOfFlashes;
		public TI.Flags FlashFlags;
		public TI.Real FlashLength;
		public TI.Color DisableColor;
	};
	#endregion


	#region global_hud_multitexture_overlay_definition
	[TI.Definition(-1, 480)]
	public partial class global_hud_multitexture_overlay_definition : TI.Definition
	{
		#region global_hud_multitexture_overlay_effector_definition
		[TI.Definition(-1, 220)]
		public partial class global_hud_multitexture_overlay_effector_definition : TI.Definition
		{
			public TI.Enum DestType, Dest, Source;
			
			public TI.RealBounds InBounds, OutBounds;

			public TI.RealColor TintColorLowerBound, TintColorUpperBound;

			public TI.Enum PeriodicFunction;
			public TI.Real FunctionPeriod, FunctionPhase;
		};
		#endregion

		public TI.ShortInteger Type;
		public TI.Enum FrameBufferBlendFunc;

		public TI.Enum AnchorsPrimaryAnchor, AnchorsSecondaryAnchor, AnchorsTertiaryAnchor;

		public TI.Enum BlendFunc0To1, BlendFunc1To2;

		public TI.RealPoint2D MapScalesPrimaryAnchor, MapScalesSecondaryAnchor, MapScalesTertiaryAnchor;

		public TI.RealPoint2D MapOffsetsPrimaryAnchor, MapOffsetsSecondaryAnchor, MapOffsetsTertiaryAnchor;

		public TI.TagReference MapPrimaryAnchor, MapSecondaryAnchor, MapTertiaryAnchor;

		public TI.Enum PrimaryWrapMode, SecondaryWrapMode, TertiaryWrapMode;

		public TI.Block<global_hud_multitexture_overlay_effector_definition> Effectors;
	};
	#endregion

	#region global_hud_sound_block
	[TI.Definition(-1, 56)]
	public partial class global_hud_sound_block : TI.Definition
	{
		public TI.TagReference Sound;
		public TI.Flags LatchedTo;
		public TI.Real Scale;
	};
	#endregion

	#region global_hud_screen_effect_definition
	[TI.Definition(-1, 184)]
	public partial class global_hud_screen_effect_definition : TI.Definition
	{
		public TI.Flags MaskFlags;
		public TI.TagReference MaskFullscreen, MaskSplitscreen;

		public TI.Flags ConvolutionFlags;
		public TI.RealBounds FovInBounds, RadiusOutBounds;

		public TI.Flags NightVisionFlags;
		public TI.ShortInteger NightVisionScriptSource;
		public TI.Real NightVisionIntensity;

		public TI.Flags DesaturationFlags;
		public TI.ShortInteger DesaturationScriptSource;
		public TI.RealColor DesaturationTint;
		public TI.Real DesaturationIntensity;
	};
	#endregion

	#region grenade_hud_interface
	[TI.TagGroup((int)TagGroups.Enumerated.grhi, 1, 504)]
	public partial class grenade_hud_interface_group : TI.Definition
	{
		#region grenade_hud_overlay_block
		[TI.Definition(-1, 136)]
		public partial class grenade_hud_overlay_block : TI.Definition
		{
			public TI.Struct<global_hud_element_struct> Element;

			public TI.Real FrameRate;
			public TI.ShortInteger SequenceIndex;

			public TI.Flags Type;
			public TI.Flags Flags;
		};
		#endregion

		public TI.Enum Anchor;

		#region HudBackground
		public TI.Struct<global_hud_interface_element_struct> HudBackground;

		public TI.ShortInteger HudBackgroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> HudBackgroundMultitexOverlay;
		#endregion

		#region TotalGrenadesBackground
		public TI.Struct<global_hud_interface_element_struct> TotalGrenadesBackground;

		public TI.ShortInteger TotalGrenadesBackgroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> TotalGrenadesBackgroundMultitexOverlay;
		#endregion

		#region TotalGrenadesNumbers
		public TI.Struct<global_hud_element_struct> TotalGrenadesNumbers;

		public TI.ByteInteger TotalGrenadesNumbersMaxNumberOfDigits;
		public TI.Flags TotalGrenadesNumbersFlags;
		public TI.ByteInteger TotalGrenadesNumbersNumberOfFractionalDigits;
		public TI.ShortInteger TotalGrenadesNumbersFlashCutoff;
		#endregion

		public TI.TagReference OverlayBitmap;
		public TI.Block<grenade_hud_overlay_block> Overlays;
		public TI.Block<global_hud_sound_block> WarningSounds;

		public TI.ShortInteger SequenceIndex, WidthOffset;
		public TI.Point2D OffsetFromReferenceCorner;
		public TI.Color OverrideIconColor;
		public TI.ByteInteger FrameRate;
		public TI.Flags Flags;
		public TI.ShortInteger TextIndex;
	};
	#endregion

	#region weapon_hud_interface
	[TI.TagGroup((int)TagGroups.Enumerated.wphi, 2, 380)]
	public partial class weapon_hud_interface_group : TI.Definition
	{
		#region weapon_hud_static_block
		[TI.Definition(-1, 180)]
		public partial class weapon_hud_static_block : TI.Definition
		{
			public TI.Enum StateAttachedTo, CanUseOnMapType;

			public TI.Struct<global_hud_interface_element_struct> Element;

			public TI.ShortInteger SequenceIndex;
			public TI.Block<global_hud_multitexture_overlay_definition> MultitexOverlay;
		};
		#endregion

		#region weapon_hud_meter_block
		[TI.Definition(-1, 180)]
		public partial class weapon_hud_meter_block : TI.Definition
		{
			public TI.Enum StateAttachedTo, CanUseOnMapType;

			public TI.Struct<global_hud_meter_struct> Meter;
		};
		#endregion

		#region weapon_hud_number_block
		[TI.Definition(-1, 160)]
		public partial class weapon_hud_number_block : TI.Definition
		{
			public TI.Enum StateAttachedTo, CanUseOnMapType;

			public TI.Struct<global_hud_element_struct> Element;

			public TI.ByteInteger MaxNumberOfDigits;
			public TI.Flags Flags;
			public TI.ByteInteger NumberOfFractionalDigits;

			public TI.Flags WeaponSpecificFlags;
		};
		#endregion

		#region weapon_hud_crosshair_block
		[TI.Definition(-1, 104)]
		public partial class weapon_hud_crosshair_block : TI.Definition
		{
			#region weapon_hud_crosshair_item_block
			[TI.Definition(-1, 108)]
			public partial class weapon_hud_crosshair_item_block : TI.Definition
			{
				public TI.Struct<global_hud_element_struct> Element;

				public TI.ShortInteger FrameRate, SequenceIndex;

				public TI.Flags Flags;
			};
			#endregion

			public TI.Enum CrosshairType, CanUseOnMapType;
			public TI.TagReference Bitmap;
			public TI.Block<weapon_hud_crosshair_item_block> Overlays;
		};
		#endregion

		#region weapon_hud_overlays_block
		[TI.Definition(-1, 104)]
		public partial class weapon_hud_overlays_block : TI.Definition
		{
			#region weapon_hud_overlay_block
			[TI.Definition(-1, 136)]
			public partial class weapon_hud_overlay_block : TI.Definition
			{
				public TI.Struct<global_hud_element_struct> Element;

				public TI.ShortInteger FrameRate;
				public TI.ShortInteger SequenceIndex;

				public TI.Flags Type;
				public TI.Flags Flags;
			};
			#endregion

			public TI.Enum StateAttachedTo, CanUseOnMapType;
			public TI.TagReference Bitmap;
			public TI.Block<weapon_hud_overlay_block> Overlays;
		};
		#endregion

		public TI.TagReference ChildHud;
		
		public TI.Flags FlashCutoffFlags;
		public TI.ShortInteger TotalAmmoCutoff, LoadedAmmoCutoff, HeatCutoff, AgeCutoff;

		public TI.Enum Anchor;
		public TI.Block<weapon_hud_static_block> StaticElements;
		public TI.Block<weapon_hud_meter_block> MeterElements;
		public TI.Block<weapon_hud_number_block> NumberElements;

		public TI.Block<weapon_hud_crosshair_block> Crosshairs;
		public TI.Block<weapon_hud_overlays_block> OverlayElements;
		public TI.Flags PostprocessedUsedCrosshairStatesFlags;

		public TI.Block<global_hud_screen_effect_definition> ScreenEffect;

		public TI.ShortInteger SequenceIndex, WidthOffset;
		public TI.Point2D OffsetFromReferenceCorner;
		public TI.Color OverrideIconColor;
		public TI.ByteInteger FrameRate;
		public TI.Flags Flags;
		public TI.ShortInteger TextIndex;
	};
	#endregion

	#region unit_hud_interface
	[TI.TagGroup((int)TagGroups.Enumerated.unhi, 1, 1388)]
	public partial class unit_hud_interface_group : TI.Definition
	{
		#region unit_hud_auxilary_overlay_block
		[TI.Definition(-1, 132)]
		public partial class unit_hud_auxilary_overlay_block : TI.Definition
		{
			public TI.Struct<global_hud_interface_element_struct> Element;

			public TI.ShortInteger SequenceIndex;
			public TI.Block<global_hud_multitexture_overlay_definition> MultitexOverlay;

			public TI.Enum Type;
			public TI.Flags Flags;
		};
		#endregion

		#region unit_hud_auxilary_panel_block
		[TI.Definition(-1, 324)]
		public partial class unit_hud_auxilary_panel_block : TI.Definition
		{
			public TI.Enum Type;

			public TI.Struct<global_hud_interface_element_struct> Background;
			public TI.ShortInteger BackgroundSequenceIndex;
			public TI.Block<global_hud_multitexture_overlay_definition> BackgroundMultitexOverlay;

			public TI.Struct<global_hud_meter_struct> Meter;
			public TI.Real MeterMinFractionCutoff;

			public TI.Flags Flags;
		};
		#endregion

		public TI.Enum Anchor;

		#region HudBackground
		public TI.Struct<global_hud_interface_element_struct> HudBackground;

		public TI.ShortInteger HudBackgroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> HudBackgroundMultitexOverlay;
		#endregion

		#region ShieldPanelBackground
		public TI.Struct<global_hud_interface_element_struct> ShieldPanelBackground;

		public TI.ShortInteger ShieldPanelBackgroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> ShieldPanelBackgroundMultitexOverlay;
		#endregion

		#region ShieldPanelMeter
		public TI.Struct<global_hud_meter_struct> ShieldPanelMeter;
		public TI.Color ShieldPanelMeterOverchargeMinColor, ShieldPanelMeterOverchargeMaxColor, ShieldPanelMeterOverchargeFlashColor, ShieldPanelMeterOverchargeEmptyColor;
		#endregion

		#region HealthPanelBackground
		public TI.Struct<global_hud_interface_element_struct> HealthPanelBackground;

		public TI.ShortInteger HealthPanelBackgroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> HealthPanelBackgroundMultitexOverlay;
		#endregion

		#region HealthPanelMeter
		public TI.Struct<global_hud_meter_struct> HealthPanelMeter;

		public TI.Color HealthPanelMeterMediumHealthLeftColor;
		public TI.Real HealthPanelMeterMaxColorHealthFractionCutoff, HealthPanelMeterMinColorHealthFractionCutoff;
		#endregion

		#region MotionSensorBackground
		public TI.Struct<global_hud_interface_element_struct> MotionSensorBackground;

		public TI.ShortInteger MotionSensorBackgroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> MotionSensorBackgroundMultitexOverlay;
		#endregion

		#region MotionSensorForeground
		public TI.Struct<global_hud_interface_element_struct> MotionSensorForeground;

		public TI.ShortInteger MotionSensorForegroundSequenceIndex;
		public TI.Block<global_hud_multitexture_overlay_definition> MotionSensorForegroundMultitexOverlay;
		#endregion

		public TI.Point2D MotionSensorCenterAnchorOffset;
		public TI.Real MotionSensorCenterWidthScale, MotionSensorCenterHeightScale;
		public TI.Flags MotionSensorCenterScalingFlags;

		public TI.Enum AuxilaryOverlaysAnchor;
		public TI.Block<unit_hud_auxilary_overlay_block> AuxilaryOverlays;

		public TI.Block<global_hud_sound_block> Sounds;

		public TI.Block<unit_hud_auxilary_panel_block> Meters;
	};
	#endregion

	#region hud_globals
	[TI.TagGroup((int)TagGroups.Enumerated.hudg, 1, 1104)]
	public partial class hud_globals_group : TI.Definition
	{
		#region hud_button_icon_block
		[TI.Definition(-1, 16)]
		public partial class hud_button_icon_block : TI.Definition
		{
			public TI.ShortInteger SequenceIndex, WidthOffset;
			public TI.Point2D OffsetFromReferenceCorner;
			public TI.Color OverrideIconColor;
			public TI.ByteInteger FrameRate;
			public TI.Flags Flags;
			public TI.ShortInteger TextIndex;
		};
		#endregion

		#region hud_waypoint_arrow_block
		[TI.Definition(-1, 104)]
		public partial class hud_waypoint_arrow_block : TI.Definition
		{
			public TI.String Name;
			public TI.Color Color;
			public TI.Real Opacity, Translucency;
			public TI.ShortInteger OnScreenSequenceIndex, OffScreenSequenceIndex, OccludedSequenceIndex;
			public TI.Flags Flags;
		};
		#endregion

		public TI.Enum Anchor;
		public TI.Point2D AnchorOffset;
		public TI.Real WidthScale;
		public TI.Real HeightScale;
		public TI.Flags ScalingFlags;
		public TI.TagReference SinglePlayerFont;
		public TI.TagReference MultiPlayerFont;
		public TI.Real UpTime;
		public TI.Real FadeTime;
		public TI.RealColor IconColor;
		public TI.RealColor TextColor;
		public TI.Real TextSpacing;
		public TI.TagReference ItemMessageText;
		public TI.TagReference IconBitmap;
		public TI.TagReference AlternateIconText;
		public TI.Block<hud_button_icon_block> ButtonIcons;

		public TI.Struct<global_hud_color_struct> HelpText;

		public TI.TagReference HudMessages;
		public TI.Struct<global_hud_color_struct> Objective;
		public TI.ShortInteger ObjectiveUptimeTicks;
		public TI.ShortInteger ObjectiveFadeTicks;

		public TI.Real WaypointOffsetTop;
		public TI.Real WaypointOffsetBottom;
		public TI.Real WaypointOffsetLeft;
		public TI.Real WaypointOffsetRight;
		public TI.TagReference WaypointArrowBitmap;
		public TI.Block<hud_waypoint_arrow_block> WaypointArrows;

		public TI.Real HudScaleInMultiplayer;

		public TI.TagReference DefaultWeaponHud;
		public TI.Real MotionSensorRange;
		public TI.Real MotionSensorVelocitySensitivity;
		public TI.Real MotionSensorScale;
		public TI.Rectangle2D DefaultChapterTitleBounds;

		public TI.Rectangle2D HudDamageIndicatorOffsets;
		public TI.TagReference IndicatorBitmap;
		public TI.ShortInteger HudDamageIndicatorSequenceIndex;
		public TI.ShortInteger HudDamageIndicatorMpSequenceIndex;
		public TI.Color HudDamageIndicatorColor;

		public TI.Struct<global_hud_color_struct> HudTimerWarning;

		public TI.Struct<global_hud_color_struct> HudTimerDone;

		public TI.TagReference CarnageReportBitmap;
		public TI.ShortInteger LoadingBeginText;
		public TI.ShortInteger LoadingEndText;
		public TI.ShortInteger CheckpointBeginText;
		public TI.ShortInteger CheckpointEndText;
		public TI.TagReference CheckpointSound;
	};
	#endregion

	#region hud_message_text
	[TI.TagGroup((int)TagGroups.Enumerated.hmt_, 1, 128)]
	public partial class hud_message_text_group : TI.Definition
	{
		#region hud_message_elements_block
		[TI.Definition(-1, 2)]
		public partial class hud_message_elements_block : TI.Definition
		{
			public TI.ByteInteger Type, Data;
		};
		#endregion

		#region hud_messages_block
		[TI.Definition(-1, 64)]
		public partial class hud_messages_block : TI.Definition
		{
			public TI.String Name;
			public TI.ShortInteger StartIndexIntoTextBlob, StartIndexOfMessageBlock;
			public TI.ByteInteger PanelCount;
		};
		#endregion

		public TI.Data TextData;
		public TI.Block<hud_message_elements_block> MessageElements;
		public TI.Block<hud_messages_block> Messages;
	};
	#endregion

	#region hud_number
	[TI.TagGroup((int)TagGroups.Enumerated.hud_, 1, 100)]
	public partial class hud_number_group : TI.Definition
	{
		public TI.TagReference DigitsBitmap;
		public TI.ByteInteger BitmapDigitWidth, ScreenDigitWidth, XOffset, YOffset, DecimalPointWidth, ColonWidth;
	};
	#endregion

	#region meter
	[TI.TagGroup((int)TagGroups.Enumerated.metr, 1, 172)]
	public partial class meter_group : TI.Definition
	{
		private TI.Flags Flags;
		public TI.TagReference StencilBitmap, SourceBitmap;
		public TI.ShortInteger StencilSequenceIndex, SourceSequenceIndex;
		public TI.Enum InterpolateColors, AnchorColors;
		public TI.RealColor EmptyColor, FullColor;
		public TI.Real UnmaskDist, MaskDist;
		public TI.Data EncodedStencil;
	};
	#endregion
}