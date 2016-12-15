/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct sound_environment
		{
			PAD32;
			int16 priority;
			PAD16;
			real room_intensity_db;
			real room_intensity_hf_db;
			real room_rolloff_factor;
			real decay_time;
			real decay_hf_ratio;
			real reflections_intensity_db;
			real reflections_delay;
			real reverb_intensity_db;
			real reverb_delay;
			real diffusion;
			real density;
			real hf_reference;
			PAD128;
		}; BOOST_STATIC_ASSERT( sizeof(sound_environment) == 0x48 );
	};
};