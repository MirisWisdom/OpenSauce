#include <Common/Precompile.hpp>
#include <blamlib/hs/c_hs_type_abi.h>

#include <blamlib/hs/c_hs_type_bool_abi.h>
#include <blamlib/hs/c_hs_type_void_abi.h>

namespace Yelo { namespace Scripting
{
	static const c_hs_type_void_abi k_hs_unparsed_abi {Enums::_hs_unparsed};
	static const c_hs_type_void_abi k_hs_special_form_abi {Enums::_hs_special_form};
	static const c_hs_type_void_abi k_hs_function_name_abi {Enums::_hs_function_name};
	static const c_hs_type_void_abi k_hs_passthrough_abi {Enums::_hs_passthrough};
	static const c_hs_type_void_abi k_hs_type_void_abi {Enums::_hs_type_void};
	static const c_hs_type_bool_abi k_hs_type_bool_abi {Enums::_hs_type_bool,{false}, nullptr, nullptr};

	c_hs_type_abi::c_hs_type_abi(
		const Enums::hs_type type)
		: m_type(type) { }

	Enums::hs_type c_hs_type_abi::get_type() const
	{
		return m_type;
	}

	const c_hs_type_abi* c_hs_type_abi::get(
		const Enums::hs_type type)
	{
		static std::array<const c_hs_type_abi*, Enums::k_number_of_hs_types> hs_type_abis = {

			&k_hs_unparsed_abi,
			&k_hs_special_form_abi,
			&k_hs_function_name_abi,
			&k_hs_passthrough_abi,
			&k_hs_type_void_abi,

			&k_hs_type_bool_abi
		};

		return hs_type_abis[type];
	}

	cstring c_hs_type_abi::get_type_name() const
	{
		return blam::hs_type_names[m_type];
	}

	bool c_hs_type_abi::castable_to_value_union() const
	{
		return castable_to_hs_type_long();
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const s_hs_value_union value)
	{
		return get_value(value.int32);
	}

	bool c_hs_type_abi::castable_to_hs_type_bool() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const bool value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_real() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const real value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_short() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const int16 value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_long() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const int32 value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_string() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		cstring value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_script() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const hs_script_index_t index) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_enum_game_difficulty() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const Enums::game_difficulty_level value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_enum_team() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const e_game_team::type_t value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_enum_ai_default_state() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const AI::e_actor_default_state::type_t value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_enum_actor_type() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const AI::e_actor_type::type_t value) const
	{
		return k_none_value_union;
	}

	bool c_hs_type_abi::castable_to_hs_type_enum_hud_corner() const
	{
		return false;
	}

	s_hs_value_union c_hs_type_abi::get_value(
		const Enums::hud_anchor value) const
	{
		return k_none_value_union;
	}
}}
