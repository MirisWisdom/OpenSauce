/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
static cstring g_forced_mp_version = nullptr;

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

		// use the current version if the queried server_version is invalid as far as we know
		if (server_version == nullptr || !BuildNumber::StringIsValid(server_version))
			server_version = BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE);

		BuildNumber::ChangeAdvertisedVersion(server_version, false);
	}
}

static void SetVersionToCurrent()
{
	BuildNumber::ChangeAdvertisedVersion(BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE), false);
}

static void* CreateNetworkServerHook()
{
	typedef void* (PLATFORM_API* create_network_game_t)();
	static const create_network_game_t network_game_server_create =
		CAST_PTR(create_network_game_t, GET_FUNC_VPTR(GAME_CREATE_NETWORK_SERVER));

	SetVersionToCurrent();

	return network_game_server_create();
}

static void* CreateNetworkClientHook()
{
	typedef void* (PLATFORM_API* create_network_game_t)();
	static const create_network_game_t network_game_client_create = 
		CAST_PTR(create_network_game_t, GET_FUNC_VPTR(GAME_CREATE_NETWORK_CLIENT));

	SetVersionFromServer();

	return network_game_client_create();
}

static void ServerListInitialize()
{
	if(CMDLINE_GET_PARAM(mp_version).ParameterSet())
	{
		cstring mp_version = CMDLINE_GET_PARAM(mp_version).GetValue();
		if(mp_version && strcmp(mp_version, "disable")==0)
			return;

		g_forced_mp_version = BuildNumber::ShortStringToBuildNumberString(mp_version);
	}

	// override the function call that omits servers of a different version from the server browser
	Memory::WriteRelativeCall(ServerVersionIsValid, GET_FUNC_VPTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL), true);

	Memory::WriteRelativeCall(CreateNetworkServerHook, GET_FUNC_VPTR(GAME_CREATE_NETWORK_SERVER_CALL), true);
	for(auto ptr : K_GAME_CREATE_NETWORK_CLIENT_CALL)
		Memory::WriteRelativeCall(CreateNetworkClientHook, ptr, true);
}