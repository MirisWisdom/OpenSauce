/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region camera_track
	public partial class camera_track_group
	{
		#region camera_track_control_point_block
		public partial class camera_track_control_point_block
		{
			public camera_track_control_point_block() : base(3)
			{
				Add(Position = new TI.RealVector3D());
				Add(Orientation = new TI.RealQuaternion());
				Add(new TI.Pad(32));
			}
		};
		#endregion

		public camera_track_group() : base(3)
		{
			Add(Flags = new TI.Flags());
			Add(ControlPoints = new TI.Block<camera_track_control_point_block>(this, 16));
			Add(new TI.Pad(32));
		}
	};
	#endregion

	#region color_table
	public partial class color_table_group
	{
		#region color_block
		public partial class color_block
		{
			public color_block() : base(2)
			{
				Add(Name = new TI.String());
				Add(Color = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			}
		};
		#endregion

		public color_table_group() : base(1)
		{
			Add(Colors = new TI.Block<color_block>(this, 512));
		}
	};
	#endregion

	#region dialogue
	public partial class dialogue_group
	{
		public dialogue_group() : base(180)
		{
			Add(new TI.Pad(2 + 2 + 12));

			Idle = new TI.TagReference[3];
			for (int x = 0; x < Idle.Length; x++)
				Add(Idle[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 3));

			Involuntary = new TI.TagReference[14];
			for (int x = 0; x < Involuntary.Length; x++)
				Add(Involuntary[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16));

			HurtingPeople = new TI.TagReference[4];
			for (int x = 0; x < HurtingPeople.Length; x++)
				Add(HurtingPeople[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 4));

			BeingHurt = new TI.TagReference[17];
			for (int x = 0; x < BeingHurt.Length; x++)
				Add(BeingHurt[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 3));

			KillingPeople = new TI.TagReference[28];
			for (int x = 0; x < KillingPeople.Length; x++)
				Add(KillingPeople[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 3));

			PlayerKillResponses = new TI.TagReference[13];
			for (int x = 0; x < PlayerKillResponses.Length; x++)
				Add(PlayerKillResponses[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 3));

			FriendsDying = new TI.TagReference[10];
			for (int x = 0; x < FriendsDying.Length; x++)
				Add(FriendsDying[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 2));

			Shouting = new TI.TagReference[13];
			for (int x = 0; x < Shouting.Length; x++)
				Add(Shouting[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 2));

			GroupComunication = new TI.TagReference[21];
			for (int x = 0; x < GroupComunication.Length; x++)
				Add(GroupComunication[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 4));

			Actions = new TI.TagReference[23];
			for (int x = 0; x < Actions.Length; x++)
				Add(Actions[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 6));

			Exclamations = new TI.TagReference[7];
			for (int x = 0; x < Exclamations.Length; x++)
				Add(Exclamations[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 4));

			PostCombatActions = new TI.TagReference[5];
			for (int x = 0; x < PostCombatActions.Length; x++)
				Add(PostCombatActions[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 4));

			PostCombatChatter = new TI.TagReference[8];
			for (int x = 0; x < PostCombatChatter.Length; x++)
				Add(PostCombatChatter[x] = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(16 * 4 + 752));
		}
	};
	#endregion

	#region font
	public partial class font_group
	{
		#region font_character_tables_block
		public partial class font_character_tables_block
		{
			public font_character_tables_block() : base(1)
			{
				Add(CharacterTable = new TI.Block<field_block<TI.BlockIndex>>(this, 256));
			}
		};
		#endregion

		#region character_block
		public partial class character_block
		{
			public character_block() : base(9)
			{
				Add(Character = new TI.ShortInteger());
				Add(CharacterWidth = new TI.ShortInteger());
				Add(BitmapWidth = new TI.ShortInteger());
				Add(BitmapHeight = new TI.ShortInteger());
				Add(BitmapOriginX = new TI.ShortInteger());
				Add(BitmapOriginY = new TI.ShortInteger());
				Add(HardwardCharacterIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(PixelsOffset = new TI.LongInteger());
			}
		};
		#endregion

		public font_group() : base(13)
		{
			Add(Flags = new TI.Flags());
			Add(AscendingHeight = new TI.ShortInteger());
			Add(DecendingHeight = new TI.ShortInteger());
			Add(LeadingHeight = new TI.ShortInteger());
			Add(LeadinWidth = new TI.ShortInteger());
			Add(new TI.Pad(36));
			Add(CharacterTables = new TI.Block<font_character_tables_block>(this, 256));
			Add(Bold = new TI.TagReference(this, TagGroups.font));
			Add(Italic = new TI.TagReference(this, TagGroups.font));
			Add(Condense = new TI.TagReference(this, TagGroups.font));
			Add(Underline = new TI.TagReference(this, TagGroups.font));
			Add(Characters = new TI.Block<character_block>(this, 20000));
			Add(Pixels = new TI.Data(this));
		}
	};
	#endregion

	#region physics
	public partial class physics_group
	{
		#region inertial_matrix_block
		public partial class inertial_matrix_block
		{
			public inertial_matrix_block() : base(3)
			{
				Add(Mat1 = new TI.RealVector3D());
				Add(Mat2 = new TI.RealVector3D());
				Add(Mat3 = new TI.RealVector3D());
			}
		};
		#endregion

		#region powered_mass_point_block
		public partial class powered_mass_point_block
		{
			public powered_mass_point_block() : base(9)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(AntigravStrength = new TI.Real());
				Add(AntigravOffset = new TI.Real());
				Add(AntigravHeight = new TI.Real());
				Add(AntigravDampFraction = new TI.Real());
				Add(AntigravNormalK1 = new TI.Real());
				Add(AntigravNormalK0 = new TI.Real());
				Add(new TI.Pad(68));
			}
		};
		#endregion

		#region mass_point_block
		public partial class mass_point_block
		{
			public mass_point_block() : base(17)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(PoweredMassPoint = new TI.BlockIndex(TI.FieldType.ShortBlockIndex));
				Add(ModelNode = new TI.ShortInteger());
				Add(RelativeMass = new TI.Real());
				Add(Mass = new TI.Real());
				Add(RelativeDensity = new TI.Real());
				Add(Density = new TI.Real());
				Add(Position = new TI.RealPoint3D());
				Add(Forward = new TI.RealVector3D());
				Add(Up = new TI.RealVector3D());
				Add(FrictionType = new TI.Enum());
				Add(new TI.Pad(2));
				Add(FrictionParallelScale = new TI.Real());
				Add(FrictionPerpendicularScale = new TI.Real());
				Add(Radius = new TI.Real());
				Add(new TI.Pad(20));
			}
		};
		#endregion

		public physics_group() : base(24)
		{
			Add(Radius = new TI.Real());
			Add(MomentScale = new TI.Real(TI.FieldType.RealFraction));
			Add(Mass = new TI.Real());
			Add(CenterOfMass = new TI.RealPoint3D());
			Add(Density = new TI.Real());
			Add(GravityScale = new TI.Real());
			Add(GroundFriction = new TI.Real());
			Add(GroundDepth = new TI.Real());
			Add(GroundDampFraction = new TI.Real(TI.FieldType.RealFraction));
			Add(GroundNormalK1 = new TI.Real());
			Add(GroundNormalK0 = new TI.Real());
			Add(new TI.Pad(4));
			Add(WaterFriction = new TI.Real());
			Add(WaterDepth = new TI.Real());
			Add(WaterDensity = new TI.Real());
			Add(new TI.Pad(4));
			Add(AirFriction = new TI.Real());
			Add(new TI.Pad(4));
			Add(MomentXX = new TI.Real());
			Add(MomentYY = new TI.Real());
			Add(MomentZZ = new TI.Real());

			Add(InertialMatrixAndInverse = new TI.Block<inertial_matrix_block>(this, 2));
			Add(PoweredMassPoints = new TI.Block<powered_mass_point_block>(this, 32));
			Add(MassPoints = new TI.Block<mass_point_block>(this, 32));
		}
	};
	#endregion

	#region point_physics
	public partial class point_physics_group
	{
		public point_physics_group() : base(8)
		{
			Add(Flags = new TI.Flags());
			Add(new TI.Pad(28));
			Add(Density = new TI.Real());
			Add(AirFriction = new TI.Real());
			Add(WaterFriction = new TI.Real());
			Add(SurfaceFriction = new TI.Real());
			Add(Elasticity = new TI.Real());
			Add(new TI.Pad(12));
		}
	};
	#endregion

	#region preferences_network_game
	public partial class preferences_network_game_group
	{
		public preferences_network_game_group() : base(9)
		{
			Add(Name = new TI.String());
			Add(PrimaryColor = new TI.RealColor());
			Add(SecondaryColor = new TI.RealColor());
			Add(Pattern = new TI.TagReference(this, TagGroups.bitm));
			Add(PatternBitmapIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(Decal = new TI.TagReference(this, TagGroups.bitm));
			Add(DecalBitmapIndex = new TI.ShortInteger());
			Add(new TI.Pad(2 + 800));
		}
	};
	#endregion

	#region sky
	public partial class sky_group
	{
		#region sky_shader_function_block
		public partial class sky_shader_function_block
		{
			public sky_shader_function_block() : base(2)
			{
				Add(new TI.Pad(4));
				Add(GlobalFunctionName = new TI.String());
			}
		};
		#endregion

		#region sky_animation_block
		public partial class sky_animation_block
		{
			public sky_animation_block() : base(4)
			{
				Add(AnimationIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(Period = new TI.Real());
				Add(new TI.Pad(28));
			}
		};
		#endregion

		#region sky_light_block
		public partial class sky_light_block
		{
			public sky_light_block() : base(10)
			{
				Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
				Add(LensFlareMarkerName = new TI.String());
				Add(new TI.Pad(28));
				Add(Flags = new TI.Flags());
				Add(Color = new TI.RealColor(TI.FieldType.RealRgbColor));
				Add(Power = new TI.Real());
				Add(TestDistance = new TI.Real());
				Add(new TI.Pad(4));
				Add(Direction = new TI.RealEulerAngles2D());
				Add(Diameter = new TI.Real(TI.FieldType.Angle));
			}
		};
		#endregion

		public sky_group() : base(22)
		{
			Add(Model = new TI.TagReference(this, TagGroups.mod2));
			Add(Animation = new TI.TagReference(this, TagGroups.antr));
			Add(new TI.Pad(24));
			Add(IndoorAmbientRadiosityColor = new TI.RealColor(TI.FieldType.RealRgbColor));
			Add(IndoorAmbientRadiosityPower = new TI.Real());
			Add(OutdoorAmbientRadiosityColor = new TI.RealColor(TI.FieldType.RealRgbColor));
			Add(OutdoorAmbientRadiosityPower = new TI.Real());
			Add(OutdoorFogColor = new TI.RealColor(TI.FieldType.RealRgbColor));
			Add(new TI.Pad(8));
			Add(OutdoorFogMaximumDensity = new TI.Real(TI.FieldType.RealFraction));
			Add(OutdoorFogStartDistance = new TI.Real());
			Add(OutdoorFogOpaqueDistance = new TI.Real());
			Add(IndoorFogColor = new TI.RealColor(TI.FieldType.RealRgbColor));
			Add(new TI.Pad(8));
			Add(IndoorFogMaximumDensity = new TI.Real(TI.FieldType.RealFraction));
			Add(IndoorFogStartDistance = new TI.Real());
			Add(IndoorFogOpaqueDistance = new TI.Real());
			Add(IndoorFogScreen = new TI.TagReference(this, TagGroups.fog_));
			Add(new TI.Pad(4));
			Add(ShaderFunctions = new TI.Block<sky_shader_function_block>(this, 8));
			Add(Animations = new TI.Block<sky_animation_block>(this, 8));
			Add(Lights = new TI.Block<sky_light_block>(this, 8));
		}
	};
	#endregion
}