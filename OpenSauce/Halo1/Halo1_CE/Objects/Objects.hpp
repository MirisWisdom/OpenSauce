/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/devices/devices.hpp>
#include <blamlib/Halo1/objects/object_definitions.hpp>
#include <blamlib/Halo1/objects/object_lights.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/widgets/widgets.hpp>
#include <blamlib/Halo1/render/render_objects.hpp>
#include <blamlib/Halo1/units/units.hpp>

namespace Yelo
{
	struct s_cache_tag_instance;

	namespace TagGroups
	{
		struct model_animation_graph;

		struct s_object_definition;
	};

	namespace Enums
	{
		enum settings_adjustment_result : long_enum;
	};

	namespace Objects
	{
		s_object_type_definition** ObjectTypeDefinitions();
		s_widget_type_definition* WidgetTypeDefinitions();


		Render::cached_object_render_states_data_t*		CachedObjectRenderStates();


		s_unit_globals_data*							UnitGlobals();


		typedef Memory::DataArray<s_device_groups_datum, 1024> device_groups_data_t;
		device_groups_data_t&							DeviceGroups();


		/////////////////////////////////////////////////////////////////////////

		struct collideable_object_data : TStructImpl(2048) // pad?
		{
		};
		collideable_object_data*						CollideableObject();

		struct cluster_collideable_object_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<cluster_collideable_object_reference_datum, 2048> cluster_collideable_object_reference_data_t;
		cluster_collideable_object_reference_data_t&	ClusterCollideableObjectReference();


		struct collideable_object_cluster_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<collideable_object_cluster_reference_datum, 2048> collideable_object_cluster_reference_data_t;
		collideable_object_cluster_reference_data_t&	CollideableObjectClusterReference();
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
		typedef Memory::DataArray<cluster_noncollideable_object_reference_datum, 2048> cluster_noncollideable_object_reference_data_t;
		cluster_noncollideable_object_reference_data_t&	ClusterNoncollideableObjectReference();


		struct noncollideable_object_cluster_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<noncollideable_object_cluster_reference_datum, 2048> noncollideable_object_cluster_reference_data_t;
		noncollideable_object_cluster_reference_data_t&	NoncollideableObjectClusterReference();
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// Object widgets
		typedef Memory::DataArray<s_widget_datum, 64> widget_data_t;
		widget_data_t&			Widgets();


		struct s_flag_datum : TStructImpl(5820)
		{
		};
		typedef Memory::DataArray<s_flag_datum, 2> flag_data_t;
		flag_data_t&			Flags();


		struct s_antenna_datum : TStructImpl(700)
		{
		};
		typedef Memory::DataArray<s_antenna_datum, 12> antenna_data_t;
		antenna_data_t&			Antennas();


		struct s_glow_datum : TStructImpl(604)
		{
		};
		typedef Memory::DataArray<s_glow_datum, 8> glow_data_t;
		glow_data_t&			Glow();


		struct s_glow_particles_datum : TStructImpl(100)
		{
		};
		typedef Memory::DataArray<s_glow_particles_datum, 512> glow_particles_data_t;
		glow_particles_data_t&	GlowParticles();


		struct s_light_volumes_datum : Memory::s_datum_base_aligned
		{
			datum_index definition_index; // msg2
		}; BOOST_STATIC_ASSERT( sizeof(s_light_volumes_datum) == 0x8 );
		typedef Memory::DataArray<s_light_volumes_datum, 256> light_volumes_data_t;
		light_volumes_data_t&	LightVolumes();


		struct s_lightnings_datum : Memory::s_datum_base_aligned
		{
			datum_index definition_index; // elec
		}; BOOST_STATIC_ASSERT( sizeof(s_lightnings_datum) == 0x8 );
		typedef Memory::DataArray<s_lightnings_datum, 256> lightnings_data_t;
		lightnings_data_t&		Lightnings();
		//////////////////////////////////////////////////////////////////////////


		void Initialize();
		void Dispose();
		void InitializeForNewMap();
		void DisposeFromOldMap();

		void PLATFORM_API Update();

		void InitializeForYeloGameState(bool enabled);

		// Get the current state of the engine's vehicle remapper
		bool VehicleRemapperEnabled();
		// Turn the engine's vehicle remapper on or off
		void VehicleRemapperEnable(bool enabled);
	};
};

#include "Objects/Objects.WeaponSettings.hpp"