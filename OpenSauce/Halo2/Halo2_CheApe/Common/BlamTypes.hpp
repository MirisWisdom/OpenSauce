/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			// character count in a [long_string] type
			k_long_string_length = 255,

			k_string_id_length = 127,
		};
	};


#pragma region String types / utils

	// 255 character ASCII string, no null terminator
	typedef char long_string[Yelo::Enums::k_long_string_length+1];
#define pad_long_string PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128

	typedef char string_id_value[Yelo::Enums::k_string_id_length+1];

	typedef uint32 string_id;
#define pad_string_id PAD32

#pragma endregion

};