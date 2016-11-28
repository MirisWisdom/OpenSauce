/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

	HS_FUNCTION_WITH_PARAMS(objects_distance_to_object, real, "", 
			"<objects> <destination-object>", 2,
		HS_TYPE(object_list),
		HS_TYPE(object)
	);

	HS_FUNCTION_WITH_PARAMS(object_data_get_real, real, "", 
			"<object> <data-name> <subdata-name>", 3,
		HS_TYPE(object),
		HS_TYPE(string),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(object_data_set_real, void, "", 
			"<object> <data-name> <subdata-name> <data-value>", 4,
		HS_TYPE(object),
		HS_TYPE(string),
		HS_TYPE(string),
		HS_TYPE(real)
	);
	HS_FUNCTION_WITH_PARAMS(object_data_set_vector, bool, "", 
			"<object> <data-name> <vector-index>", 3,
		HS_TYPE(object),
		HS_TYPE(string),
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(object_data_save_vector, bool, "", 
			"<object> <data-name> <vector-index>", 3,
		HS_TYPE(object),
		HS_TYPE(string),
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(weapon_data_get_real, real, "", 
			"<weapon> <data-name>", 2,
		HS_TYPE(object),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(weapon_data_set_real, void, "", 
			"<weapon> <data-name> <data-value>", 3,
		HS_TYPE(object),
		HS_TYPE(string),
		HS_TYPE(real)
	);
	HS_FUNCTION_WITH_PARAMS(weapon_data_magazine_get_integer, long, "", 
			"<weapon> <magazine-index> <data-name> <subdata-name>", 4,
		HS_TYPE(object),
		HS_TYPE(long),
		HS_TYPE(string),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(weapon_data_magazine_set_integer, void, "", 
			"<weapon> <magazine-index> <data-name> <subdata-name> <data-value>", 5,
		HS_TYPE(object),
		HS_TYPE(long),
		HS_TYPE(string),
		HS_TYPE(string),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(weapon_data_trigger_set_real, void, "", 
			"<weapon> <trigger-index> <data-name> <subdata-name> <data-value>", 5,
		HS_TYPE(object),
		HS_TYPE(long),
		HS_TYPE(string),
		HS_TYPE(string),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(unit_data_get_object, object, "", 
			"<unit> <data-name>", 2,
		HS_TYPE(unit),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(unit_data_get_integer, long, "", 
			"<unit> <data-name>", 2,
		HS_TYPE(unit),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(unit_data_set_integer, void, "", 
			"<unit> <data-name> <data-value>", 3,
		HS_TYPE(unit),
		HS_TYPE(string),
		HS_TYPE(long)
	);
	HS_FUNCTION_WITH_PARAMS(unit_data_get_real, real, "", 
			"<unit> <data-name>", 2,
		HS_TYPE(unit),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(unit_data_set_real, void, "", 
			"<unit> <data-name> <data-value>", 3,
		HS_TYPE(unit),
		HS_TYPE(string),
		HS_TYPE(real)
	);