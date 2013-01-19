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
	if(strcmp(BuildNumber::GameBuildString(), server_version) == 0)
		return true;

	// compare all known official versions
	return BuildNumber::StringIsValid(server_version);
}

static void SetVersionFromServer()
{
	Networking::s_gamespy_server* server = Networking::GsServerBrowserGlobals()->selected_server.server;
	if(server)
	{
		const char* server_version = Networking::GameSpy::SBServerGetStringValue(server, "gamever", "");

		if(server_version && BuildNumber::StringIsValid(server_version))
			BuildNumber::ChangeAdvertisedVersion(server_version, false);
		else
			BuildNumber::ChangeAdvertisedVersion(BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE), false);
	}
}

static void SetVersionToCurrent()
{
	BuildNumber::ChangeAdvertisedVersion(BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE), false);
}

static void* CreateNetworkServerHook()
{
	typedef void* (*create_network_game_t)();
	create_network_game_t function = CAST_PTR(create_network_game_t, GET_FUNC_VPTR(GAME_CREATE_NETWORK_SERVER));

	SetVersionToCurrent();

	return (*function)();
}

static void* CreateNetworkClientHook()
{
	typedef void* (*create_network_game_t)();
	create_network_game_t function = CAST_PTR(create_network_game_t, GET_FUNC_VPTR(GAME_CREATE_NETWORK_CLIENT));

	SetVersionFromServer();

	return (*function)();
}

void ServerListInitialize()
{
	if(CMDLINE_GET_PARAM(mp_version).ParameterSet())
	{
		cstring mp_version = CMDLINE_GET_PARAM(mp_version).GetValue();
		if(mp_version && (strcmp(mp_version, "disable") == 0))
			return;

		g_forced_mp_version = BuildNumber::VersionToBuildNumberString(mp_version);
	}

	// override the function call that omits servers of a different version from the server browser
	Memory::WriteRelativeCall(ServerVersionIsValid, GET_FUNC_VPTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL), true);

	Memory::WriteRelativeCall(CreateNetworkServerHook, GET_FUNC_VPTR(GAME_CREATE_NETWORK_SERVER_CALL), true);
	for(int i = 0; i < NUMBEROF(K_GAME_CREATE_NETWORK_CLIENT_CALL); i++)
		Memory::WriteRelativeCall(CreateNetworkClientHook, K_GAME_CREATE_NETWORK_CLIENT_CALL[i], true);
}