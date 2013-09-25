/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

namespace
{
	using namespace Yelo;
	using namespace Yelo::Enums;

	static byte_swap_code_t k_string_byte_swap_codes[] =
		{ sizeof(tag_string), 0 };

	static byte_swap_code_t k_char_integer_byte_swap_codes[] =
		{ _bs_code_1byte, 0 };
	static byte_swap_code_t k_short_integer_byte_swap_codes[] =
		{ _bs_code_2byte, 0 };
	static byte_swap_code_t k_long_integer_byte_swap_codes[] =
		{ _bs_code_4byte, 0 };

	static byte_swap_code_t k_point2d_byte_swap_codes[] =
		{ _bs_code_2byte, _bs_code_2byte, 0 };

	static byte_swap_code_t k_rectangle2d_byte_swap_codes[] =
		{ _bs_code_2byte, _bs_code_2byte, _bs_code_2byte, _bs_code_2byte, 0 };

	static byte_swap_code_t k_real_point2d_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_point3d_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_rgb_color_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_argb_color_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_quaternion_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_plane3d_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_tag_reference_byte_swap_codes[] = 
		{ _bs_code_4byte, sizeof(tag_reference_name_reference), _bs_code_4byte, sizeof(datum_index), 0 };

	static byte_swap_code_t k_tag_block_byte_swap_codes[] =
		{  _bs_code_4byte, _bs_code_4byte, sizeof(tag_block_definition*), 0 };

	static byte_swap_code_t k_tag_data_byte_swap_codes[] =
		{  _bs_code_4byte, sizeof(int32)*2, sizeof(void*)*2, 0 };

	static byte_swap_code_t k_start_array_byte_swap_codes[] =
		{ _bs_code_array_start, 0 };

	static byte_swap_code_t k_end_array_byte_swap_codes[] =
		{ _bs_code_array_end, 0 };

	static byte_swap_code_t k_pad_byte_swap_codes[] =
		{ 0 };

	static byte_swap_code_t k_explanation_byte_swap_codes[] =
		{ 0 };

	static byte_swap_code_t k_custom_byte_swap_codes[] =
		{ 0 };

	static byte_swap_code_t k_terminator_byte_swap_codes[] =
		{ _bs_code_array_end, 0 };
};

namespace Yelo { namespace TagGroups {
	const s_tag_field_definition k_tag_field_definitions[] = {
		{ sizeof(tag_string),			k_string_byte_swap_codes },
		{ sizeof(sbyte),				k_char_integer_byte_swap_codes },
		{ sizeof(int16),				k_short_integer_byte_swap_codes },
		{ sizeof(int32),				k_long_integer_byte_swap_codes },
		{ sizeof(angle),				k_long_integer_byte_swap_codes },
		{ sizeof(tag),					k_long_integer_byte_swap_codes },
		{ sizeof(_enum),				k_short_integer_byte_swap_codes },
		{ sizeof(long_flags),			k_long_integer_byte_swap_codes },
		{ sizeof(word_flags),			k_short_integer_byte_swap_codes },
		{ sizeof(byte_flags),			k_char_integer_byte_swap_codes },
		{ sizeof(point2d),				k_point2d_byte_swap_codes },
		{ sizeof(rectangle2d),			k_rectangle2d_byte_swap_codes },
		{ sizeof(rgb_color),			k_long_integer_byte_swap_codes },
		{ sizeof(argb_color),			k_long_integer_byte_swap_codes },

		{ sizeof(real),					k_long_integer_byte_swap_codes },
		{ sizeof(real_fraction),		k_long_integer_byte_swap_codes },
		{ sizeof(real_point2d),			k_real_point2d_byte_swap_codes },
		{ sizeof(real_point3d),			k_real_point3d_byte_swap_codes },
		{ sizeof(real_vector2d),		k_real_point2d_byte_swap_codes },
		{ sizeof(real_vector3d),		k_real_point3d_byte_swap_codes },
		{ sizeof(real_quaternion),		k_real_quaternion_byte_swap_codes },
		{ sizeof(real_euler_angles2d),	k_real_point2d_byte_swap_codes },
		{ sizeof(real_euler_angles3d),	k_real_point3d_byte_swap_codes },
		{ sizeof(real_plane2d),			k_real_point3d_byte_swap_codes },
		{ sizeof(real_plane3d),			k_real_plane3d_byte_swap_codes },
		{ sizeof(real_rgb_color),		k_real_rgb_color_byte_swap_codes },
		{ sizeof(real_argb_color),		k_real_argb_color_byte_swap_codes },

		{ sizeof(real_rgb_color),		k_real_rgb_color_byte_swap_codes },
		{ sizeof(real_argb_color),		k_real_argb_color_byte_swap_codes },

		{ sizeof(short_bounds),			k_point2d_byte_swap_codes },
		{ sizeof(angle_bounds),			k_real_point2d_byte_swap_codes },
		{ sizeof(real_bounds),			k_real_point2d_byte_swap_codes },
		{ sizeof(real_fraction_bounds),	k_real_point2d_byte_swap_codes },

		{ sizeof(tag_reference),		k_tag_reference_byte_swap_codes },
		{ sizeof(tag_block),			k_tag_block_byte_swap_codes },
		{ sizeof(int16),				k_short_integer_byte_swap_codes },
		{ sizeof(int32),				k_long_integer_byte_swap_codes },
		{ sizeof(tag_data),				k_tag_data_byte_swap_codes },

		{ 0,							k_start_array_byte_swap_codes },
		{ 0,							k_end_array_byte_swap_codes },
		{ 0,							k_pad_byte_swap_codes },
		{ 0,							k_pad_byte_swap_codes },
		{ 0,							k_explanation_byte_swap_codes },
		{ 0,							k_custom_byte_swap_codes },
		{ 0,							k_terminator_byte_swap_codes },

	}; BOOST_STATIC_ASSERT( NUMBEROF(k_tag_field_definitions) == Enums::k_number_of_tag_field_types );
}; };