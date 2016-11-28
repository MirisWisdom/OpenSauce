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
	public partial class global_hud_interface_element_struct
	{
		public global_hud_interface_element_struct() : base(14)
		{
			Add(AnchorOffset = new TI.Point2D());
			Add(WidthScale = new TI.Real());
			Add(HeightScale = new TI.Real());
			Add(ScalingFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(DefaultColor = new TI.Color());
			Add(FlashingColor = new TI.Color());
			Add(FlashPeriod = new TI.Real());
			Add(FlashDelay = new TI.Real());
			Add(NumberOfFlashes = new TI.ShortInteger());
			Add(FlashFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(FlashLength = new TI.Real());
			Add(DisabledColor = new TI.Color());

			// i think the 4 byte padding that follows this should be here...
		}
	};
	#endregion

	#region global_hud_element
	public partial class global_hud_element_struct
	{
		public global_hud_element_struct() : base(13)
		{
			Add(AnchorOffset = new TI.Point2D());
			Add(WidthScale = new TI.Real());
			Add(HeightScale = new TI.Real());
			Add(ScalingFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(DefaultColor = new TI.Color());
			Add(FlashingColor = new TI.Color());
			Add(FlashPeriod = new TI.Real());
			Add(FlashDelay = new TI.Real());
			Add(NumberOfFlashes = new TI.ShortInteger());
			Add(FlashFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(FlashLength = new TI.Real());
			Add(DisabledColor = new TI.Color());

			// i think the 4 byte padding that follows this should be here...
		}
	};
	#endregion

	#region global_hud_meter
	public partial class global_hud_meter_struct
	{
		public global_hud_meter_struct() : base(20)
		{
			Add(AnchorOffset = new TI.Point2D());
			Add(WidthScale = new TI.Real());
			Add(HeightScale = new TI.Real());
			Add(ScalingFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(ColorAtMeterMin = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(ColorAtMeterMax = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(FlashColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(EmptyColor = new TI.Color());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
			Add(MinMeterValue = new TI.ByteInteger());
			Add(SequenceIndex = new TI.ShortInteger());
			Add(AlphaMul = new TI.ByteInteger());
			Add(AlphaBias = new TI.ByteInteger());
			Add(ValueScale = new TI.ShortInteger());
			Add(Opacity = new TI.Real());
			Add(Translucency = new TI.Real());
			Add(DisabledColor = new TI.Color());
			Add(new TI.Pad(16));
		}
	};
	#endregion

	#region global_hud_color
	public partial class global_hud_color_struct
	{
		public global_hud_color_struct() : base(8)
		{
			Add(DefaultColor = new TI.Color());
			Add(FlashingColor = new TI.Color());
			Add(FlashPeriod = new TI.Real());
			Add(FlashDelay = new TI.Real());
			Add(NumberOfFlashes = new TI.ShortInteger());
			Add(FlashFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(FlashLength = new TI.Real());
			Add(DisableColor = new TI.Color());
		}
	};
	#endregion


	#region global_hud_multitexture_overlay_definition
	public partial class global_hud_multitexture_overlay_definition
	{
		#region global_hud_multitexture_overlay_effector_definition
		public partial class global_hud_multitexture_overlay_effector_definition
		{
			public global_hud_multitexture_overlay_effector_definition() : base(15)
			{
				Add(new TI.Pad(64));
				Add(DestType = new TI.Enum());
				Add(Dest = new TI.Enum());
				Add(Source = new TI.Enum());
				Add(new TI.Pad(2));

				Add(InBounds = new TI.RealBounds());
				Add(OutBounds = new TI.RealBounds());
				Add(new TI.Pad(64));

				Add(TintColorLowerBound = new TI.RealColor());
				Add(TintColorUpperBound = new TI.RealColor());

				Add(PeriodicFunction = new TI.Enum());
				Add(new TI.Pad(2));
				Add(FunctionPeriod = new TI.Real());
				Add(FunctionPhase = new TI.Real());
				Add(new TI.Pad(32));
			}
		};
		#endregion

		public global_hud_multitexture_overlay_definition() : base(25)
		{
			Add(new TI.Pad(2));
			Add(Type = new TI.ShortInteger());
			Add(FrameBufferBlendFunc = new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(AnchorsPrimaryAnchor = new TI.Enum());
			Add(AnchorsSecondaryAnchor = new TI.Enum());
			Add(AnchorsTertiaryAnchor = new TI.Enum());
			Add(BlendFunc0To1 = new TI.Enum());
			Add(BlendFunc1To2 = new TI.Enum());
			Add(new TI.Pad(2));
			Add(MapScalesPrimaryAnchor = new TI.RealPoint2D());
			Add(MapScalesSecondaryAnchor = new TI.RealPoint2D());
			Add(MapScalesTertiaryAnchor = new TI.RealPoint2D());
			Add(MapOffsetsPrimaryAnchor = new TI.RealPoint2D());
			Add(MapOffsetsSecondaryAnchor = new TI.RealPoint2D());
			Add(MapOffsetsTertiaryAnchor = new TI.RealPoint2D());
			Add(MapPrimaryAnchor = new TI.TagReference(this, TagGroups.bitm));
			Add(MapSecondaryAnchor = new TI.TagReference(this, TagGroups.bitm));
			Add(MapTertiaryAnchor = new TI.TagReference(this, TagGroups.bitm));
			Add(PrimaryWrapMode = new TI.Enum());
			Add(SecondaryWrapMode = new TI.Enum());
			Add(TertiaryWrapMode = new TI.Enum());
			Add(new TI.Pad(2 + 184));
			Add(Effectors = new TI.Block<global_hud_multitexture_overlay_effector_definition>(this, 30));
			Add(new TI.Pad(128));
		}
	};
	#endregion

	#region global_hud_sound_block
	public partial class global_hud_sound_block
	{
		public global_hud_sound_block() : base(4)
		{
			Add(Sound = new TI.TagReference()); // snd!,lsnd
			Add(LatchedTo = new TI.Flags());
			Add(Scale = new TI.Real());
			Add(new TI.Pad(32));
		}
	};
	#endregion

	#region global_hud_screen_effect_definition
	public partial class global_hud_screen_effect_definition
	{
		public global_hud_screen_effect_definition() : base(20)
		{
			Add(new TI.Pad(4));
			Add(MaskFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 16));
			Add(MaskFullscreen = new TI.TagReference(this, TagGroups.bitm));
			Add(MaskSplitscreen = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(8));

			Add(ConvolutionFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(FovInBounds = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
			Add(RadiusOutBounds = new TI.RealBounds());
			Add(new TI.Pad(24));

			Add(NightVisionFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(NightVisionScriptSource = new TI.ShortInteger());
			Add(NightVisionIntensity = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(24));

			Add(DesaturationFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(DesaturationScriptSource = new TI.ShortInteger());
			Add(DesaturationIntensity = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(DesaturationTint = new TI.RealColor());
			Add(new TI.Pad(24));
		}
	};
	#endregion

	#region grenade_hud_interface
	public partial class grenade_hud_interface_group
	{
		#region grenade_hud_overlay_block
		public partial class grenade_hud_overlay_block
		{
			public grenade_hud_overlay_block() : base(7)
			{
				Add(Element = new TI.Struct<global_hud_element_struct>(this));

				Add(new TI.Pad(4));
				Add(FrameRate = new TI.Real());
				Add(SequenceIndex = new TI.ShortInteger());
				Add(Type = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(16 + 40));
			}
		};
		#endregion

		public grenade_hud_interface_group() : base(34)
		{
			Add(Anchor = new TI.Enum());
			Add(new TI.Pad(2 + 32));

			#region HudBackground
			Add(HudBackground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(HudBackgroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(HudBackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			#endregion

			#region TotalGrenadesBackground
			Add(TotalGrenadesBackground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(TotalGrenadesBackgroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(TotalGrenadesBackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			#endregion

			#region TotalGrenadesNumbers
			Add(TotalGrenadesNumbers = new TI.Struct<global_hud_element_struct>(this));

			Add(new TI.Pad(4));
			Add(TotalGrenadesNumbersMaxNumberOfDigits = new TI.ByteInteger());
			Add(TotalGrenadesNumbersFlags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
			Add(TotalGrenadesNumbersNumberOfFractionalDigits = new TI.ByteInteger());
			Add(new TI.Pad(1 + 12));
			Add(TotalGrenadesNumbersFlashCutoff = new TI.ShortInteger());
			Add(new TI.Pad(2));
			#endregion

			Add(OverlayBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(Overlays = new TI.Block<grenade_hud_overlay_block>(this, 16));
			Add(WarningSounds = new TI.Block<global_hud_sound_block>(this, 12));
			Add(new TI.Pad(68)); // looks like an unused global_hud_element_struct

			Add(SequenceIndex = new TI.ShortInteger());
			Add(WidthOffset = new TI.ShortInteger());
			Add(OffsetFromReferenceCorner = new TI.Point2D());
			Add(OverrideIconColor = new TI.Color());
			Add(FrameRate = new TI.ByteInteger());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
			Add(TextIndex = new TI.ShortInteger());
			Add(new TI.Pad(48));
		}
	};
	#endregion

	#region weapon_hud_interface
	public partial class weapon_hud_interface_group
	{
		#region weapon_hud_static_block
		public partial class weapon_hud_static_block
		{
			public weapon_hud_static_block() : base(10)
			{
				Add(StateAttachedTo = new TI.Enum());
				Add(new TI.Pad(2));
				Add(CanUseOnMapType = new TI.Enum());
				Add(new TI.Pad(2 + 28));

				Add(Element = new TI.Struct<global_hud_interface_element_struct>(this));

				Add(new TI.Pad(4));
				Add(SequenceIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(MultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
				Add(new TI.Pad(4 + 40));
			}
		};
		#endregion

		#region weapon_hud_meter_block
		public partial class weapon_hud_meter_block
		{
			public weapon_hud_meter_block() : base(6)
			{
				Add(StateAttachedTo = new TI.Enum());
				Add(new TI.Pad(2));
				Add(CanUseOnMapType = new TI.Enum());
				Add(new TI.Pad(2 + 28));

				Add(Meter = new TI.Struct<global_hud_meter_struct>(this));

				Add(new TI.Pad(40));
			}
		};
		#endregion

		#region weapon_hud_number_block
		public partial class weapon_hud_number_block
		{
			public weapon_hud_number_block() : base(12)
			{
				Add(StateAttachedTo = new TI.Enum());
				Add(new TI.Pad(2));
				Add(CanUseOnMapType = new TI.Enum());
				Add(new TI.Pad(2 + 28));

				Add(Element = new TI.Struct<global_hud_element_struct>(this));

				Add(new TI.Pad(4));
				Add(MaxNumberOfDigits = new TI.ByteInteger());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
				Add(NumberOfFractionalDigits = new TI.ByteInteger());
				Add(new TI.Pad(1 + 12));
				Add(WeaponSpecificFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(2 + 36));
			}
		};
		#endregion

		#region weapon_hud_crosshair_block
		public partial class weapon_hud_crosshair_block
		{
			#region weapon_hud_crosshair_item_block
			public partial class weapon_hud_crosshair_item_block
			{
				public weapon_hud_crosshair_item_block() : base(6)
				{
					Add(Element = new TI.Struct<global_hud_element_struct>(this));

					Add(new TI.Pad(4));
					Add(FrameRate = new TI.ShortInteger());
					Add(SequenceIndex = new TI.ShortInteger());
					Add(Flags = new TI.Flags());
					Add(new TI.Pad(32));
				}
			};
			#endregion

			public weapon_hud_crosshair_block() : base(7)
			{
				Add(CrosshairType = new TI.Enum());
				Add(new TI.Pad(2));
				Add(CanUseOnMapType = new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
				Add(Overlays = new TI.Block<weapon_hud_crosshair_item_block>(this, 16));
				Add(new TI.Pad(40));
			}
		};
		#endregion

		#region weapon_hud_overlays_block
		public partial class weapon_hud_overlays_block
		{
			#region weapon_hud_overlay_block
			public partial class weapon_hud_overlay_block
			{
				public weapon_hud_overlay_block() : base(8)
				{
					Add(Element = new TI.Struct<global_hud_element_struct>(this));

					Add(new TI.Pad(4));
					Add(FrameRate = new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(SequenceIndex = new TI.ShortInteger());
					Add(Type = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
					Add(Flags = new TI.Flags());
					Add(new TI.Pad(16 + 40));
				}
			};
			#endregion

			public weapon_hud_overlays_block() : base(7)
			{
				Add(StateAttachedTo = new TI.Enum());
				Add(new TI.Pad(2));
				Add(CanUseOnMapType = new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
				Add(Overlays = new TI.Block<weapon_hud_overlay_block>(this, 16));
				Add(new TI.Pad(40));
			}
		};
		#endregion

		public weapon_hud_interface_group() : base(27)
		{
			Add(ChildHud = new TI.TagReference(this, TagGroups.wphi));
			Add(FlashCutoffFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(TotalAmmoCutoff = new TI.ShortInteger());
			Add(LoadedAmmoCutoff = new TI.ShortInteger());
			Add(HeatCutoff = new TI.ShortInteger());
			Add(AgeCutoff = new TI.ShortInteger());
			Add(new TI.Pad(32));
			Add(Anchor = new TI.Enum());
			Add(new TI.Pad(2 + 32));

			Add(StaticElements = new TI.Block<weapon_hud_static_block>(this, 16));
			Add(MeterElements = new TI.Block<weapon_hud_meter_block>(this, 16));
			Add(NumberElements = new TI.Block<weapon_hud_number_block>(this, 16));
			Add(Crosshairs = new TI.Block<weapon_hud_crosshair_block>(this, 19));
			Add(OverlayElements = new TI.Block<weapon_hud_overlays_block>(this, 16));
			Add(PostprocessedUsedCrosshairStatesFlags = new TI.Flags());
			Add(new TI.Pad(12)); // g_null_block, so we just pad it
			Add(ScreenEffect = new TI.Block<global_hud_screen_effect_definition>(this, 1));
			Add(new TI.Pad(132));

			Add(SequenceIndex = new TI.ShortInteger());
			Add(WidthOffset = new TI.ShortInteger());
			Add(OffsetFromReferenceCorner = new TI.Point2D());
			Add(OverrideIconColor = new TI.Color());
			Add(FrameRate = new TI.ByteInteger());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
			Add(TextIndex = new TI.ShortInteger());
			Add(new TI.Pad(48));
		}
	};
	#endregion

	#region unit_hud_interface
	public partial class unit_hud_interface_group
	{
		#region unit_hud_auxilary_overlay_block
		public partial class unit_hud_auxilary_overlay_block
		{
			public unit_hud_auxilary_overlay_block() : base(9)
			{
				Add(Element = new TI.Struct<global_hud_interface_element_struct>(this));

				Add(new TI.Pad(4));
				Add(SequenceIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(MultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
				Add(new TI.Pad(4));
				Add(Type = new TI.Enum());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(24));
			}
		};
		#endregion

		#region unit_hud_auxilary_panel_block
		public partial class unit_hud_auxilary_panel_block
		{
			public unit_hud_auxilary_panel_block() : base(12)
			{
				Add(Type = new TI.Enum());
				Add(new TI.Pad(2 + 16));

				Add(Background = new TI.Struct<global_hud_interface_element_struct>(this));
				Add(new TI.Pad(4));
				Add(BackgroundSequenceIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(BackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
				Add(new TI.Pad(4));

				Add(Meter = new TI.Struct<global_hud_meter_struct>(this));
				Add(MeterMinFractionCutoff = new TI.Real());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(24 + 64));
			}
		};
		#endregion

		public unit_hud_interface_group() : base(55)
		{
			Add(Anchor = new TI.Enum());
			Add(new TI.Pad(2 + 32));

			#region HudBackgroundAnchor
			Add(HudBackground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(HudBackgroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(HudBackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			#endregion

			#region ShieldPanelBackground
			Add(ShieldPanelBackground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(ShieldPanelBackgroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(ShieldPanelBackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			#endregion

			#region ShieldPanelMeter
			Add(ShieldPanelMeter = new TI.Struct<global_hud_meter_struct>(this));

			Add(ShieldPanelMeterOverchargeMinColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(ShieldPanelMeterOverchargeMaxColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(ShieldPanelMeterOverchargeFlashColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(ShieldPanelMeterOverchargeEmptyColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(new TI.Pad(16));
			#endregion

			#region HealthPanelBackground
			Add(HealthPanelBackground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(HealthPanelBackgroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(HealthPanelBackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			#endregion

			#region HealthPanelMeter
			Add(HealthPanelMeter = new TI.Struct<global_hud_meter_struct>(this));

			Add(HealthPanelMeterMediumHealthLeftColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(HealthPanelMeterMaxColorHealthFractionCutoff = new TI.Real());
			Add(HealthPanelMeterMinColorHealthFractionCutoff = new TI.Real());
			Add(new TI.Pad(20));
			#endregion

			#region MotionSensorBackground
			Add(MotionSensorBackground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(MotionSensorBackgroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(MotionSensorBackgroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			#endregion

			#region MotionSensorForeground
			Add(MotionSensorForeground = new TI.Struct<global_hud_interface_element_struct>(this));

			Add(new TI.Pad(4));
			Add(MotionSensorForegroundSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(MotionSensorForegroundMultitexOverlay = new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4 + 32));
			#endregion

			Add(MotionSensorCenterAnchorOffset = new TI.Point2D());
			Add(MotionSensorCenterWidthScale = new TI.Real());
			Add(MotionSensorCenterHeightScale = new TI.Real());
			Add(MotionSensorCenterScalingFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));

			Add(AuxilaryOverlaysAnchor = new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(AuxilaryOverlays = new TI.Block<unit_hud_auxilary_overlay_block>(this, 16));
			Add(new TI.Pad(16));

			Add(Sounds = new TI.Block<global_hud_sound_block>(this, 12));

			Add(Meters = new TI.Block<unit_hud_auxilary_panel_block>(this, 16));

			Add(new TI.Pad(356 + 48));
		}
	};
	#endregion

	#region hud_globals
	public partial class hud_globals_group
	{
		#region hud_button_icon_block
		public partial class hud_button_icon_block
		{
			public hud_button_icon_block() : base(7)
			{
				Add(SequenceIndex = new TI.ShortInteger());
				Add(WidthOffset = new TI.ShortInteger());
				Add(OffsetFromReferenceCorner = new TI.Point2D());
				Add(OverrideIconColor = new TI.Color());
				Add(FrameRate = new TI.ByteInteger());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
				Add(TextIndex = new TI.ShortInteger());
			}
		};
		#endregion

		#region hud_waypoint_arrow_block
		public partial class hud_waypoint_arrow_block
		{
			public hud_waypoint_arrow_block() : base(11)
			{
				Add(Name = new TI.String());
				Add(new TI.Pad(8));
				Add(Color = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
				Add(Opacity = new TI.Real());
				Add(Translucency = new TI.Real());
				Add(OnScreenSequenceIndex = new TI.ShortInteger());
				Add(OffScreenSequenceIndex = new TI.ShortInteger());
				Add(OccludedSequenceIndex = new TI.ShortInteger());
				Add(new TI.Pad(2 + 16));
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(24));
			}
		};
		#endregion

		public hud_globals_group() : base(58)
		{
			Add(Anchor = new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(AnchorOffset = new TI.Point2D());
			Add(WidthScale = new TI.Real());
			Add(HeightScale = new TI.Real());
			Add(ScalingFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(SinglePlayerFont = new TI.TagReference(this, TagGroups.font));
			Add(MultiPlayerFont = new TI.TagReference(this, TagGroups.font));
			Add(UpTime = new TI.Real());
			Add(FadeTime = new TI.Real());
			Add(IconColor = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(TextColor = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(TextSpacing = new TI.Real());
			Add(ItemMessageText = new TI.TagReference(this, TagGroups.ustr));
			Add(IconBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(AlternateIconText = new TI.TagReference(this, TagGroups.ustr));
			Add(ButtonIcons = new TI.Block<hud_button_icon_block>(this, 18));

			Add(HelpText = new TI.Struct<global_hud_color_struct>(this));
			Add(new TI.Pad(4));

			Add(HudMessages = new TI.TagReference(this, TagGroups.hmt_));
			Add(Objective = new TI.Struct<global_hud_color_struct>(this));
			Add(ObjectiveUptimeTicks = new TI.ShortInteger());
			Add(ObjectiveFadeTicks = new TI.ShortInteger());

			Add(WaypointOffsetTop = new TI.Real());
			Add(WaypointOffsetBottom = new TI.Real());
			Add(WaypointOffsetLeft = new TI.Real());
			Add(WaypointOffsetRight = new TI.Real());
			Add(new TI.Pad(32));
			Add(WaypointArrowBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(WaypointArrows = new TI.Block<hud_waypoint_arrow_block>(this, 16));

			Add(new TI.Pad(80));

			Add(HudScaleInMultiplayer = new TI.Real());
			Add(new TI.Pad(256));

			Add(DefaultWeaponHud = new TI.TagReference(this, TagGroups.wphi));
			Add(MotionSensorRange = new TI.Real());
			Add(MotionSensorVelocitySensitivity = new TI.Real());
			Add(MotionSensorScale = new TI.Real());
			Add(DefaultChapterTitleBounds = new TI.Rectangle2D());
			Add(new TI.Pad(44));

			Add(HudDamageIndicatorOffsets = new TI.Rectangle2D());
			Add(new TI.Pad(32));
			Add(IndicatorBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(HudDamageIndicatorSequenceIndex = new TI.ShortInteger());
			Add(HudDamageIndicatorMpSequenceIndex = new TI.ShortInteger());
			Add(HudDamageIndicatorColor = new TI.Color());
			Add(new TI.Pad(16));

			Add(HudTimerWarning = new TI.Struct<global_hud_color_struct>(this));
			Add(new TI.Pad(4));

			Add(HudTimerDone = new TI.Struct<global_hud_color_struct>(this));
			Add(new TI.Pad(4 + 40));

			Add(CarnageReportBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(LoadingBeginText = new TI.ShortInteger());
			Add(LoadingEndText = new TI.ShortInteger());
			Add(CheckpointBeginText = new TI.ShortInteger());
			Add(CheckpointEndText = new TI.ShortInteger());
			Add(CheckpointSound = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(96));
		}
	};
	#endregion

	#region hud_message_text
	public partial class hud_message_text_group
	{
		#region hud_message_elements_block
		public partial class hud_message_elements_block
		{
			public hud_message_elements_block() : base(2)
			{
				Add(Type = new TI.ByteInteger());
				Add(Data = new TI.ByteInteger());
			}
		};
		#endregion

		#region hud_messages_block
		public partial class hud_messages_block
		{
			public hud_messages_block() : base(5)
			{
				Add(Name = new TI.String());
				Add(StartIndexIntoTextBlob = new TI.ShortInteger());
				Add(StartIndexOfMessageBlock = new TI.ShortInteger());
				Add(PanelCount = new TI.ByteInteger());
				Add(new TI.Pad(3 + 24));
			}
		};
		#endregion

		public hud_message_text_group() : base(4)
		{
			Add(TextData = new TI.Data(this));
			Add(MessageElements = new TI.Block<hud_message_elements_block>(this, 8192));
			Add(Messages = new TI.Block<hud_messages_block>(this, 1024));
			Add(new TI.Pad(84));
		}
	};
	#endregion

	#region hud_number
	public partial class hud_number_group
	{
		public hud_number_group() : base(8)
		{
			Add(DigitsBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(BitmapDigitWidth = new TI.ByteInteger());
			Add(ScreenDigitWidth = new TI.ByteInteger());
			Add(XOffset = new TI.ByteInteger());
			Add(YOffset = new TI.ByteInteger());
			Add(DecimalPointWidth = new TI.ByteInteger());
			Add(ColonWidth = new TI.ByteInteger());
			Add(new TI.Pad(2 + 76));
		}
	};
	#endregion

	#region meter
	public partial class meter_group
	{
		public meter_group() : base(16)
		{
			Add(Flags = new TI.Flags());
			Add(StencilBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(SourceBitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(StencilSequenceIndex = new TI.ShortInteger());
			Add(SourceSequenceIndex = new TI.ShortInteger());
			Add(new TI.Pad(16 + 4));
			Add(InterpolateColors = new TI.Enum());
			Add(AnchorColors = new TI.Enum());
			Add(new TI.Pad(8));
			Add(EmptyColor = new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(FullColor = new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(new TI.Pad(20));
			Add(UnmaskDist = new TI.Real());
			Add(MaskDist = new TI.Real());
			Add(new TI.Pad(20));
			Add(EncodedStencil = new TI.Data(this));
		}
	};
	#endregion
}