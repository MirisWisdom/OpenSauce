/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Networking
	{
		struct s_network_client_time
		{
			enum { TIME_SYNCH_SAMPLES = 0x1E };
			struct s_time_sample : TStructImpl(0x14)
			{
			};

			int32 time_delta;
			int32 number_of_time_samples;
			PAD32; // unknown
			s_time_sample samples[TIME_SYNCH_SAMPLES];
		}; BOOST_STATIC_ASSERT( sizeof(s_network_client_time) == 0x264 );
	};
};