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
	namespace Enums
	{
		enum actor_type : _enum;
		enum game_team : _enum;
	};

	namespace AI
	{
		struct s_prop_datum
		{
			uint16 header;							//	0x0
			PAD16;									//	0x2
			datum_index owner_actor_index;			//	0x4
			datum_index next_prop_index;			//	0x8
			datum_index orphan_prop_index;			//	0xC
			Enums::actor_type actor_type;			//	0x10
			Enums::game_team team;					//	0x12
			PAD8; // unknown field					//	0x14
			PAD24;									//	0x15
			datum_index unit_index;					//	0x18
			datum_index actor_index;				//	0x1C
			real danger_radius;						//	0x20
			_enum state;							//	0x24
			PAD16; // unknown field						0x26
			PAD32; // unknown field						0x28
			PAD32; // unknown field						0x2C
			Enums::actor_perception_type perception; //	0x30
			PAD16; // unknown field						0x32
			PAD16; // unknown field						0x34
			PAD16; // unknown field						0x36
			PAD16; // unknown field						0x38
			PAD16; // unknown field						0x3A
			PAD16; // unknown field						0x3C
			PAD16;									//	0x3E
			PAD_TYPE(real_point3d); // unknown field	0x40
			PAD16; // unknown field						0x4C
			PAD8; // unknown field						0x4E
			PAD8;									//	0x4F
			PAD32; // unknown field						0x50
			PAD32; // unknown field						0x54
			PAD32; // unknown field						0x58
			PAD32; // unknown field						0x5C
			bool enemy;								//	0x60
			bool ally;								//	0x61
			PAD8; // unknown field						0x62
			PAD8; // unknown field						0x63
			PAD8; // unknown field						0x64
			PAD8;									//	0x65
			PAD16; // unknown field						0x66
			PAD16; // unknown field						0x68
			PAD16; // unknown field						0x6A
			PAD16; // unknown field						0x6C
			PAD16;									//	0x6E
			PAD32; // unknown field						0x70
			PAD8; // unknown field						0x74
			PAD8;									//	0x75
			PAD16; // unknown field						0x76
			PAD16; // unknown field						0x78
			PAD16;									//	0x7A
			PAD32; // unknown field						0x7C
			PAD_TYPE(real_point3d); // unknown field	0x80
			PAD32; // unknown field						0x8C
			PAD_TYPE(real_point3d); // unknown field	0x90
			PAD16; // unknown field						0x9C
			PAD16;									//	0x9E
			PAD32; // unknown field						0xA0
			bool is_unopposable;					//	0xA4
			PAD8;									//	0xA5
			int16 unopposable_casualties_inflicted; //	0xA6
			PAD16; // unknown field						0xA8
			PAD16; // unknown field						0xAA
			PAD16; // unknown field						0xAC
			PAD16; // unknown field						0xAE
			PAD16; // unknown field						0xB0
			PAD16;									//	0xB2
			PAD32; // unknown field						0xB4
			PAD8; // unknown field						0xB8
			PAD8; // unknown field						0xB9
			PAD8; // unknown field						0xBA
			PAD8; // unknown field						0xBB
			PAD_TYPE(real_point3d); // unknown field	0xBC
			PAD_TYPE(real_point3d); // unknown field	0xC8
			PAD_TYPE(real_vector3d); // unknown field	0xD4
			PAD_TYPE(real_vector3d); // unknown field	0xE0
			PAD32; // unknown field						0xEC
			PAD_TYPE(real_point3d); // unknown field	0xF0
			s_scenario_location location;			//	0xFC
			PAD_TYPE(real_point3d); // unknown field	0x104
			PAD32; // unknown field						0x110
			PAD32; // unknown field						0x114
			PAD8; // unknown field						0x118
			PAD24;									//	0x119
			PAD32; // unknown field						0x11C
			PAD8; // unknown field						0x120
			PAD8; // unknown field						0x121
			PAD8; // unknown field						0x122
			PAD8; // unknown field						0x123
			PAD8; // unknown field						0x124
			PAD8; // unknown field						0x125
			PAD8; // unknown field						0x126
			bool dead;								//	0x127
			PAD8; // unknown field						0x128
			PAD8; // unknown field						0x129
			PAD8; // unknown field						0x12A
			bool actor_is_noncombat;				//	0x12B
			PAD8; // unknown field						0x12C
			PAD8; // unknown field						0x12D
			PAD8; // unknown field						0x12E
			PAD8; // unknown field						0x12F
			PAD8; // unknown field						0x130
			PAD8; // unknown field						0x131
			PAD8; // unknown field						0x132
			bool ignored;							//	0x133
			PAD8; // unknown field						0x134
			PAD8; // unknown field						0x135
			PAD8; // unknown field						0x136
			PAD8; // unknown field						0x137
		}; BOOST_STATIC_ASSERT(sizeof(s_prop_datum) == 0x138);

		typedef Memory::DataArray<	s_prop_datum, 
									768> 
			prop_data_t;

		prop_data_t&					Props();
	};
};