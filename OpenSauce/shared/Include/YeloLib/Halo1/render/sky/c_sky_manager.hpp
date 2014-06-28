/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

namespace Yelo
{
	namespace Render { namespace Sky
	{
		class c_sky_manager
		{
			class s_sky_entry
			{
				datum_index m_sky_index;
				bool m_is_override;
				std::shared_ptr<s_sky_entry> m_original_sky_entry;

			public:
				s_sky_entry(datum_index sky_index);
				s_sky_entry(datum_index sky_index, std::shared_ptr<s_sky_entry> sky_entry);

				bool IsOverride();

				datum_index Get();

				std::shared_ptr<s_sky_entry> OriginalSky();
			};
			typedef std::shared_ptr<s_sky_entry> t_sky_entry_ptr;
			typedef std::vector<t_sky_entry_ptr> t_sky_entry_list;

			t_sky_entry_list m_sky_list;

		public:
			void Reset();
			void Clear();

			void SetScenarioSkies(const TagBlock<tag_reference>& skies);
			void SetSkyIndex(byte sky_index, datum_index sky_datum);
			datum_index GetSkyDatum(byte sky_index);
		};
	};};
};