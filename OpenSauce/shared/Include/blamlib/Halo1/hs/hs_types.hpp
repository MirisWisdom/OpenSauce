/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs.hpp>

namespace Yelo
{
	namespace Scripting
	{
		typedef s_hs_value_union (API_FUNC* proc_hs_typecast)(s_hs_value_union value);

		extern proc_hs_typecast k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types];
		void HsTypesInitialize();
		void HsTypesDispose();


		typedef bool (API_FUNC* proc_hs_type_parse)(Enums::hs_type type, datum_index expression_index);
		typedef void (API_FUNC* proc_hs_type_inspect)(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size);

		class c_hs_type_abi abstract
		{
		public:
			static const c_hs_type_abi* Get(Enums::hs_type type);

		protected:
			Enums::hs_type m_type;
			PAD16;

		public:
			c_hs_type_abi(Enums::hs_type type)
				: m_type(type)
			{
			}

			Enums::hs_type GetType() const								{ return m_type; }
			cstring GetTypeName() const;

			virtual s_hs_value_union GetDefaultValue() const abstract;
			virtual s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union* source) const abstract;
			virtual void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union& destination) const abstract;
			virtual size_t GetSize() const abstract;

			virtual bool Parse(Enums::hs_type type, datum_index expression_index) const abstract;

			virtual bool Inspectable() const abstract;
			virtual void Inspect(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size) const abstract;

			virtual bool CastableToValueUnion() const							{ return CastableTo_hs_type_long(); }
			virtual s_hs_value_union GetValue(s_hs_value_union value)			{ return GetValue(value.int32); }

			virtual bool CastableTo_hs_type_bool() const						{ return false; }
			virtual s_hs_value_union GetValue(bool value) const					{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_real() const						{ return false; }
			virtual s_hs_value_union GetValue(real value) const					{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_short() const						{ return false; }
			virtual s_hs_value_union GetValue(int16 value) const				{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_long() const						{ return false; }
			virtual s_hs_value_union GetValue(int32 value) const				{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_string() const						{ return false; }
			virtual s_hs_value_union GetValue(cstring value) const				{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_script() const						{ return false; }
			virtual s_hs_value_union GetValue(hs_script_index_t index) const	{ return k_none_value_union; }


			virtual bool CastableTo_hs_type_enum_game_difficulty() const				{ return false; }
			virtual s_hs_value_union GetValue(Enums::game_difficulty_level value) const	{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_team() const							{ return false; }
			virtual s_hs_value_union GetValue(Enums::game_team value) const				{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_ai_default_state() const				{ return false; }
			virtual s_hs_value_union GetValue(Enums::actor_default_state value) const	{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_actor_type() const						{ return false; }
			virtual s_hs_value_union GetValue(Enums::actor_type value) const			{ return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_hud_corner() const						{ return false; }
			virtual s_hs_value_union GetValue(Enums::hud_anchor value) const			{ return k_none_value_union; }
		};

		// Yes, I declared as structs, to avoid having to explicitly say 'public' for inheritance and members

		struct c_hs_type_void_abi final
			:  c_hs_type_abi
		{
			c_hs_type_void_abi(Enums::hs_type type)
				: c_hs_type_abi(type)
			{
			}

			s_hs_value_union GetDefaultValue() const override															{ return k_none_value_union; }
			s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union* source) const override				{ return k_none_value_union; }
			void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union& destination) const override	{ }
			size_t GetSize() const override																				{ return 0; }
			bool Parse(Enums::hs_type type, datum_index expression_index) const override								{ return false; }
			bool Inspectable() const override																			{ return false; }
			void Inspect(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size) const override	{ }
		};

		struct c_hs_type_value_abi abstract 
			:  c_hs_type_abi
		{
		protected:
			s_hs_value_union m_default_value;
			proc_hs_type_parse m_parse;
			proc_hs_type_inspect m_inspect;

		public:
			c_hs_type_value_abi(Enums::hs_type type, s_hs_value_union default_value, proc_hs_type_parse parse, proc_hs_type_inspect inspect)
				: c_hs_type_abi(type)
				, m_default_value(default_value)
				, m_parse(parse)
				, m_inspect(inspect)
			{
			}

			s_hs_value_union GetDefaultValue() const override															{ return m_default_value; }

			bool Parse(Enums::hs_type type, datum_index expression_index) const override								{ return m_parse(type, expression_index); }
			bool Inspectable() const override																			{ return m_inspect != nullptr; }
			void Inspect(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size) const override	{ m_inspect(type, value, buffer, buffer_size); }
		};

		struct c_hs_type_bool_abi final
			:  c_hs_type_value_abi
		{
			c_hs_type_bool_abi(Enums::hs_type type, s_hs_value_union default_value, proc_hs_type_parse parse, proc_hs_type_inspect inspect)
				: c_hs_type_value_abi(type, default_value, parse, inspect)
			{
			}

			size_t GetSize() const override	{ return sizeof(bool); }

			s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union* source) const override;
			void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union& destination) const override;
		};
	};
};