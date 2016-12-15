/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
	};

	struct s_file_reference {
		tag			signature;
		word_flags	flags;
		_enum		location;
		long_string	file_name;
		HANDLE		handle;
		HRESULT		api_result;
	}; BOOST_STATIC_ASSERT( sizeof(s_file_reference) == 0x110 );
};