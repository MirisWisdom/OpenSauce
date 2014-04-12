/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/actor_perception.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace AI
	{
		struct s_prop_datum : TStructImpl(312)
		{
			TStructGetPtrImpl(datum_index,		OwnerActorIndex, 0x4);
			TStructGetPtrImpl(datum_index,		NextPropIndex, 0x8);
			TStructGetPtrImpl(datum_index,		ParentPropIndex, 0xC); // guess these are union'd?
			TStructGetPtrImpl(datum_index,		OrphanPropIndex, 0xC);

			//TStructGetPtrImpl(bool,				, 0x14);
			//TStructGetPtrImpl(datum_index,			, 0x18); // unit_index
			//TStructGetPtrImpl(datum_index,			, 0x1C); // actor_index (swarm?)
			TStructGetPtrImpl(_enum,			State, 0x24);

			TStructGetPtrImpl(Enums::actor_perception_type,			Perception, 0x30);

			TStructGetPtrImpl(bool,				Enemy, 0x60);

			TStructGetPtrImpl(int16,			UnopposableCasualtiesInflicted, 0xA6);
			//TStructGetPtrImpl(int16,			, 0xA8);

			TStructGetPtrImpl(bool,				Dead, 0x127);
		};
		typedef Memory::DataArray<	s_prop_datum, 
									768> 
			prop_data_t;

		prop_data_t&					Props();
	};
};