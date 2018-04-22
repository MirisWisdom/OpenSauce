#pragma once

#include <blam/hs/c_hs_type_value_abi.h>

namespace Yelo { namespace Scripting
{
	struct c_hs_type_bool_abi final
		: c_hs_type_value_abi
	{
		c_hs_type_bool_abi(
			const Enums::hs_type type,
			const s_hs_value_union default_value,
			const proc_hs_type_parse parse,
			const proc_hs_type_inspect inspect);

		size_t get_size() const override;

		s_hs_value_union read_value(
			const Enums::hs_type type,
			const s_hs_value_union* const source) const override;
		void write_value(
			const Enums::hs_type type,
			const s_hs_value_union source,
			s_hs_value_union& destination) const override;
	};
}}
