/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cache/cache_files_base.hpp>
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header : public s_cache_header_base
		{
			enum {
				k_version = 609,

				k_pad_size = 484 * sizeof(int32),
			};

			tag header_signature;

			int32 version;
			int32 file_length;
			uint32 compressed_file_length;

			uint32 offset_to_index;
			uint32 tag_memory_size;
			PAD32; // ?
			PAD32; // ?
			tag_string name;
			tag_string build_string;
			_enum cache_type;
			PAD16;
			uint32 crc;
			PAD32; // ?

			PAD32; // Align so the yelo header starts on a 16B boundary
			s_cache_header_yelo yelo;
			byte padding[k_pad_size - (sizeof(uint32) + sizeof(s_cache_header_yelo))];

			tag footer_signature;

			API_INLINE bool ValidSignatures() const			{ return header_signature == k_header_signature && footer_signature == k_footer_signature; }
			API_INLINE bool ValidFileSize(int32 max) const	{ return file_length >= 0 && file_length <= max; }
			API_INLINE bool ValidName() const				{ return strnlen_s(name, Enums::k_tag_string_length) <= Enums::k_tag_string_length; }
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );

		struct s_cache_tag_instance
		{
			tag group_tag;			// 0x0
			tag parent_groups[2];	// 0x4
			datum_index datum;		// 0xC
			cstring name;			// 0x10
			void* definition;		// 0x14
			BOOL bool_in_data_file;	// 0x18
			UNUSED_TYPE(int32);		// 0x1C

			template<typename T> API_INLINE T* Definition() const { return CAST_PTR(T*, definition); }

			// Is this an instance of a certain tag group?
			// If this instance a child of a certain tag group?
			API_INLINE bool MatchesGroup(tag group_tag) const
			{
				return this->group_tag == group_tag ||
					// test the hierarchy graph
					parent_groups[0] == group_tag ||
					parent_groups[1] == group_tag;
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_instance) == 0x20 );

		struct s_cache_tag_header
		{
			void* tags_address;		// 0x0
			datum_index scenario;	// 0x4
			UNKNOWN_TYPE(int32);	// 0x8
			int32 count;			// 0xC
			struct {
				int32 count;
				int32 offset;
			}vertices, indices;		// 0x10, 0x18
			uint32 model_data_size;	// 0x20
			tag signature;			// 0x24

			// nasty hack assumption
#pragma warning(push)
#pragma warning(disable : 4200)
			s_cache_tag_instance tags[];
#pragma warning(pop)
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_header) == 0x28 );
	};
};