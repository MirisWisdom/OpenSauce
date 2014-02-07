/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/time/sequence/c_transition_sequence_controller.hpp>

namespace Yelo
{
	namespace Render { namespace Lightmaps
	{
		typedef void (*t_set_lightmap_set_func)(const TagGroups::s_scenario_information_bsp_lightmap_set*);

		class c_lightmap_sequence_controller : public Time::Sequence::c_transition_sequence_controller<Enums::k_maximum_structure_bsps_per_scenario_upgrade>
		{
			t_set_lightmap_set_func	m_set_lightmap_set_func;

			struct
			{
				int16 bsp_info_index;
				const TagBlock<const TagGroups::s_scenario_information_bsp>* bsps;
				const TagGroups::s_scenario_information_bsp_lightmap_set* set;
				const TagGroups::s_scenario_information_bsp_lightmap_sequence* sequence;
			}m_current_lightmap;

			void ChangeLightmapSet(const int16 index);

			void OnTransitionUpdate(const int16 instance, const real progress);
			void OnSequenceChanged(const int16 instance, const byte index);

			const TagGroups::s_transition_sequence_block* GetSequenceBlock(const int16 instance, const byte index);

		public:
			void Ctor();
			void Dtor();

			void InitializeNewMap(const TagBlock<const TagGroups::s_scenario_information_bsp>* bsps);
			void DisposeFromOldMap();
			void InitializeNewBSP(const int16 bsp_info_index);
			void DisposeFromOldBSP();

			void SetLightmapSetCallback(t_set_lightmap_set_func set_func);
		};
	}; };
};