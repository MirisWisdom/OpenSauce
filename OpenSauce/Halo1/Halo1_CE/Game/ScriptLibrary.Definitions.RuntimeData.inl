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