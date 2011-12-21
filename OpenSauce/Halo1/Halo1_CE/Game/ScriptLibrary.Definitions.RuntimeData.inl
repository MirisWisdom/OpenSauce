/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

	HS_FUNCTION(runtime_integers_reset, void, "");
	HS_FUNCTION_WITH_PARAMS(runtime_integer_get, long, "", 
			"<value-index>", 1,
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_integer_set, long, "", 
			"<value-index> <value>", 2,
		HS_TYPE(short),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_integer_inc, long, "", 
			"<value-index>", 1,
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_integer_dec, long, "", 
			"<value-index>", 1,
		HS_TYPE(short)
	);