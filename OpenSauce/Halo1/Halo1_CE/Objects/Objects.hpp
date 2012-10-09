/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/Data.hpp"

#include <blamlib/Halo1/devices/devices.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/widgets/widgets.hpp>
#include <blamlib/Halo1/units/units.hpp>

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

	namespace Objects
	{
		s_object_type_definition** ObjectTypeDefinitions();
		s_widget_type_definition* WidgetTypeDefinitions();


		struct s_cached_object_render_states_datum : TStructImpl(256)
		{
		};
		typedef Memory::DataArray<s_cached_object_render_states_datum, 256> t_cached_object_render_states_data;
		t_cached_object_render_states_data*				CachedObjectRenderStates();


		s_unit_globals_data*								UnitGlobals();


		typedef Memory::DataArray<s_device_groups_datum, 1024> t_device_groups_data;
		t_device_groups_data*							DeviceGroups();


		typedef Memory::DataArray<s_object_header_datum, 2048> t_object_header_data;
		t_object_header_data*							ObjectHeader();

		s_objects_pool_data*								ObjectsPool();

		s_object_globals_data*							ObjectGlobals();

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

		void InitializeForYeloGameState(bool enabled);

		void LoadSettings(TiXmlElement* objects_element);
		void SaveSettings(TiXmlElement* objects_element);

		void MultiTeamVehiclesSet(bool enabled);
		void VehicleRemapperEnable(bool enabled);


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

		size_t PredictMemoryPoolUsage(Enums::object_type type, int32 node_count, bool include_yelo_upgrades = false);

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