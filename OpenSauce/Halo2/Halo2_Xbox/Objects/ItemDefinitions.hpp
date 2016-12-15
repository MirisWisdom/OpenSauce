/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once
#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace GameState
	{
		struct s_item_data : TStructImpl(Enums::k_object_size_item - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object };

			TStructSubGetPtrImpl(byte_flags, Flags, 0x12C);
			//TStructSubGetPtrImpl(short, , 0x132);
		};

		struct s_weapon_data : TStructImpl(Enums::k_object_size_weapon - Enums::k_object_size_item)
		{
			enum { DATA_OFFSET = Enums::k_object_size_item };
		};

		struct s_equipment_data : TStructImpl(Enums::k_object_size_equipment - Enums::k_object_size_item)
		{
			enum { DATA_OFFSET = Enums::k_object_size_item };
		};

		struct s_garbage_data : TStructImpl(Enums::k_object_size_garbage - Enums::k_object_size_item)
		{
			enum { DATA_OFFSET = Enums::k_object_size_item };
		};


		struct s_item_datum
		{
			s_object_data object;
			s_item_data item;
		};

		struct s_weapon_datum : s_item_datum
		{
			s_weapon_data weapon;
		};

		struct s_equipment_datum : s_item_datum
		{
			s_equipment_data equipment;
		};

		struct s_garbage_datum : s_item_datum
		{
			s_garbage_data garbage;
		};
	};

	namespace TagGroups
	{
	};
};