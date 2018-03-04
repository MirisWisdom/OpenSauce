#pragma once

#include <blamlib/hs/c_hs_type_abi.h>

namespace Yelo { namespace Scripting
{
	struct c_hs_type_value_abi
		: c_hs_type_abi
	{
	protected:
		s_hs_value_union m_default_value;
		proc_hs_type_parse m_parse;
		proc_hs_type_inspect m_inspect;

	public:
		c_hs_type_value_abi(
			const Enums::hs_type type,
			const s_hs_value_union default_value,
			const proc_hs_type_parse parse,
			const proc_hs_type_inspect inspect);

		s_hs_value_union get_default_value() const override;

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
