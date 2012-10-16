/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

/*!
 * \brief
 * Determines the unit definition override for a player in Free For All games
 */
static datum_index PlayerFindUnitDefinitionOverride(const s_player_datum* player, datum_index unit_definition_index,
	const TAG_TBLOCK(& player_units, TagGroups::s_network_game_player_unit))
{
	// NOTE: currently don't override FFA players
	return unit_definition_index;
}
/*!
 * \brief
 * Determines the unit definition override for a player in Team-based games
 */
static datum_index PlayerFindTeamUnitDefinitionOverride(const s_player_datum* player, datum_index unit_definition_index,
	const TAG_TBLOCK(& player_units, TagGroups::s_network_game_player_unit))
{
	// HACK: not the best way of doing this (hate hard coding stuff), but for now it works
	static cstring k_team_names[Enums::k_number_of_multiplayer_teams] = { 
		"red_team", "blue_team" };

	int32 player_team_index = player->team_index;
	if(player_team_index >= 0 && player_team_index < NUMBEROF(k_team_names))
	{
		cstring player_team_name = k_team_names[player_team_index];

		for(int32 x = 0; x < player_units.Count; x++)
		{
			const TagGroups::s_network_game_player_unit& game_player_unit = player_units[x];

			// Null definitions are removed by CheApe extensions, so tag_index should never be null (unless modified by non-OS tools)
			if( !strcmp(game_player_unit.name, player_team_name) )
				unit_definition_index = game_player_unit.definition.tag_index;
		}
	}

	return unit_definition_index;
}

/*!
 * \brief
 * Determines the unit definition override to use for a player based on their state and if any overrides exist in the PY globals definition
 * 
 * \param player_index
 * Player's index
 * 
 * \param unit_definition_index
 * Default unit definition for multiplayer games
 * 
 * \returns
 * The override unit definition, based on the player's state, or the default definition
 */
static datum_index PlayerSpawnCreateUnitMultiplayerGetDefinitionOverride(datum_index player_index, datum_index unit_definition_index)
{
	const TAG_TBLOCK(& player_units, TagGroups::s_network_game_player_unit) = TagGroups::_global_yelo_globals->networking.player_units;
	if(player_units.Count == 0) return unit_definition_index;

	const s_player_datum* player = (*Players::Players())[player_index];

	unit_definition_index = GameEngine::GlobalVariant()->universal_variant.teams ?
		PlayerFindTeamUnitDefinitionOverride(player, unit_definition_index, player_units) :
		PlayerFindUnitDefinitionOverride(player, unit_definition_index, player_units);

	return unit_definition_index;
}

static API_FUNC_NAKED void PLATFORM_API PlayerSpawnCreateUnitMultiplayerHook()
{
	static uint32 RETN_ADDRESS = GET_FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN);

	__asm {
		push	eax		// unit_definition_index
		push	ebx		// player_index
		call	PlayerSpawnCreateUnitMultiplayerGetDefinitionOverride

		jmp		RETN_ADDRESS
	}
}