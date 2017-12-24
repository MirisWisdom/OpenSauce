#include <Common/Precompile.hpp>
#include <blamlib/hs/c_hs_type_bool_abi.h>

#include <blamlib/hs/c_hs_type_abi.h>

namespace Yelo { namespace Scripting
{
	c_hs_type_bool_abi::c_hs_type_bool_abi(
		const Enums::hs_type type,
		const s_hs_value_union default_value,
		const proc_hs_type_parse parse,
		const proc_hs_type_inspect inspect): c_hs_type_value_abi(type, default_value, parse, inspect) { }

	size_t c_hs_type_bool_abi::get_size() const
	{
		return sizeof(bool);
	}

	s_hs_value_union c_hs_type_bool_abi::read_value(
		const Enums::hs_type type,
		const s_hs_value_union*const source) const
	{
		if (source != nullptr)
		{
			return get_value(source->boolean);
		}

		return get_default_value();
	}

	void c_hs_type_bool_abi::write_value(
		const Enums::hs_type type,
		const s_hs_value_union source,
		s_hs_value_union& destination) const
	{
		destination.boolean = source.boolean;
	}
}}
