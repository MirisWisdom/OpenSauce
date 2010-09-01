/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	HS_FUNCTION(player_local_get, short, "");