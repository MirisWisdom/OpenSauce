/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Scenario
	{
		struct object_names_block
		{
			tag_string Name;
			_enum Type;
			int16 Object; // block_index
		};

		struct object_palette_block
		{
			TagReference<'obje'> Definition;
			PAD64; PAD64; PAD64; PAD64;
		};


		struct c_object_identifier
		{
			int32 UniqueId;
			int16 OriginBspIndex; // block_index
			byte_enum Type;
			byte_enum Source; // Structure, Editor, Dynamic or Legacy
		};

		struct s_scenario_object_datum
		{
			long_flags PlacementFlags;
			real_point3d Position;
			real_euler_angles3d Rotation;
			real Scale;
			word_flags TransformFlags;
			word_flags ManualBspFlags;
			c_object_identifier ObjectId;
			byte_enum BspPolicy;
			PAD8;
			int16 EditorFolder; // block_index
		};

		struct s_scenario_object_permutation
		{
			string_id VariantName;
			long_flags ActiveChangeColors;
			rgb_color ChangeColors[4]; // Primary, Secondary, Tertiary, and Quaternary
		};

		struct s_scenario_object
		{
			int16 Type; // block_index, object_palette_block
			int16 Name; // block_index, object_names_block
			s_scenario_object_datum ObjectData;
			s_scenario_object_permutation PermutationData;
		};




		struct s_scenario_scenery_datum
		{
			_enum PathfindingPolicy;
			_enum LightmappingPolicy;
			tag_block PathFindingReferences;
			PAD16;
			word_flags ValidMultiplayerGames;
		};
		struct s_scenario_scenery : s_scenario_object
		{
			s_scenario_scenery_datum SceneryData;
		};


		struct s_scenario_equipment_datum
		{
			long_flags EquipmentFlags;
		};
		struct s_scenario_equipment : s_scenario_object
		{
			s_scenario_equipment_datum EquipmentData;
		};


		struct s_scenario_weapon_datum
		{
			int16 RoundsLeft, RoundsLoaded;
			long_flags Flags;
		};
		struct s_scenario_weapon : s_scenario_object
		{
			s_scenario_weapon_datum WeaponData;
		};


		struct s_scenario_sound_scenery_datum
		{
			long_enum VolumeType;
			real Height;
			real_bounds OverrideDistanceBounds;
			angle_bounds OverrideConeAngleBounds;
			real OverrideOuterConeGain;
		};
		struct s_scenario_sound_scenery : s_scenario_object
		{
			s_scenario_sound_scenery_datum SceneryData;
		};


		struct s_scenario_crate : s_scenario_object
		{
			// no create specific data
		};


		struct s_scenario_creature : s_scenario_object
		{
			// no creature specific data
		};




		struct s_scenario_unit_datum
		{
			real BodyVitality;
			long_flags Flags;
		};
		struct s_scenario_unit : s_scenario_object
		{
			s_scenario_unit_datum UnitData;
		};

		struct s_scenario_biped : s_scenario_unit
		{
		};
		struct s_scenario_vehicle : s_scenario_unit
		{
		};



		struct scenario_device_group
		{
			tag_string Name;
			real InitialValue;
			long_flags Flags;
		};

		struct s_scenario_device_datum
		{
			int16 PowerGroup; // block_index, scenario_device_group
			int16 PositionGroup; // block_index, scenario_device_group
			long_flags Flags;
		};
		struct s_scenario_device : s_scenario_object
		{
			s_scenario_device_datum DeviceData;
		};

		struct s_scenario_machine_datum
		{
			long_flags Flags;
			tag_block PathfindingReferences;
		};
		struct s_scenario_machine : s_scenario_device
		{
			s_scenario_machine_datum MachineData;
		};

		struct s_scenario_control_datum
		{
			long_flags Flags;
			int16 CustomMessageIndex;
			PAD16;
		};
		struct s_scenario_control : s_scenario_device
		{
			s_scenario_control_datum ControlData;
		};

		struct s_scenario_light_fixture_datum
		{
			real_rgb_color Color;
			real Intensity;
			angle FalloffAngle, CutoffAngle;
		};
		struct s_scenario_light_fixture : s_scenario_device
		{
			s_scenario_light_fixture_datum LightFixtureData;
		};

		struct s_scenario_light_datum
		{
			_enum Type;
			word_flags Flags;
			_enum LightmapType;
			word_flags LightmapFlags;
			real LightmapHalfLife, LightmapLightScale;

			real_point3d TargetPoint;
			real Width, HeightScale;
			real FieldOfView;

			real FalloffDistance, CutoffDistance;
		};
		struct s_scenario_light : s_scenario_device
		{
			s_scenario_light_datum LightData;
		};
	};
};