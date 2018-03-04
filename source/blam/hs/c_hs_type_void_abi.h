#pragma once

#include <blamlib/hs/c_hs_type_abi.h>

namespace Yelo { namespace Scripting
{
	struct c_hs_type_void_abi final
		: c_hs_type_abi
	{
		explicit c_hs_type_void_abi(
			Enums::hs_type type);

		s_hs_value_union get_default_value() const override;

		s_hs_value_union read_value(
			const Enums::hs_type type,
			const s_hs_value_union*const source) const override;

		void write_value(
			const Enums::hs_type type,
			const s_hs_value_union source,
			s_hs_value_union& destination) const override;

		size_t get_size() const override;

		bool parse(
			const Enums::hs_type type,
			const datum_index expression_index) const override;

		bool inspectable() const override;

		void inspect(
			const Enums::hs_type type,
			const s_hs_value_union value,
			char* const buffer,
			const size_t buffer_size) const override;
	};
}}
