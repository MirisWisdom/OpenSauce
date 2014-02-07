/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/settings/yelo_settings_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

namespace Yelo
{
	namespace Render { namespace Sky
	{
		class c_sky_manager
		{
			int16													m_sky_index;
			datum_index												m_sky_datum;

			TagBlock<tag_reference>*								m_scenario_sky_refs;
			const TagBlock<TagGroups::s_scenario_information_sky>*	m_sky_override_refs;
			byte*													m_sky_index_gamestate;

		public:
			void Ctor();
			void Dtor();

			void SetGamestate(byte* gamestate);
			void SetSkyBlocks(TagBlock<tag_reference>* scenario, const TagBlock<TagGroups::s_scenario_information_sky>* overrides);

			void SetSkyIndex(byte sky_index);
			void SetSkyOverride(byte scenario_sky_index, byte override_sky_index);
			void GetCurrentSkyDatum(datum_index& sky);
		};
	};};
};