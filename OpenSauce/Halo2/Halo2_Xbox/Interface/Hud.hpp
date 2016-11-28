/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum hud_position
		{
			_hud_position_top_left,
			_hud_position_top_right,
			_hud_position_bottom_left,
			_hud_position_bottom_right,
			_hud_position_center,
			_hud_position,
		};
	};

	namespace Hud
	{
	};

	namespace GameState
	{
		struct s_hud_scripted_globals
		{
			bool Active;
			bool ShowHelpText;
			bool ShowHudMessages;
			PAD8;
		};
		s_hud_scripted_globals* _HudScriptedGlobals();

		struct s_hud_messaging : TStructImpl(5032)
		{
			struct s_text_element
			{
				uint32 TimeDisplayed;
				wchar_t Text[63];
				bool Display;
				byte DisplayIndex;
				byte NextMessage;
				UNKNOWN_TYPE(byte);
				UNKNOWN_TYPE(int16);
			}; BOOST_STATIC_ASSERT( sizeof(s_text_element) == 0x88);

			struct s_player : TStructImpl(1248)
			{
				TStructGetPtrImpl(s_text_element, TextElements, 0x0); // [4]
				TStructGetPtrImpl(s_text_element, ItemMessage, 0450);
				TStructGetPtrImpl(real, ItemMessageUpTime, 0x4DC);
			};

			TStructGetPtrImpl(s_player, Players, 0x0);
			// Accounts for ALL messages for ALL players
			TStructGetPtrImpl(byte, MessageCount, 0x1385);
			// the game tick when this timer was set and started
			TStructGetPtrImpl(uint32, TimerStartTick, 0x1398);
			// amount of game ticks left in the timer
			TStructGetPtrImpl(short, TimerTicksLeft, 0x139C);
			TStructGetPtrImpl(short, TimerWarningTicks, 0x139E);
			TStructGetPtrImpl(point2d, TimerPosition, 0x13A0);
			TStructGetPtrImpl(_enum, TimerHudPosition, 0x13A4);
			TStructGetPtrImpl(bool, TimerPause, 0x13A6);
			TStructGetPtrImpl(bool, TimerShow, 0x13A7);
		};
		s_hud_messaging* _HudMessaging();

		struct s_hud_nav_points : TStructImpl(192)
		{
		};
		s_hud_nav_points* _HudNavPoints();

		struct s_weapon_hud_globals : TStructImpl(484)
		{
		};
		s_weapon_hud_globals* _WeaponHudGlobals();

#if PLATFORM_ID == PLATFORM_H2_ALPHA
		struct s_unit_hud_globals : TStructImpl(3016)
		{
		};
		s_unit_hud_globals* _UnitHudGlobals();

		struct s_first_person_weapon_globals : TStructImpl(255728)
		{
		};
		s_first_person_weapon_globals* _FpWeapons();
#else

		struct s_first_person_weapon_globals1 : TStructImpl(33584)
		{
		};
		s_first_person_weapon_globals1* _FpWeapons1();

		struct s_first_person_weapon_globals2 : TStructImpl(32768)
		{
		};
		s_first_person_weapon_globals2* _FpWeapons2();
#endif

		struct s_objective_globals
		{
			// 1 - show
			// 2 - finished
			long_flags UNKNOWN(0)[4];
			int32 UNKNOWN(1);
		};
		s_objective_globals* _ObjectiveGlobals();

		struct s_motion_sensor_globals : TStructImpl(3016)
		{
		};
		s_motion_sensor_globals* _MotionSensor();
	};
};