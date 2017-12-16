/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace e_actor_type
		{
			typedef enum : _enum
			{
				elite,
				jackal,
				grunt,
				hunter,
				engineer,
				// uses elite actor type definition
				assassin,
				// uses marine actor type definition
				player,
				marine,
				crew,
				combat_form,
				infection_form,
				carrier_form,
				// uses sentinel actor type definition
				monitor,
				sentinel,
				// uses grunt actor type definition
				none,
				mounted_weapon,

				k_count
			} type_t;
		}
	}

	namespace blam
	{
		extern cstring g_actor_type_names[AI::e_actor_type::k_count];
	}
}
