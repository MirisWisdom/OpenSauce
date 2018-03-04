/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Players
	{
		struct s_player_datum;
	};

	namespace Enums
	{
		enum hud_chat_type : long_enum;
		enum settings_adjustment_result : long_enum;

		enum rasterize_score : long_enum
		{
			_rasterize_score_welcome,
			_rasterize_score_s_died,
			_rasterize_score_s_was_killed_by_guardians,
			_rasterize_score_s_was_killed_by_vehicle,
			_rasterize_score_s_was_killed_by_s,
			_rasterize_score_s_was_betrayed_bs_s,
			_rasterize_score_s_commited_suicide,
			_rasterize_score_double_kill,
			_rasterize_score_you_killed_s,
			_rasterize_score_triple_kill,
			_rasterize_score_killtacular,
			_rasterize_score_killing_spree,
			_rasterize_score_running_riot,
			_rasterize_score_you_betrayed_s,
			_rasterize_score_killtacular_d,
			_rasterize_score_triple_kill_d,
			_rasterize_score_double_kill_d,
			_rasterize_score_running_riot_d,
			_rasterize_score_started_killing_spree,
			_rasterize_score_you_killed_s_d,
			_rasterize_score_you_are_odd_man_out,
			_rasterize_score_you_are_out_of_lives,
			_rasterize_score_rejoin_in_d,
			_rasterize_score_waiting_for_space,
			_rasterize_score_you_quit,
			_rasterize_score_s_quit,
			_rasterize_score_hold_s_for_score,
			_rasterize_score_d_remaining,
			_rasterize_score_telefragged,
			_rasterize_score,
		};
	};

	namespace GameUI
	{
		void Initialize();
		void Dispose();
		void PLATFORM_API Update();

		void PLATFORM_API FirstPersonWeaponsUpdate();


		// Predicate to use for determining when to send a message to players
		typedef bool (API_FUNC* proc_send_hud_chat_predicate)(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index);
		// Predicate for always sending a message to 
		bool SendHudChatToEveryonePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index);

		// Send [messsage] to all valid players (machine_index != NONE) and who pass the [send_predicate]
		void SendHudChat(Enums::hud_chat_type msg_type, wcstring message, byte player_number,
			proc_send_hud_chat_predicate send_predicate = SendHudChatToEveryonePredicate);
	};
};
#endif