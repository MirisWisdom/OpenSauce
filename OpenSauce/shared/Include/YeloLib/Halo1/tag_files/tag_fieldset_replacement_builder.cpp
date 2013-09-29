/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		void c_tag_field_set_replacement_builder::NextSourceField(bool copy_current_field_first)
		{
			YELO_ASSERT( !CurrentSourceFieldIsTerminator() );

			if(copy_current_field_first)
			{
				YELO_ASSERT(m_target_fields_cursor < m_target_fields_max_count);
				tag_field* dst=			&m_target_fields[m_target_fields_cursor++];
				const tag_field* src=	&m_source_fields[m_source_fields_cursor];
				std::memcpy(dst, src, sizeof(*src));
			}

			scanner.Scan();
			++m_source_fields_cursor;
		}

		bool c_tag_field_set_replacement_builder::CurrentSourceFieldIsTerminator() const
		{
			return m_source_fields[m_source_fields_cursor].type == Enums::_field_terminator;
		}

		c_tag_field_set_replacement_builder::c_tag_field_set_replacement_builder(
			const tag_field* source_fields,
			tag_field* target_fields, size_t target_fields_size, size_t target_fields_count)
			:
			m_source_fields( source_fields ),
			m_source_fields_cursor( 0 ),

			m_target_fields( target_fields ),
			m_target_fields_cursor( 0 ),
			m_target_fields_max_count( target_fields_size ),

			m_available_pad_size( 0 ),
			scanner( source_fields )
		{
			YELO_ASSERT( source_fields != nullptr );
			YELO_ASSERT( target_fields != nullptr && target_fields_size > 0 );
			// the target fields shouldn't be initialized yet
			YELO_ASSERT( target_fields_count == 0 );

			scanner.AddAllFieldTypes();
		}

		void c_tag_field_set_replacement_builder::CopyFieldsFromSource(size_t field_count)
		{
			for(size_t x = 0; x < field_count && !CurrentSourceFieldIsTerminator(); x++)
				;
		}

		void c_tag_field_set_replacement_builder::SkipFieldsFromSource(size_t field_count)
		{
		}
	};
};