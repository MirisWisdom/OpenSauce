/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/cseries/cseries_base.hpp>
#include <blam/memory/datum_index.hpp>
#include <blam/math/color_math.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace e_actor_action
		{
			typedef enum : _enum
			{
				none,
				sleep,
				alert,
				fight,
				flee,
				uncover,
				guard,
				search,
				wait,
				vehicle,
				charge,
				obey,
				converse,
				avoid,

				k_count
			} type_t;
		}

		namespace e_action_class
		{
			typedef enum : _enum
			{
				passive = 1,

				k_count
			} type_t;
		}

		typedef void (PLATFORM_API* proc_action_function_t)(
			datum_index actor_index);
		typedef void (PLATFORM_API* proc_action_function_2C_t)(
			datum_index actor_index,
			datum_index,
			datum_index);

		struct s_action_function_definition
		{
			e_actor_action::type_t action;
			PAD16;
			cstring name;
			const real_argb_color debug_color;
			size_t action_data_size;
			e_action_class::type_t action_class;
			PAD16;
			proc_action_function_t begin;
			proc_action_function_t perform;
			proc_action_function_t update;
			proc_action_function_t control;
			proc_action_function_t end;
			proc_action_function_t func28;
			proc_action_function_2C_t func2C;
			proc_action_function_t func30;
			proc_action_function_t func34;
			proc_action_function_t func38;
		};
	}
}
