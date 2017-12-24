#include <Common/Precompile.hpp>
#include <blamlib/hs/c_hs_type_value_abi.h>

namespace Yelo { namespace Scripting
{
	c_hs_type_value_abi::c_hs_type_value_abi(
		const Enums::hs_type type,
		const s_hs_value_union default_value,
		const proc_hs_type_parse parse,
		const proc_hs_type_inspect inspect): c_hs_type_abi(type)
		, m_default_value(default_value)
		, m_parse(parse)
		, m_inspect(inspect) { }

	s_hs_value_union c_hs_type_value_abi::get_default_value() const
	{
		return m_default_value;
	}

	bool c_hs_type_value_abi::parse(
		const Enums::hs_type type,
		const datum_index expression_index) const
	{
		return m_parse(type, expression_index);
	}

	bool c_hs_type_value_abi::inspectable() const
	{
		return m_inspect != nullptr;
	}

	void c_hs_type_value_abi::inspect(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char* const buffer,
		const size_t buffer_size) const
	{
		m_inspect(type, value, buffer, buffer_size);
	}
}}
