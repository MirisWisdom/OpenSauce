/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct s_sound_permutation;
	};

	namespace blam
	{
		void PLATFORM_API sound_cache_open();
		void PLATFORM_API sound_cache_close();

		bool PLATFORM_API sound_cache_sound_request(TagGroups::s_sound_permutation* sound, 
			// block										increment software_reference_count
			bool block_thread = false, bool load = true, bool reference = false);

		void PLATFORM_API sound_cache_sound_finished(TagGroups::s_sound_permutation* sound);
	};
};