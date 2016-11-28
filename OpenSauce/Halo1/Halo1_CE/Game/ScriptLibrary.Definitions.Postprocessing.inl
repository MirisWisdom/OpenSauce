/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

	HS_FUNCTION(pp_load, void, "loads post processing");
	HS_FUNCTION(pp_unload, void, "unloads post processing");

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_instance_index_by_name, short, "returns the index of the effect instance named <instance-name>", "<instance-name>",1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_instance_active, void, "instantly set an effect instance to on or off", "<instance-index> <instance-enabled>",2,
		HS_TYPE(short),
		HS_TYPE(bool)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_instance_fade, void, "fade an effect instance in or out", "<instance-index> <fade-start> <fade-end> <fade-time>",4,
		HS_TYPE(short),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_instance_current_fade, real, "returns current fade amount of an effect instance", "<instance-index>",1,
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_instance_fade_direction, short, "returns fade direction of an effect instance, -1 = no fade, 0 = fade out, 1 = fade in", "<instance-index>",1,
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_index_by_name, short, "returns the index of the effect named <effect-name>", "<effect-name>",1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_is_valid, bool, "returns whether an effect is valid and ready to use", "<effect-index>",1,
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_shader_variable_index_by_name, short, "gets the index of an exposed post processing effect variable named <name>", 
			"<effect-index> <variable-name>",2,
		HS_TYPE(short),
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_shader_variable_boolean, void, "sets the value of an exposed boolean variable in a post processing effect", 
			"<effect-index> <variable-index> <boolean> <real>",4,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(bool),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_shader_variable_integer, void, "sets the value of an exposed integer variable in a post processing effect", 
			"<effect-index> <variable-index> <long> <real>",4,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(long),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_shader_variable_real, void, "sets the value of an exposed float/vector/color variable in a post processing effect", 
			"<effect-index> <variable-index> <real> <real> <real> <real> <real>",7,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_shader_instance_active, void, "disables/enables a shader instance in an effect", 
			"<effect-index> <shader-instance-index> <boolean>",3,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(bool)
	);

	HS_FUNCTION_WITH_PARAMS(pp_bloom_set_size, void, "changes the bloom size over a period of time", 
			"<real> <real>",2,
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_bloom_set_exposure, void, "changes the bloom exposure over a period of time", 
			"<real> <real>",2,
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_bloom_set_mix_amount, void, "changes the bloom mix amount over a period of time", 
			"<real> <real>",2,
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_bloom_set_minimum_color, void, "changes the bloom minimum color over a period of time", 
			"<real> <real> <real> <real>",4,
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_bloom_set_maximum_color, void, "changes the bloom maximum color over a period of time", 
			"<real> <real> <real> <real>",4,
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);