/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	[TI.Definition]
	public sealed class field_block<FieldType> : TI.field_block<FieldType> where FieldType : TI.Field, new() { };

	#region camera_track
	[TI.TagGroup((int)TagGroups.Enumerated.trak, 2, 48)]
	public partial class camera_track_group : TI.Definition
	{
		#region camera_track_control_point_block
		[TI.Definition(-1, 60)]
		public partial class camera_track_control_point_block : TI.Definition
		{
			public TI.RealVector3D Position;
			public TI.RealQuaternion Orientation;
		};
		#endregion

		private TI.Flags Flags;
		public TI.Block<camera_track_control_point_block> ControlPoints;
	};
	#endregion

	#region color_table
	[TI.TagGroup((int)TagGroups.Enumerated.colo, 1, 12)]
	public partial class color_table_group : TI.Definition
	{
		#region color_block
		[TI.Definition(-1, 48)]
		public partial class color_block : TI.Definition
		{
			public TI.String Name;
			public TI.RealColor Color;
		};
		#endregion

		public TI.Block<color_block> Colors;
	};
	#endregion

	#region dialogue
	[TI.TagGroup((int)TagGroups.Enumerated.udlg, 1, 4112)]
	public partial class dialogue_group : TI.Definition
	{
		public TI.TagReference[] Idle;
		public TI.TagReference[] Involuntary;
		public TI.TagReference[] HurtingPeople;
		public TI.TagReference[] BeingHurt;
		public TI.TagReference[] KillingPeople;
		public TI.TagReference[] PlayerKillResponses;
		public TI.TagReference[] FriendsDying;
		public TI.TagReference[] Shouting;
		public TI.TagReference[] GroupComunication;
		public TI.TagReference[] Actions;
		public TI.TagReference[] Exclamations;
		public TI.TagReference[] PostCombatActions;
		public TI.TagReference[] PostCombatChatter;
	};
	#endregion

	#region font
	[TI.TagGroup((int)TagGroups.Enumerated.font, 1, 156)]
	public partial class font_group : TI.Definition
	{
		#region font_character_tables_block
		[TI.Definition(-1, 12)]
		public partial class font_character_tables_block : TI.Definition
		{
			// font_character_table_block, field_block<TI.BlockIndex>

			public TI.Block<field_block<TI.BlockIndex>> CharacterTable;
		};
		#endregion

		#region character_block
		[TI.Definition(-1, 20)]
		public partial class character_block : TI.Definition
		{
			public TI.ShortInteger Character, CharacterWidth, BitmapWidth, BitmapHeight, BitmapOriginX, BitmapOriginY, HardwardCharacterIndex;
			public TI.LongInteger PixelsOffset;
		};
		#endregion

		public TI.Flags Flags; // suppose to be a long for some reason...
		public TI.ShortInteger AscendingHeight, DecendingHeight;
		public TI.ShortInteger LeadingHeight, LeadinWidth;
		public TI.Block<font_character_tables_block> CharacterTables;
		public TI.TagReference Bold, Italic, Condense, Underline;
		public TI.Block<character_block> Characters;
		public TI.Data Pixels;
	};
	#endregion

	#region physics
	[TI.TagGroup((int)TagGroups.Enumerated.phys, 4, 128)]
	public partial class physics_group : TI.Definition
	{
		#region inertial_matrix_block
		[TI.Definition(-1, 36)]
		public partial class inertial_matrix_block : TI.Definition
		{
			public TI.RealVector3D Mat1, Mat2, Mat3;
		};
		#endregion

		#region powered_mass_point_block
		[TI.Definition(-1, 128)]
		public partial class powered_mass_point_block : TI.Definition
		{
			public TI.String Name;
			public TI.Flags Flags;
			public TI.Real AntigravStrength, AntigravOffset, AntigravHeight, AntigravDampFraction, AntigravNormalK1, AntigravNormalK0;
		};
		#endregion

		#region mass_point_block
		[TI.Definition(-1, 128)]
		public partial class mass_point_block : TI.Definition
		{
			public TI.String Name;
			public TI.BlockIndex PoweredMassPoint;
			public TI.ShortInteger ModelNode;
			public TI.Flags Flags;
			public TI.Real RelativeMass, Mass, RelativeDensity, Density;
			public TI.RealPoint3D Position;
			public TI.RealVector3D Forward;
			public TI.RealVector3D Up;
			public TI.Enum FrictionType;
			public TI.Real FrictionParallelScale, FrictionPerpendicularScale;
			public TI.Real Radius;
		};
		#endregion

		public TI.Real Radius;
		public TI.Real MomentScale;
		public TI.Real Mass;
		public TI.RealPoint3D CenterOfMass;
		public TI.Real Density;
		public TI.Real GravityScale;
		public TI.Real GroundFriction;
		public TI.Real GroundDepth;
		public TI.Real GroundDampFraction;
		public TI.Real GroundNormalK1, GroundNormalK0;
		public TI.Real WaterFriction;
		public TI.Real WaterDepth;
		public TI.Real WaterDensity;
		public TI.Real AirFriction;
		public TI.Real MomentXX, MomentYY, MomentZZ;

		public TI.Block<inertial_matrix_block> InertialMatrixAndInverse;
		public TI.Block<powered_mass_point_block> PoweredMassPoints;
		public TI.Block<mass_point_block> MassPoints;
	};
	#endregion

	#region point_physics
	[TI.TagGroup((int)TagGroups.Enumerated.pphy, 1, 64)]
	public partial class point_physics_group : TI.Definition
	{
		public TI.Flags Flags;
		public TI.Real Density, AirFriction, WaterFriction, SurfaceFriction, Elasticity;
	};
	#endregion

	#region preferences_network_game
	[TI.TagGroup((int)TagGroups.Enumerated.ngpr, 2, 896)]
	public partial class preferences_network_game_group : TI.Definition
	{
		public TI.String Name;
		public TI.RealColor PrimaryColor, SecondaryColor;
		public TI.TagReference Pattern;
		public TI.ShortInteger PatternBitmapIndex;
		public TI.TagReference Decal;
		public TI.ShortInteger DecalBitmapIndex;
	};
	#endregion

	#region sky
	[TI.TagGroup((int)TagGroups.Enumerated.sky_, 1, 208)]
	public partial class sky_group : TI.Definition
	{
		#region sky_shader_function_block
		[TI.Definition(-1, 36)]
		public partial class sky_shader_function_block : TI.Definition
		{
			public TI.String GlobalFunctionName;
		};
		#endregion

		#region sky_animation_block
		[TI.Definition(-1, 36)]
		public partial class sky_animation_block : TI.Definition
		{
			public TI.ShortInteger AnimationIndex;
			public TI.Real Period;
		};
		#endregion

		#region sky_light_block
		[TI.Definition(-1, 116)]
		public partial class sky_light_block : TI.Definition
		{
			public TI.TagReference LensFlare;
			public TI.String LensFlareMarkerName;
			public TI.Flags Flags;
			public TI.RealColor Color;
			public TI.Real Power;
			public TI.Real TestDistance;
			public TI.RealEulerAngles2D Direction;
			public TI.Real Diameter;
		};
		#endregion

		public TI.TagReference Model, Animation;
		public TI.RealColor IndoorAmbientRadiosityColor;
		public TI.Real IndoorAmbientRadiosityPower;
		public TI.RealColor OutdoorAmbientRadiosityColor;
		public TI.Real OutdoorAmbientRadiosityPower;

		public TI.RealColor OutdoorFogColor;
		public TI.Real OutdoorFogMaximumDensity;
		public TI.Real OutdoorFogStartDistance;
		public TI.Real OutdoorFogOpaqueDistance;

		public TI.RealColor IndoorFogColor;
		public TI.Real IndoorFogMaximumDensity;
		public TI.Real IndoorFogStartDistance;
		public TI.Real IndoorFogOpaqueDistance;

		public TI.TagReference IndoorFogScreen;

		public TI.Block<sky_shader_function_block> ShaderFunctions;
		public TI.Block<sky_animation_block> Animations;
		public TI.Block<sky_light_block> Lights;
	};
	#endregion

	#region spheroid
	[TI.TagGroup((int)TagGroups.Enumerated.boom, 1, 4)]
	public partial class spheroid_group : TI.Definition
	{
		public spheroid_group() : base(1)
		{
			Add(new TI.Pad(4));
		}
	};
	#endregion
}