/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
// NOTE: NON-STANDARD ENGINE SOURCE FILE
#include "Common/Precompile.hpp"
#if PLATFORM_IS_EDITOR
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		void s_tag_field_scan_state::InitializeWhatNewDoesnt()
		{
			field_size = 0;
			field_end_offset = 0;
#ifndef __TAG_FIELD_SCAN_USE_BLAM_DATA
			fields_debug_bytes = 0;
#endif
			pad = FALSE;

			// we intentionally don't initialize [stack]

			field = nullptr;
			field_address = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		// c_tag_field_scanner
		c_tag_field_scanner::c_tag_field_scanner(const tag_field* fields, void* block_element)
		{
			blam::tag_field_scan_state_new(m_state, fields, block_element);
			m_state.InitializeWhatNewDoesnt();
			m_state.SetYeloScanState();
		}

		c_tag_field_scanner& c_tag_field_scanner::AddFieldType(Enums::field_type field_type)
		{
			blam::tag_field_scan_state_add_field_type(m_state, field_type);
			return *this;
		}

		c_tag_field_scanner& c_tag_field_scanner::AddAllFieldTypes()
		{
			memset(m_state.field_types, -1, sizeof(m_state.field_types));
			return *this;
		}

		bool c_tag_field_scanner::Scan()
		{
			return blam::tag_field_scan(m_state);
		}
		c_tag_field_scanner& c_tag_field_scanner::ScanToEnd()
		{
			for(int debug_scan_count = 0; Scan(); debug_scan_count++)
				assert(debug_scan_count <= Enums::k_maximum_field_byte_swap_codes);

			return *this;
		}

		bool c_tag_field_scanner::TagFieldIsStringId() const
		{
			return TagGroups::TagFieldIsStringId(m_state.field);
		}
		bool c_tag_field_scanner::TagFieldIsOldStringId() const
		{
			return TagGroups::TagFieldIsOldStringId(m_state.field);
		}
		int32 c_tag_field_scanner::StringFieldGetLength() const
		{
			YELO_ASSERT(m_state.field->type == Enums::_field_string);

			return TagGroups::StringFieldGetLength(m_state.field);
		}

		bool c_tag_field_scanner::s_iterator::operator!=(const c_tag_field_scanner::s_iterator& other) const
		{
			if(other.IsEndHack())
				return !m_scanner->IsDone();
			else if(this->IsEndHack())
				return !other.m_scanner->IsDone();

			return m_scanner != other.m_scanner;
		}
	};

	namespace blam
	{
#ifdef __TAG_FIELD_SCAN_USE_BLAM_DATA
		static void tag_field_scan_set_field_address(TagGroups::s_tag_field_scan_state& state)
		{
			state.field_address = state.block_element == nullptr ?
				nullptr :
				state.field_address = CAST_PTR(byte*,state.block_element) + state.field_end_offset;
		}
#else
		static void tag_field_scan_set_field_address_for_yelo(TagGroups::s_tag_field_scan_state& state,
			size_t field_size, size_t field_debug_bytes)
		{
//			assert( field_debug_bytes>=0 );

			state.field_address = state.block_element == nullptr ?
				nullptr :
			state.field_address = CAST_PTR(byte*,state.block_element) + state.field_end_offset;

			state.field_size = CAST(int16, field_size);
			state.field_end_offset += CAST(uint16, field_size);
			state.fields_debug_bytes += CAST(uint16, field_debug_bytes);
		}
#endif
		bool PLATFORM_API tag_field_scan(TagGroups::s_tag_field_scan_state& state)
		{
			YELO_ASSERT( !state.done );

			const tag_field* field;
			do {
				field = &state.fields[state.field_count++];

				switch(field->type)
				{
				case Enums::_field_array_start:
					YELO_ASSERT( state.stack_index<Enums::k_tag_field_scan_stack_size );
					state.stack[state.stack_index].field_index = state.field_count;
					state.stack[state.stack_index].count = field->DefinitionCast<int16>(); // TODO: asm has this as int16, but all other array_start code treats the def as int32. is this cast really safe?
					YELO_ASSERT( state.stack[state.stack_index].count>0 );
					state.stack_index++;
					break;

				case Enums::_field_array_end:
					{
						YELO_ASSERT( state.stack_index>0 );
						auto& stack = state.stack[state.stack_index-1];

						if( --stack.count > 0 )
							state.field_count = stack.field_index;
						else
							--state.stack_index;
					}
					break;
				}

				state.field = field;
#ifdef __TAG_FIELD_SCAN_USE_BLAM_DATA
				tag_field_scan_set_field_address(state);
#endif

				size_t field_size;
				size_t field_runtime_size = 0;
				field_size = field->GetSize(&field_runtime_size);
#ifdef __TAG_FIELD_SCAN_USE_BLAM_DATA
				state.field_size = CAST(int16, field_size);
				state.field_end_offset += CAST(int32, field_size);
#else
				tag_field_scan_set_field_address_for_yelo(state, field_size, field_size - field_runtime_size);
#endif

				if(field->type == Enums::_field_terminator)
				{
					state.done = true;
					return false;
				}

			} while( !BIT_VECTOR_TEST_FLAG32(state.field_types, field->type) );

			return true;
		}
	};
};

#endif