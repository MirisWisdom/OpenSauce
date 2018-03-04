#pragma once

#include <blamlib/hs/hs_types.hpp>

namespace Yelo { namespace Scripting
{
	class c_hs_type_abi
	{
	public:
		static const c_hs_type_abi* get(
			const Enums::hs_type type);

	protected:
		Enums::hs_type m_type;
		PAD16;

	public:
		explicit c_hs_type_abi(
			const Enums::hs_type type);

		virtual ~c_hs_type_abi() = default;

		Enums::hs_type get_type() const;
		cstring get_type_name() const;

		virtual s_hs_value_union get_default_value() const = 0;
		virtual s_hs_value_union read_value(
			const Enums::hs_type type,
			const s_hs_value_union*const source) const = 0;
		virtual void write_value(
			const Enums::hs_type type,
			const s_hs_value_union source,
			s_hs_value_union& destination) const = 0;
		virtual size_t get_size() const = 0;

		virtual bool parse(
			const Enums::hs_type type,
			const datum_index expression_index) const = 0;

		virtual bool inspectable() const = 0;
		virtual void inspect(
			const Enums::hs_type type,
			const s_hs_value_union value,
			char*const buffer,
			const size_t buffer_size) const = 0;

		virtual bool castable_to_value_union() const;

		virtual s_hs_value_union get_value(
			const s_hs_value_union value);

		virtual bool castable_to_hs_type_bool() const;

		virtual s_hs_value_union get_value(
			const bool value) const;

		virtual bool castable_to_hs_type_real() const;

		virtual s_hs_value_union get_value(
			const real value) const;

		virtual bool castable_to_hs_type_short() const;

		virtual s_hs_value_union get_value(
			const int16 value) const;

		virtual bool castable_to_hs_type_long() const;

		virtual s_hs_value_union get_value(
			const int32 value) const;

		virtual bool castable_to_hs_type_string() const;

		virtual s_hs_value_union get_value(
			cstring value) const;

		virtual bool castable_to_hs_type_script() const;

		virtual s_hs_value_union get_value(
			const hs_script_index_t index) const;

		virtual bool castable_to_hs_type_enum_game_difficulty() const;

		virtual s_hs_value_union get_value(
			const Enums::game_difficulty_level value) const;

		virtual bool castable_to_hs_type_enum_team() const;

		virtual s_hs_value_union get_value(
			const e_game_team::type_t value) const;

		virtual bool castable_to_hs_type_enum_ai_default_state() const;

		virtual s_hs_value_union get_value(
			const AI::e_actor_default_state::type_t value) const;

		virtual bool castable_to_hs_type_enum_actor_type() const;

		virtual s_hs_value_union get_value(
			const AI::e_actor_type::type_t value) const;

		virtual bool castable_to_hs_type_enum_hud_corner() const;

		virtual s_hs_value_union get_value(
			const Enums::hud_anchor value) const;
	};
}}
