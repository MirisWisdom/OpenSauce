/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/


static void* scripting_volume_test_player_team_evaluate(void** arguments)
{
	struct s_arguments {
		int16 trigger_volume;
		PAD16;
		int16 team_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.boolean = false;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(*player->GetTeamIndex() == args->team_index && 
				Engine::Scenario::TriggerVolumeTestObject(args->trigger_volume, *player->GetSlaveUnitIndex()))
			{
				result.boolean = true;
				break;
			}
		}
	}

	return result.pointer;
}

static void* scripting_volume_test_player_team_all_evaluate(void** arguments)
{
	struct s_arguments {
		int16 trigger_volume;
		PAD16;
		int16 team_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.boolean = true;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(*player->GetTeamIndex() == args->team_index && 
				!Engine::Scenario::TriggerVolumeTestObject(args->trigger_volume, *player->GetSlaveUnitIndex()))
			{
				result.boolean = false;
				break;
			}
		}
	}
	else result.boolean = false;

	return result.pointer;
}

static void* scripting_player_team_teleport_evaluate(void** arguments)
{
	struct s_arguments {
		int16 team_index;
		PAD16;
		int16 cutscene_flag;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(*player->GetTeamIndex() == args->team_index)
			{
				//Engine::Objects::OrientToCutsceneFlag(*player->GetSlaveUnitIndex(), args->cutscene_flag);

				TagGroups::scenario* scnr = GameState::Scenario();
				Engine::Players::Teleport(iter.Current(), scnr->cutscene_flags[args->cutscene_flag].position);
			}
		}
	}

	return NULL;
}

static void* scripting_player_team_players_evaluate(void** arguments)
{
	struct s_arguments {
		int16 team_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.datum = datum_index::null;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		// An object list will only be created if there is 1 or more players on the specified team
		datum_index object_list = datum_index::null;

		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(*player->GetTeamIndex() == args->team_index)
			{
				if(object_list.IsNull())
					object_list = Scripting::ObjectListNew();

				Engine::HS::ObjectListAdd(object_list, *player->GetSlaveUnitIndex());
			}
		}

		// list will either be null or a handle to a object-list on return
		result.datum = object_list;
	}

	return result.pointer;
}


static int32 scripting_player_data_get_integer_by_name(s_player_datum* player, cstring data_name, bool for_team_data = false)
{
	cstring s = data_name; // alias for keeping the code width down

		 if( !strcmp(s,"kills_this_lifetime") )	return *player->GetKillsThisLifetime();

	else if( !strcmp(s,"friendly_fire_kills") )	return *player->GetFriendlyFireKills();
	else if( !strcmp(s,"deaths") )				return *player->GetDeaths();
	else if( !strcmp(s,"suicides") )			return *player->GetSuicides();

	//else if( !strcmp(s,"") )	return *player-;

	else if( !for_team_data )
	{
			 if( !strcmp(s,"team_index") )			return *player->GetTeamIndex();
		else if( !strcmp(s,"current_spree_count") )	return *player->GetCurrentSpreeCount();
		else if( !strcmp(s,"last_kill_time") )		return *player->GetLastKillTime();
		else if( !strcmp(s,"ping") )				return *player->GetPing();
		//else if( !strcmp(s,"") )	return *player-;
	}

	return NONE;
}
static void* scripting_player_data_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		int16 player_list_index;
		PAD16;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.int32 = NONE;

	if(args->player_list_index >= 0)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(player->GetNetworkPlayer()->player_list_index == args->player_list_index)
			{
				result.int32 = scripting_player_data_get_integer_by_name(player, args->data_name);
				break;
			}
		}
	}

	return result.pointer;
}
static void* scripting_player_team_data_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		int16 team_index;
		PAD16;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.int32 = NONE;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(*player->GetTeamIndex() == args->team_index)
				result.int32 += scripting_player_data_get_integer_by_name(player, args->data_name, true);
		}
	}

	return result.pointer;
}

static datum_index scripting_player_data_get_object_by_name(s_player_datum* player, cstring data_name)
{
	datum_index object_index = *player->GetSlaveUnitIndex();
	
	if(!object_index.IsNull()) // only get object data fields whenever this player is alive
	{
		cstring s = data_name; // alias for keeping the code width down

			 if( !strcmp(s,"nearest_object") )	return player->GetNearestObjectActionResult()->action_object_index;
		else if( !strcmp(s,"slave_unit") )		return *player->GetSlaveUnitIndex();
		else if( !strcmp(s,"last_slave_unit") )	return *player->GetLastSlaveUnitIndex();
		else if( !strcmp(s,"target_player") )
		{
			datum_index target_player = *player->GetTargetPlayerIndex();
			if(!target_player.IsNull())
				return *(*Players::Players())[target_player]->GetSlaveUnitIndex();
		}
		else if( _HS_UTIL_STRNCMP(s,"weapon") )
		{
			Objects::s_unit_datum* unit = (*Objects::ObjectHeader())[object_index]->_unit;

				 if( !strcmp(s,"weapon0") ) return unit->unit.GetWeaponObjectIndices()[0];
			else if( !strcmp(s,"weapon1") ) return unit->unit.GetWeaponObjectIndices()[1];
			else if( !strcmp(s,"weapon2") ) return unit->unit.GetWeaponObjectIndices()[2];
			else if( !strcmp(s,"weapon3") ) return unit->unit.GetWeaponObjectIndices()[3];
		}
	}
	
	return datum_index::null;
}
static void* scripting_player_data_get_object_evaluate(void** arguments)
{
	struct s_arguments {
		int16 player_list_index;
		PAD16;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.datum = datum_index::null;

	if(args->player_list_index >= 0)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(player->GetNetworkPlayer()->player_list_index == args->player_list_index)
			{
				result.datum = scripting_player_data_get_object_by_name(player, args->data_name);
				break;
			}
		}
	}

	return result.pointer;
}

static real* scripting_player_data_get_real_by_name(s_player_datum* player, cstring data_name, Enums::hs_type& out_type)
{
	cstring s = data_name; // alias for keeping the code width down

	out_type = HS_TYPE(real);
		 if( !strcmp(s,"speed") )	return player->GetSpeed();
	//else if( !strcmp(s,"") )		return player->GetSpeed();

	return NULL;
}
static void* scripting_player_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		int16 player_list_index;
		PAD16;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;
	result.real = -1.0f;

	if(args->player_list_index >= 0)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(player->GetNetworkPlayer()->player_list_index == args->player_list_index)
			{
				Enums::hs_type result_type;
				result.ptr.real = scripting_player_data_get_real_by_name(player, args->data_name, result_type);
				Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
				break;
			}
		}
	}

	return result.pointer;
}
static void* scripting_player_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		int16 player_list_index;
		PAD16;
		cstring data_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(args->player_list_index >= 0)
	{
		t_players_data::Iterator iter(Players::Players());
		s_player_datum* player;
		while( (player = iter.Next()) != NULL )
		{
			if(player->GetNetworkPlayer()->player_list_index == args->player_list_index)
			{
				TypeHolder result;
				Enums::hs_type result_type;
				result.ptr.real = scripting_player_data_get_real_by_name(player, args->data_name, result_type);
				Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
				break;
			}
		}
	}

	return NULL;
}




static void* scripting_player_local_get_evaluate()
{
	TypeHolder result; result.pointer = NULL;
	result.int32 = NONE;

	t_players_data::Iterator iter(Players::Players());
	s_player_datum* player;
	while( (player = iter.Next()) != NULL )
	{
		if(*player->GetLocalIndex() != NONE)
		{
			result.int32 = player->GetNetworkPlayer()->player_list_index;
			break;
		}
	}

	return result.pointer;
}