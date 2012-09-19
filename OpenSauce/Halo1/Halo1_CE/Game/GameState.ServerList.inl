/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static const char* g_forced_mp_version = NULL;

static BOOL PLATFORM_API ServerVersionIsValid(const char* server_version)
{
	if(g_forced_mp_version)
	{
		// compare with the forced server version
		if(strcmp(g_forced_mp_version, server_version) == 0)
			return true;
		return false;
	}

	// compare with the games version
	if(strcmp(GameBuildString(), server_version) == 0)
		return true;

	// compare all known official versions
	for(int i = 0; i < NUMBEROF(Networking::g_game_build_numbers); i++)
		if(strcmp(Networking::g_game_build_numbers[i], server_version) == 0)
			return true;
	return false;
}

void ServerListInitialize()
{
	if(CMDLINE_GET_PARAM(mp_version).ParameterSet())
	{
		char* mp_version = CMDLINE_GET_PARAM(mp_version).GetValue();
		if(mp_version && (strcmp(mp_version, "disable") == 0))
			return;

		if(strcmp(mp_version, "1.00") == 0)
			g_forced_mp_version = Networking::g_game_build_numbers[Enums::_game_build_number_index_100];
		else if(strcmp(mp_version, "1.07") == 0)
			g_forced_mp_version = Networking::g_game_build_numbers[Enums::_game_build_number_index_107];
		else if(strcmp(mp_version, "1.08") == 0)
			g_forced_mp_version = Networking::g_game_build_numbers[Enums::_game_build_number_index_108];
		else if(strcmp(mp_version, "1.09") == 0)
			g_forced_mp_version = Networking::g_game_build_numbers[Enums::_game_build_number_index_109];
		else
			g_forced_mp_version = NULL;
	}

	// override the function call that omits servers of a different version from the server browser
	Memory::WriteRelativeCall(ServerVersionIsValid, GET_FUNC_VPTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL), true);
}