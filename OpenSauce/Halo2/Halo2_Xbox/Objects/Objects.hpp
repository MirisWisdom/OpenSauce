/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>
#include <blamlib/Halo2/memory/memory_pool.hpp>

#include "Objects/ObjectDefinitions.hpp"
#include "Objects/DeviceDefinitions.hpp"
#include "Objects/ItemDefinitions.hpp"
#include "Objects/UnitDefinitions.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum object_type
		{
			_object_type_object = -4,
			_object_type_device = -3,
			_object_type_item = -2,
			_object_type_unit = -1,

			_object_type_biped = 0,
			_object_type_vehicle,
			_object_type_weapon,
			_object_type_equipment,
			_object_type_garbage,
			_object_type_projectile,
			_object_type_scenery,
			_object_type_machine,
			_object_type_control,
			_object_type_light_fixture,
			_object_type_sound_scenery,
			_object_type_crate,
			_object_type_creature,
			_object_type,
		};
	};

	namespace Flags
	{
		enum object_type
		{
			_object_type_biped =		FLAG(Enums::_object_type_biped),
			_object_type_vehicle =		FLAG(Enums::_object_type_vehicle),
			_object_type_weapon =		FLAG(Enums::_object_type_weapon),
			_object_type_equipment =	FLAG(Enums::_object_type_equipment),
			_object_type_garbage =		FLAG(Enums::_object_type_garbage),
			_object_type_projectile =	FLAG(Enums::_object_type_projectile),
			_object_type_scenery =		FLAG(Enums::_object_type_scenery),
			_object_type_machine =		FLAG(Enums::_object_type_machine),
			_object_type_control =		FLAG(Enums::_object_type_control),
			_object_type_light_fixture =FLAG(Enums::_object_type_light_fixture),
			_object_type_sound_scenery =FLAG(Enums::_object_type_sound_scenery),
			_object_type_crate =		FLAG(Enums::_object_type_crate),
			_object_type_creature =		FLAG(Enums::_object_type_creature),

			_object_type_mask_object =
				_object_type_biped |
				_object_type_vehicle |
				_object_type_weapon |
				_object_type_equipment |
				_object_type_garbage |
				_object_type_projectile |
				_object_type_scenery |
				_object_type_machine |
				_object_type_control |
				_object_type_light_fixture |
				_object_type_sound_scenery |
				_object_type_crate |
				_object_type_creature,

			_object_type_mask_device =
				_object_type_machine |
				_object_type_control |
				_object_type_light_fixture,

			_object_type_mask_item =
				_object_type_weapon |
				_object_type_equipment |
				_object_type_garbage,

			_object_type_mask_unit =
				_object_type_biped |
				_object_type_vehicle,
		};

		enum object_datum_flag
		{
			// 0x4
			_object_datum_1_flag = FLAG(0),
			_object_datum_2_flag = FLAG(1),
			_object_datum_3_flag = FLAG(2),
			_object_datum_4_flag = FLAG(3),
			_object_datum_5_flag = FLAG(4),
			_object_datum_6_flag = FLAG(5),
			_object_datum_7_flag = FLAG(6),
			_object_datum_8_flag = FLAG(7),
			_object_datum_9_flag = FLAG(8),
			_object_datum_10_flag = FLAG(9),
			_object_datum_11_flag = FLAG(10),
			_object_datum_12_flag = FLAG(11),
			_object_datum_13_flag = FLAG(12),
			_object_datum_14_flag = FLAG(13),
			_object_datum_15_flag = FLAG(14),
			_object_datum_16_flag = FLAG(15),
			_object_datum_17_flag = FLAG(16),
			_object_datum_deleted_when_deactived_flag = FLAG(17),
			_object_datum_19_flag = FLAG(18),
			_object_datum_cinematic_lod_flag = FLAG(19),
			_object_datum_something_about_lighting_flag = FLAG(20),
			_object_datum_cinematic_visibility_flag = FLAG(21),
			_object_datum_23_flag = FLAG(22),
			_object_datum_24_flag = FLAG(23),
			_object_datum_uses_cinematic_lighting_flag = FLAG(24),
			_object_datum_26_flag = FLAG(25),
			_object_datum_27_flag = FLAG(26),
			_object_datum_28_flag = FLAG(27),
			_object_datum_29_flag = FLAG(28),
			_object_datum_30_flag = FLAG(29),
			_object_datum_31_flag = FLAG(30),
			_object_datum_32_flag = FLAG(31),

			_object_datum_cannot_take_damage_bit = 7,
			_object_datum_cannot_die_bit = 6,
		};
	};

	namespace Objects
	{
		typedef void (PLATFORM_API* proc_object_type)(datum_index object_datum);

		struct s_type_definition
		{
			cstring name; // 0x0
			tag group_tag; // 0x4
			short datum_size; // 0x8
			short placement_tag_block_offset; // 0xA
			short palette_tag_block_offset; // 0xC
			short placement_tag_block_size; // 0xE
			proc_initialize initialize; // 0x10
			proc_dispose dispose; // 0x14
			proc_initialize_for_new_map initialize_for_new_map; // 0x18
			proc_dispose_from_old_map dispose_from_old_map; // 0x1C
			proc_object_type adjust_placement; // 0x20
			proc_object_type disconnect_from_structure_bsp; // 0x24
			proc_object_type place; // 0x28
			void* _new; // 0x2C
			void* update; // 0x30
			void* UNKNOWN(34); // 0x34
			void* UNKNOWN(38); // 0x38
			void* UNKNOWN(_0); // 0x3C
			void* UNKNOWN(_1); // 0x40, may have something to do with object type specific animation processing
			void* UNKNOWN(_2); // 0x44
			void* UNKNOWN(48); // 0x48
			void* export_function_values; // 0x4C
			void* UNKNOWN(50); // 0x50, may have something to do with data encoding
			void* UNKNOWN(54); // 0x54
			void* handle_deleted_object; // 0x58
			void* UNKNOWN(5c); // 0x5C
			void* UNKNOWN(60); // 0x60
			void* UNKNOWN(64); // 0x64
			void* UNKNOWN(_5); // 0x68
			void* UNKNOWN(6c); // 0x6C
			void* UNKNOWN(70); // 0x70
			void* UNKNOWN(74); // 0x74
			void* UNKNOWN(_6); // 0x78
			void* UNKNOWN(7c); // 0x7C
			void* UNKNOWN(_7); // 0x80
			s_type_definition* base_object_types[2]; // 0x84
			s_type_definition* object_type; // 0x8C
			TAG_PAD(int32, 13); // 0x90
			s_type_definition* next; // 0xC4
		};

		// animation layout
		// sizeof(int32)*3
		// node matrices {
		// sizeof(int32)*2
		// node_matix matrix
		// }
		// animation frame info & data []
		// animation info blobs
		// blah

		struct s_object_node_matrix
		{
			//float BoundingBox[3][2];
			real_matrix4x3 Matrix;
		};

		struct s_object_animation_frame
		{
		};

		struct s_object_animation_datum
		{
			PAD(0, 4);
			PAD(1, 2);
			int16 Index;
			datum_index AnimationDefinition;
			int16 UNKNOWN(0);
			int16 UNKNOWN(1);
			string_id Animation;
			string_id UNKNOWN(2);
			string_id UNKNOWN(3);
			string_id PreviousAnimation;
			int16 Size;
		};

		// !-- OBJECTS --!

		struct s_object_creation_data : TStructImpl(200)
		{
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			TStructGetPtrImpl(byte_enum, Type, 0x0A);
			TStructGetPtrImpl(datum_index, ScenarioDatumIndex, 0x10);
			TStructGetPtrImpl(real, Scale, 0x58);
#else
#endif
		};



		// returns true if [type] exists in the object_type mask [mask]
		inline bool ObjectTypeInMask(long_flags flags, _enum type) { return TEST_FLAG(flags, type); }
		inline bool ObjectTypesInMask(long_flags flags, long_flags types) { return (flags & types) != 0; }

		// Gets the angle and distance of [target_object] relative to [source_object]
		void GetAngleAndDistance(datum_index source_object, datum_index target_object, real& angle, real& distance);

		void GetObjectInView(datum_index source_object, long_flags types_mask, datum_index::index_t& index, real& distance);
	};

	namespace GameState
	{
		struct s_device_groups_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<s_device_groups_datum, 1024> t_device_groups_data;
		t_device_groups_data* _DeviceGroups();

		struct s_object_header_datum : Memory::s_datum_base
		{
			// bit 0 - _object_header_active_bit
			// bit 1 - _object_header_requires_motion_bit
			// bit 3 - _object_header_being_deleted_bit
			// bit 5 - _object_header_connected_to_map_bit
			// bit 6 - _object_header_child_bit
			byte_flags Flags;
			byte_enum Type;
			int16 ClusterIndex;
			uint16 Size;

			union {
				void* Address;
				union _As {
					s_object_data* Object;
					s_device_datum* Device;
					s_item_datum* Item;
					s_unit_datum* Unit;

					s_scenery_datum* Scenery;
					s_sound_scenery_datum* SoundScenery;
					s_projectile_datum* Projectile;
					s_crate_datum* Crate;
					s_creature_datum* Creature;

					s_device_control_datum* Control;
					s_device_machine_datum* Machine;
					s_device_lightfixture_datum* LightFixture;

					s_weapon_datum* Weapon;
					s_equipment_datum* Equipment;
					s_garbage_datum* Garbage;

					s_biped_datum* Biped;
					s_vehicle_datum* Vehicle;
				}As;
			};
		};
		typedef Memory::DataArray<s_object_header_datum, 2048> t_object_data;
		t_object_data* _Object();

		Memory::s_memory_pool* _Objects();

		struct s_object_name_list
		{
			datum_index List[640];
		};
		s_object_name_list* _ObjectNameList();

		struct s_object_globals : TStructImpl(132)
		{
		};
		s_object_globals* _ObjectGlobals();


		struct s_object_cluster_datum_1 : TStructImpl(12 + 20)
		{
		};
		struct s_object_cluster_data_1 : Memory::DataArray<s_object_cluster_datum_1, 2048>
		{
			byte UNKNOWN(0)[288]; // I think this is actually 256...
		};


		struct s_object_cluster_datum_2 : TStructImpl(12)
		{
		};
		struct s_object_cluster_data_2 : Memory::DataArray<s_object_cluster_datum_2, 2048>
		{
			byte UNKNOWN(0)[256];
		};

		struct s_object_cluster_data
		{
			s_object_cluster_data_1 ClusterObj;
			s_object_cluster_data_2 ObjCluster;
		};
		struct s_object_clusters
		{
			datum_index References[512];
			s_object_cluster_data Collideable;
			s_object_cluster_data Noncollideable;
		};
		s_object_clusters* _ObjectClusters();

		struct s_object_scripting_globals : TStructImpl(772)
		{
		};
		s_object_scripting_globals* _ObjectScriptingGlobals();

		struct s_cached_object_render_states_datum : TStructImpl(256)
		{
		};
		typedef Memory::DataArray<s_cached_object_render_states_datum, 256> t_cached_object_render_states_data;
		t_cached_object_render_states_data* _CachedObjectRenderStates();

		struct s_object_placement_data
		{
			int16 PlacedStructureBspIndex;
			PAD16;
		};
		s_object_placement_data* _ObjectPlacementData();




		// Gets a pointer to a object in the game state
		template<typename T> T* ObjectTryAndGet(datum_index object_index, T** out = NULL, uint16* out_size = NULL)
		{
			s_object_header_datum* obj;
			T* inst = NULL;


			obj = (*_Object())[object_index];

			if(!obj->IsNull())
			{
				inst = (T*)obj->Address;

				if(out != NULL) *out = inst;
				if(out_size != NULL) *out_size = obj->Size;
			}

			return inst;
		}
		// Gets the total count of active objects that are of type [desired_type]
		int32 ObjectGetTypeCount(_enum desired_type);


		// Gets an object in the game state based on its index in the scenario_group->objects block
		template<typename T> T* ScenarioObjectGet(int16 scenario_object_index, T** out = NULL, uint16* out_size = NULL)
		{
			return ObjectTryAndGet<T>(_ObjectNameList()->List[scenario_object_index], out, out_size);
		}
	};
};