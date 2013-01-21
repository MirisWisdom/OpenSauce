/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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