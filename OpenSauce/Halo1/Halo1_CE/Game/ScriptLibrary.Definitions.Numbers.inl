/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

	HS_FUNCTION_WITH_PARAMS(abs_integer, long, "", 
			"<value>", 1,
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(abs_real, real, "", 
			"<value>", 1,
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(bitwise_and, long, "", 
			"<value> <flags>", 2,
		HS_TYPE(long),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(bitwise_or, long, "", 
			"<value> <flags>", 2,
		HS_TYPE(long),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(bitwise_xor, long, "", 
			"<value> <flags>", 2,
		HS_TYPE(long),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(bitwise_lhs, long, "", 
			"<value> <bit-count>", 2,
		HS_TYPE(long),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(bitwise_rhs, long, "", 
			"<value> <bit-count>", 2,
		HS_TYPE(long),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(bit_test, bool, "", 
			"<value> <bit-index>", 2,
		HS_TYPE(long),
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(bit_toggle, long, "", 
			"<value> <bit-index> <on-or-off>", 3,
		HS_TYPE(long),
		HS_TYPE(short),
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(bit_flags_test, bool, "", 
			"<value> <flags>", 2,
		HS_TYPE(long),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(bit_flags_toggle, long, "", 
			"<value> <flags> <add-or-remove>", 3,
		HS_TYPE(long),
		HS_TYPE(long),
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(hex_string_to_long, long, "", 
			"<value>", 1,
		HS_TYPE(string)
	);