#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <cseries/MacrosCpp.hpp>

namespace Yelo
{
	namespace e_field_type
	{
		typedef enum : _enum
		{
			string,
			char_integer,
			short_integer,
			long_integer,
			angle,
			tag,
			word_enum,
			long_flags,
			word_flags,
			byte_flags,
			point_2d,
			rectangle_2d,
			rgb_color,
			argb_color,
			real,
			real_fraction,
			real_point_2d,
			real_point_3d,
			real_vector_2d,
			real_vector_3d,
			real_quaternion,
			real_euler_angles_2d,
			real_euler_angles_3d,
			real_plane_2d,
			real_plane_3d,
			real_rgb_color,
			real_argb_color,
			real_hsv_color,
			real_ahsv_color,
			short_bounds,
			angle_bounds,
			real_bounds,
			real_fraction_bounds,
			tag_reference,
			block,
			short_block_index,
			long_block_index,
			data,
			array_start,
			array_end,
			pad,
			skip,
			explanation,
			custom,
			terminator,

			k_count
		} type_t;
	}

	struct tag_field
	{
		e_field_type::type_t type;
		PAD16;
		cstring name;
		void* definition;

		#if PLATFORM_IS_EDITOR
		template <typename t_type>
		t_type* get_definition() const
		{
			return reinterpret_cast<t_type*>(definition);
		}

		template <typename t_type>
		t_type get_definition_as() const
		{
			return reinterpret_cast<t_type>(definition);
		}

		size_t get_size(
			size_t* const runtime_size) const;
		bool is_read_only() const;
		bool is_advanced() const;
		bool is_block_name() const;
		bool is_invisible() const;
		#endif
	};

	ASSERT_SIZE(tag_field, 0xC);
}
