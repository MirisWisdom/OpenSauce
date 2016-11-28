/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace AI
	{
		struct s_action_flee_state : TStructImpl(0x30)
		{
			//TStructGetPtrImpl(word_flags?,			, 0x0);
			//TStructGetPtrImpl(int16,				flee_stationary_ticks, 0x2);
			//TStructGetPtrImpl(bool,				, 0x4);
			//TStructGetPtrImpl(bool,				, 0x5);
			//TStructGetPtrImpl(bool,				find_new_flee_position, 0x6);
			//PAD8
			//TStructGetPtrImpl(int16,				flee_firing_position_index, 0x8);

			//TStructGetPtrImpl(bool,				, 0xA);

			//TStructGetPtrImpl(_enum,				, 0xC);
			//TStructGetPtrImpl(bool,				unable_to_flee, 0xE);
			//TStructGetPtrImpl(bool,				done_fleeing, 0xF);
			//TStructGetPtrImpl(bool,				, 0x10); // true if they're blindly fleeing and speaking
			//PAD24;?
			//TStructGetPtrImpl(int32,				, 0x14); // game time, last-time-of-speaking? will only update every 60 ticks

			//TStructGetPtrImpl(datum_index,				, 0x1C); // prop
		};
	};
};