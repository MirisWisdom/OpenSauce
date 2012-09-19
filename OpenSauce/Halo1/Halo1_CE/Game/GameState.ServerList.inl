/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static cstring g_forced_mp_version = NULL;

static BOOL PLATFORM_API ServerVersionIsValid(cstring server_version)
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
	return Networking::GameBuildNumberIsValid(server_version);
}

void ServerListInitialize()
{
	if(CMDLINE_GET_PARAM(mp_version).ParameterSet())
	{
		cstring mp_version = CMDLINE_GET_PARAM(mp_version).GetValue();
		if(mp_version && (strcmp(mp_version, "disable") == 0))
			return;

		g_forced_mp_version = Networking::GameVersionToBuildNumberString(mp_version);
	}

	// override the function call that omits servers of a different version from the server browser
	Memory::WriteRelativeCall(ServerVersionIsValid, GET_FUNC_VPTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL), true);
}