/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

	HS_FUNCTION_WITH_PARAMS(volume_test_player_team, bool, "returns true if any players of the specified team are within the specified volume.", "<volume> <team-index>", 2,
		HS_TYPE(trigger_volume),
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(volume_test_player_team_all, bool, "returns true if all players of the specified team are within the specified volume.", "<volume> <team-index>", 2,
		HS_TYPE(trigger_volume),
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(player_team_teleport, void, "moves the specified team to the specified flag.", "<team-index> <cutscene-flag>", 2,
		HS_TYPE(short),
		HS_TYPE(cutscene_flag)
	);
	HS_FUNCTION_WITH_PARAMS(player_team_players, object_list, "returns a list of players on the specified team", "<team-index>", 1,
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(player_data_get_integer, long, "", 
			"<player-list-index> <data-name>", 2,
		HS_TYPE(short),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(player_team_data_get_integer, long, "", 
			"<team-index> <data-name>", 2,
		HS_TYPE(short),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(player_data_get_object, object, "", 
			"<player-list-index> <data-name>", 2,
		HS_TYPE(short),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(player_data_get_real, real, "", 
			"<player-list-index> <data-name>", 2,
		HS_TYPE(short),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(player_data_set_real, void, "", 
			"<player-list-index> <data-name> <data-value>", 3,
		HS_TYPE(short),
		HS_TYPE(string),
		HS_TYPE(real)
	);
	HS_FUNCTION(player_local_get, short, "");