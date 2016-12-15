/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Scripting
	{
		typedef void* (API_FUNC* hs_yelo_function_proc)();
		typedef void* (API_FUNC* hs_yelo_function_with_params_proc)(void** arguments);
	};
};

#define HS_TYPE(hstype) BOOST_JOIN(Yelo::Enums::_hs_type_,hstype)