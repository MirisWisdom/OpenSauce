#include <blam/blam.h>
#include <blam/hs/c_hs_type_void_abi.h>

namespace Yelo { namespace Scripting
{
	c_hs_type_void_abi::c_hs_type_void_abi(
		const Enums::hs_type type)
		: c_hs_type_abi(type) { }

	s_hs_value_union c_hs_type_void_abi::get_default_value() const
	{
		return k_none_value_union;
	}

	s_hs_value_union c_hs_type_void_abi::read_value(
		const Enums::hs_type type,
		const s_hs_value_union*const source) const
	{
		return k_none_value_union;
	}

	void c_hs_type_void_abi::write_value(
		const Enums::hs_type type,
		const s_hs_value_union source,
		s_hs_value_union& destination) const { }

	size_t c_hs_type_void_abi::get_size() const
	{
		return 0;
	}

	bool c_hs_type_void_abi::parse(
		const Enums::hs_type type,
		const datum_index expression_index) const
	{
		return false;
	}

	bool c_hs_type_void_abi::inspectable() const
	{
		return false;
	}

	void c_hs_type_void_abi::inspect(
		const Enums::hs_type type,
		const s_hs_value_union value,
		char*const buffer,
		const size_t buffer_size) const { }
}}
