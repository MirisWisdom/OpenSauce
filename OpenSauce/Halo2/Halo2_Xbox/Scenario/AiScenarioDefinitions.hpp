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
		struct squad_group_definition
		{
			string_id Name;
			int16 ParentGroup;
			int16 InitialOrders; // orders_block
		};


		struct actor_starting_location_definition
		{
			string_id Name; // old sid
			real_point3d Position;
			int16 ReferenceFrame;
			PAD16;
			real_euler_angles2d Fracing;
			long_flags Flags;
			int16 CharacterType, // character_palette_block
				InitialWeapons[2+1], // scenario_weapon_palette_block
				VehicleType; // scenario_vehicle_palette_block
			_enum SeatType;
			_enum GrenadeType;
			int16 SwarmCount;
			string_id ActorVariantName, VehicleVariantName;
			real InitialMovementDistance;
			int16 EmitterVehicle; // scenario_vehicle_block
			_enum InitialMovementMode;

			tag_string PlacementScript;
			int16 _Skip;
			PAD16;
		};
		struct squad_definition
		{
			tag_string Name;
			long_flags Flags;
			_enum Team;
			int16 ParentGroup;
			real SquadDelayTime;
			int16 DifficultyCounts[2]; // Normal, Insane
			_enum MajorUpgrade;
			PAD16;

			int16 VehicleType, // scenario_vehicle_palette_block
				CharacterType, // character_palette_block
				InitialZone; // zone_block
			PAD16;
			int16 InitialWeapons[2]; // scenario_weapon_palette_block
			_enum GrenadeType;
			int16 InitialOrder;
			string_id VehicleVariant;
			TagBlock<actor_starting_location_definition> StartingLocations;

			tag_string PlacementScript;
			int16 _Skip;
			PAD16;
		};


		struct firing_position_definition
		{
			real_point3d Position;
			int16 ReferenceFrame;
			word_flags Flags;
			int16 AreaIndex;

			int16 ClusterIndex;
			int16 _Skip;

			real_euler_angles2d Normal;
		};
		struct flight_hint_reference
		{
			int16 FlightHintIndex, PointIndex;
		};
		struct area_definition
		{
			tag_string Name;
			long_flags AreaFlags;

			byte _Skip[20 + 4];
			TAG_PAD(int32, 16);
			int16 ManualReferenceFrame;
			PAD16;
			TagBlock<flight_hint_reference> FlightHints;
		};
		struct zone_definition
		{
			tag_string Name;
			long_flags Flags;
			int16 ManualBsp;
			PAD16;
			TagBlock<firing_position_definition> FiringPositions;
			TagBlock<zone_definition> Areas;
		};


		struct order_trigger_reference
		{
			long_flags TriggerFlags;
			int16 TriggerIndex; // triggers_block
			PAD16;
		};
		struct ai_scene_trigger
		{
			_enum CombinationRule;
			PAD16;
			TagBlock<order_trigger_reference> Triggers;
		};

		struct ai_scene_role
		{
			string_id Name;
			_enum Group;
			PAD16;
			TagBlock<string_id> RoleVariants; // struct ai_scene_role_variant
		};

		struct ai_scene
		{
			string_id Name;
			long_flags Flags;
			TagBlock<ai_scene_trigger> TriggerConditions;
			TagBlock<ai_scene_role> Roles;
		};
	};
};