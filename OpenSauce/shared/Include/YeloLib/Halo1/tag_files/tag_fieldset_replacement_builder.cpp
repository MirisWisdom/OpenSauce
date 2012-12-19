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
		void c_tag_fieldset_replacement_builder::NextSourceField(bool copy_current_field_first)
		{
			YELO_ASSERT( !CurrentSourceFieldIsTerminator() );

			if(copy_current_field_first)
			{
				YELO_ASSERT(m_target_fields_cursor < m_target_fields_max_count);
				tag_field* dst=			&m_target_fields[m_target_fields_cursor++];
				const tag_field* src=	&m_source_fields[m_source_fields_cursor];
				memcpy_s(dst, sizeof(*dst), src, sizeof(*src));
			}

			scanner.Scan();
			++m_source_fields_cursor;
		}

		bool c_tag_fieldset_replacement_builder::CurrentSourceFieldIsTerminator() const
		{
			return m_source_fields[m_source_fields_cursor].type == Enums::_field_terminator;
		}

		c_tag_fieldset_replacement_builder::c_tag_fieldset_replacement_builder(
			const tag_field* source_fields,
			tag_field* target_fields, int target_fields_size, int target_fields_count)
			:
			m_source_fields( source_fields ),
			m_source_fields_cursor( 0 ),

			m_target_fields( target_fields ),
			m_target_fields_cursor( 0 ),
			m_target_fields_max_count( target_fields_size ),

			m_available_pad_size( NONE ),
			scanner(source_fields, NULL)
		{
			YELO_ASSERT( source_fields != NULL );
			YELO_ASSERT( target_fields != NULL && target_fields_size > 0 );
			// the target fields shouldn't be initialized yet
			YELO_ASSERT( target_fields_count == 0 );

			scanner.AddAllFieldTypes();
		}

		void c_tag_fieldset_replacement_builder::CopyFieldsFromSource(int field_count)
		{
			for(int x = 0; x < field_count && !CurrentSourceFieldIsTerminator(); x++)
				;
		}

		void c_tag_fieldset_replacement_builder::SkipFieldsFromSource(int field_count)
		{
		}
	};
};