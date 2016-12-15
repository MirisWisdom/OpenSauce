/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
	HS_FUNCTION(sv_mapdownload_start_server, void, "starts the map download server");
	HS_FUNCTION(sv_mapdownload_stop_server, void, "stops the map download server");

	HS_FUNCTION_WITH_PARAMS(sv_mapdownload_set_part_definitions_path, void, "sets the path containing the map part definition xmls", "<string>", 1,
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(sv_mapdownload_set_host, void, "sets the web address where the map parts are being hosted", "<string>", 1,
		HS_TYPE(string)
	);
	HS_FUNCTION(sv_mapdownload_reload_map_part_definitions, void, "re-loads the map part definitions from the specified folder");