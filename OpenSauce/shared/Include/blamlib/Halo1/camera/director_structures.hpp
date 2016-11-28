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
		enum game_perspective : _enum
		{
			// Incomplete
			_game_perspective_first_person,
			_game_perspective_following_camera,

			_game_perspective
		};
	};

	namespace Camera
	{
		struct s_director_data
		{
			PAD16; // unknown field						0x0
			PAD16;									//	0x2
			PAD32; // unknown field						0x4
			void* camera_update_proc;				//	0x8
			PAD(1, 64);								//	0xC
			PAD32; // unknown field						0x4C
			PAD8; // unknown field						0x50
			bool inhibit_facing;					//	0x51
			bool inhibit_input;						//	0x52
			PAD8;									//	0x53
			int16 game_perspective;					//	0x54
			int16  perspective;						//	0x56
			s_camera_command command;				//	0x58
			PAD8; // unknown field						0xC0
			PAD24; 									//	0xC1
			PAD32; // unknown field						0xC4
			PAD(2, sizeof(real_vector3d) * 4);		//	0xC8
		}; BOOST_STATIC_ASSERT(sizeof(s_director_data) == 0xF8);

		s_director_data*				GlobalDirector();
	};
};