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
	HS_FUNCTION_WITH_PARAMS(runtime_integer_operation, long, "", 
			"<value-index> <operation-name> <operation-value>", 3,
		HS_TYPE(short),
		HS_TYPE(string),
		HS_TYPE(long)
	);

	HS_FUNCTION(runtime_vectors_reset, void, "");
	HS_FUNCTION_WITH_PARAMS(runtime_vector_get_element, real, "", 
			"<value-index> <element-index>", 2,
		HS_TYPE(short),
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_vector_set_element, bool, "", 
			"<value-index> <element-index> <operation-name> <operation-value>", 4,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(string),
		HS_TYPE(real)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_vector_set, bool, "", 
			"<value-index> <operation-name> <value-x> <value-y> <value-z>", 5,
		HS_TYPE(short),
		HS_TYPE(string),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_vector_operation, bool, "", 
			"<value-index> <operation-name> <operation-vector-index>", 3,
		HS_TYPE(short),
		HS_TYPE(string),
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(runtime_vector_to_string, string, "", 
			"<value-index>", 1,
		HS_TYPE(short),
	);