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

	HS_FUNCTION(pp_load, void, "loads post processing");
	HS_FUNCTION(pp_unload, void, "unloads post processing");
	HS_FUNCTION_WITH_PARAMS(pp_set_effect_active, void, "instantly set an effect to on or off", "<effect-index> <effect-enabled>",2,
		HS_TYPE(short),
		HS_TYPE(bool)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_fade, void, "fade an effect in or out", "<effect-index> <fade-start> <fade-end> <fade-time>",4,
		HS_TYPE(short),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_index_by_name, short, "returns the index of the effect named <effect-name>", "<effect-name>",1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_current_fade, real, "returns current fade amount of an effect", "<effect-index>",1,
		HS_TYPE(short)
	);

	HS_FUNCTION_WITH_PARAMS(pp_get_effect_fade_direction, short, "returns fade direction of an effect, -1 = no fade, 0 = fade out, 1 = fade in", "<effect-index>",1,
		HS_TYPE(short)
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
			"<effect-index> <variable-index> <boolean>",3,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(bool)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_shader_variable_integer, void, "sets the value of an exposed integer variable in a post processing effect", 
			"<effect-index> <variable-index> <long>",3,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(long)
	);

	HS_FUNCTION_WITH_PARAMS(pp_set_effect_shader_variable_real, void, "sets the value of an exposed float/vector/color variable in a post processing effect", 
			"<effect-index> <variable-index> <real> <real> <real> <real>",6,
		HS_TYPE(short),
		HS_TYPE(short),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real),
		HS_TYPE(real)
	);