/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

map_list_data_t* MultiplayerMaps()	PTR_IMP_GET2(multiplayer_maps);

static void MapListInitializeYelo()
{
	WIN32_FIND_DATAA fd;
	// TODO: need an implementation which also checks the user profile's maps folder
	HANDLE h = FindFirstFileA("maps\\*.yelo", &fd);
	if (h != INVALID_HANDLE_VALUE)
	{
		do Engine::Cache::MapListAddMap(fd.cFileName, Cache::K_MAP_FILE_EXTENSION_YELO);
		while (FindNextFileA(h, &fd));

		FindClose(h);
	}
}

static void MapListInitialize()
{
	static const uintptr_t INITIALIZE = GET_FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE);

	__asm	call	INITIALIZE
	MapListInitializeYelo();
}

API_FUNC_NAKED void MapListReIntialize()
{
	static const uintptr_t DISPOSE = GET_FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE);

	__asm {
		call	DISPOSE
		call	MapListInitialize
		retn
	}
}

static void MapListInitializeHooks()
{
	Memory::WriteRelativeCall(Interface::MapListInitialize,
		GET_FUNC_VPTR(MULTIPLAYER_MAP_LIST_INITIALIZE_CALL));

	*GET_PTR2(MAP_LIST_ADD_SKIP_CRC_JZ) = Enums::_x86_opcode_jmp_short;
}