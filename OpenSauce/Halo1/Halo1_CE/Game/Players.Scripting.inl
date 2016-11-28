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
	TypeHolder result; result.pointer = nullptr;
	result.boolean = false;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		for(auto player : Players::Players())
		{
			if(player->team_index == args->team_index && 
				blam::scenario_trigger_volume_test_object(args->trigger_volume, player->slave_unit_index))
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
	TypeHolder result; result.pointer = nullptr;
	result.boolean = true;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		for(auto player : Players::Players())
		{
			if(player->team_index == args->team_index && 
				!blam::scenario_trigger_volume_test_object(args->trigger_volume, player->slave_unit_index))
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
		for(auto player : Players::Players())
		{
			if(player->team_index == args->team_index)
			{
				TagGroups::scenario* scnr = Scenario::Scenario();
				blam::player_teleport(player.index, datum_index::null, scnr->cutscene_flags[args->cutscene_flag].position);
			}
		}
	}

	return nullptr;
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

		for(auto player : Players::Players())
		{
			if(player->team_index == args->team_index)
			{
				if(object_list.IsNull())
					object_list = blam::object_list_new();

				blam::object_list_add(object_list, player->slave_unit_index);
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

		 if( !strcmp(s,"kills_this_lifetime") )	return player->kills_this_lifetime;

	else if( !strcmp(s,"friendly_fire_kills") )	return player->friendly_fire_kills;
	else if( !strcmp(s,"deaths") )				return player->deaths;
	else if( !strcmp(s,"suicides") )			return player->suicides;

	//else if( !strcmp(s,"") )	return *player-;

	else if( !for_team_data )
	{
			 if( !strcmp(s,"team_index") )			return player->team_index;
		else if( !strcmp(s,"current_spree_count") )	return player->current_spree_count;
		else if( !strcmp(s,"last_kill_time") )		return player->last_kill_time;
		else if( !strcmp(s,"ping") )				return player->ping;
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
	TypeHolder result; result.pointer = nullptr;
	result.int32 = NONE;

	if(args->player_list_index >= 0)
	{
		for(auto player : Players::Players())
		{
			if(player->network_player.player_list_index == args->player_list_index)
			{
				result.int32 = scripting_player_data_get_integer_by_name(player.datum, args->data_name);
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
	TypeHolder result; result.pointer = nullptr;
	result.int32 = NONE;

	if(args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
	{
		for(auto player : Players::Players())
		{
			if(player->team_index == args->team_index)
				result.int32 += scripting_player_data_get_integer_by_name(player.datum, args->data_name, true);
		}
	}

	return result.pointer;
}

static datum_index scripting_player_data_get_object_by_name(s_player_datum* player, cstring data_name)
{
	datum_index object_index = player->slave_unit_index;
	
	if(!object_index.IsNull()) // only get object data fields whenever this player is alive
	{
		cstring s = data_name; // alias for keeping the code width down

			 if( !strcmp(s,"nearest_object") )	return player->nearest_object_action_result.action_object_index;
		else if( !strcmp(s,"slave_unit") )		return player->slave_unit_index;
		else if( !strcmp(s,"last_slave_unit") )	return player->last_slave_unit_index;
		else if( !strcmp(s,"target_player") )
		{
			datum_index target_player = player->target_player_index;
			if(!target_player.IsNull())
				return Players::Players()[target_player]->slave_unit_index;
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
	TypeHolder result; result.pointer = nullptr;
	result.datum = datum_index::null;

	if(args->player_list_index >= 0)
	{
		for(auto player : Players::Players())
		{
			if(player->network_player.player_list_index == args->player_list_index)
			{
				result.datum = scripting_player_data_get_object_by_name(player.datum, args->data_name);
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
		 if( !strcmp(s,"speed") )	return &player->speed;
	//else if( !strcmp(s,"") )		return &player->;

	return nullptr;
}
static void* scripting_player_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		int16 player_list_index;
		PAD16;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.real = -1.0f;

	if(args->player_list_index >= 0)
	{
		for(auto player : Players::Players())
		{
			if(player->network_player.player_list_index == args->player_list_index)
			{
				Enums::hs_type result_type;
				result.ptr.real = scripting_player_data_get_real_by_name(player.datum, args->data_name, result_type);
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
		for(auto player : Players::Players())
		{
			if(player->network_player.player_list_index == args->player_list_index)
			{
				TypeHolder result;
				Enums::hs_type result_type;
				result.ptr.real = scripting_player_data_get_real_by_name(player.datum, args->data_name, result_type);
				Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
				break;
			}
		}
	}

	return nullptr;
}




static void* scripting_player_local_get_evaluate()
{
	TypeHolder result; result.pointer = nullptr;
	result.int32 = NONE;

	auto* local_player = Players::LocalPlayer();
	if(local_player != nullptr)
		result.int32 = local_player->network_player.player_list_index;

	return result.pointer;
}