/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_set_thread_count, void, "sets the number of http threads the http server starts", "<short>", 1,
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_set_root, void, "sets the folder to serve files from", "<string>", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_set_throttle, void, "sets the maximum bandwidth the http server will use in bytes per sec, append k and m for kilo and mega bytes respectively", "<string>", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_set_ports, void, "sets the ports to listen on", "<string>", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION(sv_httpserver_show_config, void, "displays the current http server config options");

	HS_FUNCTION(sv_httpserver_start, void, "starts the http server");

	HS_FUNCTION(sv_httpserver_stop, void, "stop the http server");


	HS_FUNCTION_WITH_PARAMS(sv_httpserver_log_enable, void, "set whether to output http server events into the server log", "<bool>", 1,
		HS_TYPE(bool)
	);

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_set_connection_ban, void, "sets the variables that controls how many connections an IP can have over time", "<max_connections> <connection_cooloff> <forget_connection_time>", 3,
		HS_TYPE(long),
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION(sv_httpserver_banlist, void, "prints the current http server ip ban list");

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_banlist_file, void, "sets the http servers ban list suffix", "<string>", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_ban_ip, void, "adds an ip to the ban list, the version bool is false for ipv4 and true for ipv6", "<bool> <short> <short> <short> <short> <short> <short> <short> <short>", 9,
		HS_TYPE(bool),
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(sv_httpserver_unban_ip, void, "removes an ip from the ban list by its index", "<short>", 1,
		HS_TYPE(short)
	);