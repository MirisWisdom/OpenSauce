/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static const char* g_forced_mp_version = NULL;

static const char* g_valid_server_versions[] =
{
	"01.00.00.0609",
	"01.00.07.0613",
	"01.00.08.0616",
	"01.00.09.0620",
};

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
	for(int i = 0; i < NUMBEROF(g_valid_server_versions); i++)
		if(strcmp(g_valid_server_versions[i], server_version) == 0)
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
			g_forced_mp_version = g_valid_server_versions[0];
		else if(strcmp(mp_version, "1.07") == 0)
			g_forced_mp_version = g_valid_server_versions[1];
		else if(strcmp(mp_version, "1.08") == 0)
			g_forced_mp_version = g_valid_server_versions[2];
		else if(strcmp(mp_version, "1.09") == 0)
			g_forced_mp_version = g_valid_server_versions[3];
		else
			g_forced_mp_version = NULL;
	}

	// override the function call that omits servers of a different version from the server browser
	Memory::WriteRelativeCall(ServerVersionIsValid, GET_FUNC_VPTR(GAME_SERVER_VERSION_TEST_CALL), true);
}