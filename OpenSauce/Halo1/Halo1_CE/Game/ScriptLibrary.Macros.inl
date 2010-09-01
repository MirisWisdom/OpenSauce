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
//////////////////////////////////////////////////////////////////////////
// hs_function
// [name]	Code based name
// [ret]	hs_type this function returns
// [info]	(string) Function's "help" text, short little description
#define HS_FUNCTION(name, ret, info)											\
	Yelo::Scripting::hs_function_definition function_##name##_definition = {	\
		HS_TYPE(ret),0,															\
		#name,																	\
		CAST_PTR(Yelo::Scripting::hs_parse_proc,Yelo::Scripting::GET_FUNC_PTR(HS_MACRO_FUNCTION_PARSE)), \
		NULL,																	\
		info,																	\
		NULL,																	\
		Yelo::Flags::_hs_access_flag_enabled, 0									\
	}

// [name]	Code based name
// [ret]	[hs_type] this function returns
// [info]	(string) Function's "help" text, short little description
// [param]	(string) Function's "parameters" text, lists the parameter names
// [argc]	How many parameters this function takes
// [...]	List of parameter's [hs_type] types
#define HS_FUNCTION_WITH_PARAMS(name, ret, info, param, paramc, ...)			\
	Yelo::Scripting::hs_function_definition function_##name##_definition = {	\
		HS_TYPE(ret),0,															\
		#name,																	\
		CAST_PTR(Yelo::Scripting::hs_parse_proc,Yelo::Scripting::GET_FUNC_PTR(HS_MACRO_FUNCTION_PARSE)), \
		NULL,																	\
		info,																	\
		param,																	\
		Yelo::Flags::_hs_access_flag_enabled,									\
		paramc, {__VA_ARGS__}													\
	}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// hs_external_global
#define HS_GLOBAL(name, type, value)										\
	Yelo::Scripting::hs_global_definition global_##name##_definition = {	\
		#name,																\
		HS_TYPE(type),														\
		0,																	\
		CAST_PTR(void*,value),												\
		Yelo::Flags::_hs_access_flag_enabled								\
	}

#define HS_GLOBAL_EX(name, type, value, flags)								\
	Yelo::Scripting::hs_global_definition global_##name##_definition = {	\
		#name,																\
		HS_TYPE(type),														\
		flags,																\
		CAST_PTR(void*,value),												\
		Yelo::Flags::_hs_access_flag_enabled								\
	}


#if PLATFORM_IS_DEDI
	#define HS_GLOBAL2(name, type, valuece, valueded) \
		Yelo::Scripting::hs_global_definition global_##name##_definition = {#name, HS_TYPE(type), 0, CAST_PTR(void*,valueded), Yelo::Flags::_hs_access_flag_enabled}
#elif PLATFORM_IS_USER
	#define HS_GLOBAL2(name, type, valuece, valueded) \
		Yelo::Scripting::hs_global_definition global_##name##_definition = {#name, HS_TYPE(type), 0, CAST_PTR(void*,valuece), Yelo::Flags::_hs_access_flag_enabled}
#endif
//////////////////////////////////////////////////////////////////////////