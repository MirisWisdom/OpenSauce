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
		enum actor_action : _enum
		{
			_actor_action_none,
			_actor_action_sleep,
			_actor_action_alert,
			_actor_action_fight,
			_actor_action_flee,
			_actor_action_uncover,
			_actor_action_guard,
			_actor_action_search,
			_actor_action_wait,
			_actor_action_vehicle,
			_actor_action_charge,
			_actor_action_obey,
			_actor_action_converse,
			_actor_action_avoid,

			k_number_of_actor_actions
		};

		enum action_class : _enum
		{
			_action_class_passive = 1,
		};
	};

	namespace AI
	{
		typedef void (PLATFORM_API* proc_action_function)(datum_index actor_index);
		typedef void (PLATFORM_API* proc_action_function_2C)(datum_index actor_index, datum_index, datum_index);
		struct s_action_function_definition
		{
			Enums::actor_action action;
			PAD16;
			cstring name;
			const real_argb_color debug_color;
			size_t action_data_size;
			Enums::action_class action_class;
			PAD16;
			proc_action_function begin;
			proc_action_function perform;
			proc_action_function update;
			proc_action_function control;
			proc_action_function end;
			proc_action_function func28;
			proc_action_function_2C func2C;
			proc_action_function func30;
			proc_action_function func34;
			proc_action_function func38;
		};

		// TODO: move to ai/action_flee.hpp
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

		// TODO: move to ai/action_fight.hpp
		struct s_action_fight_state
		{
			UNKNOWN_TYPE(int16); // timer
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_action_fight_state) == 0x4 );
	};
};