/*
Yelo: Open Sauce SDK
Halo 1 (CE) Edition

See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum
		{
			_object_looping_sound_unk0_bit,					// when set uses real @ 0x8 for scaling
			_object_looping_sound_unk1_bit,					// real @ 0x8 is valid if this is NOT set, only used if unk0_bit is set
		};
	}
	namespace Sound
	{
		// seems to be mostly just function pointers
		struct sound_engine_definition
		{
			// void (*)(int, bool, _enum);										// 0x1C fill buffer related, args: soundbuffer_index, bool?, sound class
		};

		struct s_sound_datum : TStructImpl(0xB0)
		{
			// int16															// 0x2 != 0 means looping sound ?

			// int16															// 0x6 index for something ?
			// datum_index sound_index;											// 0x8
			// datum_index parent_index;										// 0xC  eg index to object or looping sound
			// bool (*)(datum_index, void* parent_data, void* output)			// 0x10 gets transform using parent_index
			// int16															// 0x14 some sort of enum [0 to 2]
			
			// real_point3d														// 0x20

			// data related to above function (0x1C in size?)					// 0x54
			// int32 															// 0x84 real time ?
			// real																// 0x88 pitch range related ?
			// int16															// 0x8C index to directsoundbuffer (related, set at first iteration)
			// int16															// 0x8E pitch range block index
			// int16															// 0x90 permutation block index
			// int16															// 0x92
			// int16															// 0x94 track block index (when parent == looping sound)
			// datum_index														// 0x98
			// real																// 0x9C
			// real																// 0xA0
			// bool																// 0xAC determined by sound class
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_datum) == 0xB0 );

		
		struct s_looping_sound_datum : TStructImpl(0xE4)
		{
			// datum_index definition_index;					// 0x4
			// 													// 0x8

			// datum_index object_index;						// 0x10

			// int16 node_index;								// 0x1A
			// real_vector3d relative_position;					// 0x1C
			// real_point3d relative_forward;					// 0x28

			// bool												// 0x4E "not a loop" flag related (set on sound completion) ?
			// int16											// 0x50 number of child sounds ?
			// int16											// 0x52
			// int32 detail_sound_related_?[32]					// 0x54 period bounds ticks ?
			// datum_index track_related_?[4]					// 0xD4

		}; BOOST_STATIC_ASSERT( sizeof(s_looping_sound_datum) == 0xE4 );

		struct s_object_looping_sound_datum : TStructImpl(0x34)
		{
			// int16															// 0x2 initialized to 2 ?
			// long_flags flags;												// 0x4
			// real																// 0x8 ? set when created as child ?
			// int32															// 0xC
			// datum_index object_index;										// 0x10
			// int32															// 0x14 initialized to NONE ?
			// int16 primary_scale_functon;										// 0x18
			// int16 node_index;												// 0x1A
			// real_vector3d relative_position;									// 0x1C
			// real_point3d relative_forward;									// 0x28
		} BOOST_STATIC_ASSERT( sizeof(s_looping_sound_datum) == 0x34 );

	};
};