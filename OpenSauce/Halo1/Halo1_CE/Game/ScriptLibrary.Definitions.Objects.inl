/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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