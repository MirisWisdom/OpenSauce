/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_circular_queue_signature = 'circ',
		};
	};

	namespace Memory
	{
		struct s_circular_queue
		{
			cstring name;
			tag signature;
			int32 read_offset;
			int32 write_offset;
			int32 buffer_size;
			void* buffer;
		};
	};
};