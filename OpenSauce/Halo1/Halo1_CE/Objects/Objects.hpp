/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/Data.hpp"

#include "Objects/ObjectDefinitions.hpp"
#include "Objects/DeviceDefinitions.hpp"
#include "Objects/ItemDefinitions.hpp"
#include "Objects/UnitDefinitions.hpp"

namespace Yelo
{
	struct s_cache_tag_instance;

	namespace TagGroups
	{
		struct model_animation_graph;

		struct s_object_definition;
	};

	namespace Flags
	{
		enum object_header_flags : byte_flags
		{
			_object_header_active_bit = 0,
			_object_header_requires_motion_bit = 1,
			// 2
			_object_header_being_deleted_bit = 3,
			// 4
			_object_header_connected_to_map_bit = 5,
			_object_header_child_bit = 6,
		};
	};

	namespace Objects
	{
		typedef bool (PLATFORM_API* proc_object_type)(datum_index object_datum);
		typedef int32 (PLATFORM_API* proc_object_type_create_to_network)(datum_index object_index, void* buffer, int32 buffer_size_in_bits);
		typedef bool (PLATFORM_API* proc_object_type_network)(void* unk, void* header, void* client);
		struct s_object_type_definition
		{
			cstring name;													// 0x0
			tag group_tag;													// 0x4
			int16 datum_size;												// 0x8
			int16 placement_tag_block_offset;								// 0xA
			int16 palette_tag_block_offset;									// 0xC
			int16 placement_tag_block_size;									// 0xE
			int32 update_message_type;										// 0x10
			proc_initialize				proc_initialize;					// 0x14
			proc_dispose				proc_dispose;						// 0x18
			proc_initialize_for_new_map proc_initialize_for_new_map;		// 0x1C
			proc_dispose_from_old_map	proc_dispose_from_old_map;			// 0x20
			proc_object_type			proc_adjust_placement;				// 0x24
			proc_object_type 			proc_new;							// 0x28
			proc_object_type 			proc_place;							// 0x2C
			proc_object_type 			proc_delete;						// 0x30
			proc_object_type 			proc_update;						// 0x34
			void* proc_export_function_values;								// 0x38
			void* proc_handle_deleted_object;								// 0x3C
			void* proc_handle_region_destroyed;								// 0x40
			void* proc_handle_parent_destroyed;								// 0x44
			void* proc_preprocess_node_orientations;						// 0x48
			void* proc_postprocess_node_matrices;							// 0x4C
			proc_object_type 			proc_reset;							// 0x50
			proc_object_type 			proc_disconnect_from_structure_bsp;	// 0x54
			void* proc_notify_impulse_sound;								// 0x58
			proc_object_type 			proc_render_debug;					// 0x5C

			void* render_message_debug_proc;								// 0x60, bool (*)(datum_index object_index)
			proc_object_type_create_to_network create_to_network_proc;		// 0x64
			void* update_baseline_proc;										// 0x68
			void* build_update_delta_proc;									// 0x6C, int32 (*)(datum_index object_index, void* buffer, int32 buffer_size_in_bits, _enum message_delta_mode)
			void* process_update_delta_proc;								// 0x70
			
			void* is_network_time_valid_proc;								// 0x74
			void* unknown_proc;												// 0x78, unused. Only networked object types implemented this (and they only returned true)
			void* update_network_time_proc;									// 0x7C

			s_object_type_definition* object_type;
			s_object_type_definition* base_object_types[2];
			int32 _unused1[13]; // s_object_type_definition*.
			s_object_type_definition* next;
			int32 _unused2;
		}; BOOST_STATIC_ASSERT( sizeof(s_object_type_definition) == 0xC8 );


		typedef bool (*proc_widget_type)(datum_index object_datum);
		struct s_widget_type_definition
		{
			tag group_tag;
			bool needs_lighting; PAD24;
			proc_initialize				initialize_proc;
			proc_initialize_for_new_map initialize_for_new_map_proc;
			proc_dispose_from_old_map	dispose_from_old_map_proc;
			proc_dispose				dispose_proc;
			proc_widget_type			new_proc;
			proc_widget_type 			delete_proc;
			proc_widget_type 			update_proc;
			proc_widget_type 			render_proc;
		};



		struct s_cached_object_render_states_datum : TStructImpl(256)
		{
		};
		typedef Memory::DataArray<s_cached_object_render_states_datum, 256> t_cached_object_render_states_data;
		t_cached_object_render_states_data*				CachedObjectRenderStates();


		struct s_unit_globals_data
		{
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int32);
		};
		s_unit_globals_data*								UnitGlobals();


		struct s_device_groups_datum : Memory::s_datum_base
		{
			word_flags flags;
			real value;
		};
		typedef Memory::DataArray<s_device_groups_datum, 1024> t_device_groups_data;
		t_device_groups_data*							DeviceGroups();


		struct s_object_header_datum : Memory::s_datum_base
		{
			byte flags;				// Enums::object_header_flags
			byte object_type;
			int16 cluster_index;
			int16 data_size;
			union {
				void* address;

				s_object_data* _object; // Note: since 's_object_data' is the base of all object datums, we can do this

				// Allow us to explicitly cast (without the cast) to a object type 
				// in scenarios were we're programming for something specific (ie, unit types only)
				union {
					s_scenery_datum* _scenery;
					s_sound_scenery_datum* _sound_scenery;
					s_garbage_datum* _garbage;
					s_projectile_datum* _projectile;
					s_placeholder_datum* _placeholder;

					s_device_datum* _device;
					s_device_machine_datum* _machine;
					s_device_control_datum* _control;
					s_device_lightfixture_datum* _lightfixture;

					s_item_datum* _item;
					s_weapon_datum* _weapon;
					s_equipment_datum* _equipment;

					s_unit_datum* _unit;
					s_biped_datum* _biped;
					s_vehicle_datum* _vehicle;
				}Type;
			};
		};
		typedef Memory::DataArray<s_object_header_datum, 2048> t_object_header_data;
		t_object_header_data*							ObjectHeader();

		struct s_objects_pool_data
		{
			Memory::s_memory_pool header;

			byte data[2 * (1024*1024)];
		};
		s_objects_pool_data*								ObjectsPool();

		struct s_object_globals_data
		{
			bool object_is_being_placed;
			bool object_marker_initialized;
			bool garbage_collect_now;
			UNKNOWN_TYPE(bool);
			int16 object_type_being_placed; // script value enum
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(datum_index);
			byte pad[140];
			// _enum object_pvs_set 0x90
			// none = 0
			// object = 1
			// camera = 2
			// short object_pvs_object 0x94
		};
		s_object_globals_data*							ObjectGlobals();

		struct s_object_name_list_data
		{
			datum_index object_name_to_datum_table[512];
		};
		s_object_name_list_data*							ObjectNameList();


		//////////////////////////////////////////////////////////////////////////
		//

		struct collideable_object_data : TStructImpl(2048) // pad?
		{
		};
		collideable_object_data*						CollideableObject();

		struct cluster_collideable_object_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<cluster_collideable_object_reference_datum, 2048> t_cluster_collideable_object_reference_data;
		t_cluster_collideable_object_reference_data*	ClusterCollideableObjectReference();


		struct collideable_object_cluster_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<collideable_object_cluster_reference_datum, 2048> t_collideable_object_cluster_reference_data;
		t_collideable_object_cluster_reference_data*	CollideableObjectClusterReference();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		//
		struct noncollideable_object_data : TStructImpl(2048) // pad?
		{
		};
		noncollideable_object_data*						NoncollideableObject();

		struct cluster_noncollideable_object_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<cluster_noncollideable_object_reference_datum, 2048> t_cluster_noncollideable_object_reference_data;
		t_cluster_noncollideable_object_reference_data*	ClusterNoncollideableObjectReference();


		struct noncollideable_object_cluster_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<noncollideable_object_cluster_reference_datum, 2048> t_noncollideable_object_cluster_reference_data;
		t_noncollideable_object_cluster_reference_data*	NoncollideableObjectClusterReference();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// Object widgets
		struct s_widget_datum : Memory::s_datum_base
		{
			_enum type;
			datum_index type_datum_index; // index to the type's data array (eg, antenna)
			datum_index next_widget_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_widget_datum) == 0xC );
		typedef Memory::DataArray<s_widget_datum, 64> t_widget_data;
		t_widget_data*			Widgets();


		struct s_flag_datum : TStructImpl(5820)
		{
		};
		typedef Memory::DataArray<s_flag_datum, 2> t_flag_data;
		t_flag_data*			Flags();


		struct s_antenna_datum : TStructImpl(700)
		{
		};
		typedef Memory::DataArray<s_antenna_datum, 12> t_antenna_data;
		t_antenna_data*			Antennas();


		struct s_glow_datum : TStructImpl(604)
		{
		};
		typedef Memory::DataArray<s_glow_datum, 8> t_glow_data;
		t_glow_data*			Glow();


		struct s_glow_particles_datum : TStructImpl(100)
		{
		};
		typedef Memory::DataArray<s_glow_particles_datum, 512> t_glow_particles_data;
		t_glow_particles_data*	GlowParticles();


		struct s_light_volumes_datum : TStructImpl(2048)
		{
		};
		typedef Memory::DataArray<s_light_volumes_datum, 8> t_light_volumes_data;
		t_light_volumes_data*	LightVolumes();


		struct s_lightnings_datum : TStructImpl(2048)
		{
		};
		typedef Memory::DataArray<s_lightnings_datum, 256> t_lightnings_data;
		t_lightnings_data*		Lightnings();
		//////////////////////////////////////////////////////////////////////////


		void Initialize();
		void Dispose();
		void InitializeForNewMap();

		void PLATFORM_API Update();

#if !PLATFORM_IS_DEDI
		void LoadSettings(TiXmlElement* objects_element);
		void SaveSettings(TiXmlElement* objects_element);
#endif

		void MultiTeamVehiclesSet(bool enabled);


		void PlacementDataNewAndCopy(s_object_placement_data& data, datum_index src_object_index, 
			datum_index tag_index_override = datum_index::null, datum_index owner_object_index = datum_index::null);

		// Walks the parent object datums of [object_index] until it gets
		// to the upper most parent and returns that parent's
		// datum index
		datum_index GetUltimateObject(datum_index object_index);
		// Walks the next object datums of [object_index] [n] amount of
		// times, and returns the result datum_index.
		// Say, if [n] is zero, it will return the datum_index
		// that is returned by [object_index]'s GetNextObjectIndex
		datum_index GetNextObjectN(datum_index object_index, int32 n = 0);

		TagGroups::s_object_definition const* GetObjectDefinition(datum_index object_index);
		template<typename TObjectDefinition>
		TObjectDefinition const* GetObjectDefinition(datum_index object_index)
		{
			return CAST_PTR(TObjectDefinition const*, GetObjectDefinition(object_index));
		}

		TagGroups::model_animation_graph const* GetObjectAnimations(datum_index object_index);

		real GetObjectDistanceFromPoint(datum_index object_index, const real_vector3d& dest_point);

		// Delete all children of the specified type(s) on the parent object
		void DeleteChildrenByType(datum_index parent, long_flags object_type_mask);
		// Detach all children of the specified type(s) from the parent object
		void DetachChildrenByType(datum_index parent, long_flags object_type_mask);

		namespace Weapon
		{
			void Initialize();
			void Dispose();

#if !PLATFORM_IS_DEDI
			bool AdjustSettings();
			void LoadSettings(TiXmlElement* weapons_element);
			void SaveSettings(TiXmlElement* weapons_element);
#endif

#if defined(DX_WRAPPER)
			void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
			void OnLostDevice();
			void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
			void Render();
			void Release();
#elif PLATFORM_IS_USER
			DOC_TODO("Need fallback initialization for when we're not using the DX_WRAPPER")
#endif
		};

		namespace Vehicle
		{
			void Initialize();
			void Dispose();

#if !PLATFORM_IS_DEDI
			bool AdjustSettings();
			void LoadSettings(TiXmlElement* vehicles_element);
			void SaveSettings(TiXmlElement* vehicles_element);
#endif

#if defined(DX_WRAPPER)
			void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
			void OnLostDevice();
			void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
			void Render();
			void Release();
#elif PLATFORM_IS_USER
			DOC_TODO("Need fallback initialization for when we're not using the DX_WRAPPER")
#endif
		};
	};
};