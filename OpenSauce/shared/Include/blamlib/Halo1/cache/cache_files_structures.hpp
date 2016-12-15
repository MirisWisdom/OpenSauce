/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cache/cache_files_base.hpp>
#include <blamlib/Halo1/cache/cache_constants.hpp>
#include <blamlib/Halo1/cache/cache_files.hpp>
#include <YeloLib/Halo1/cache/cache_files_structures_yelo.hpp>

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
			int32 compressed_file_length;

			int32 offset_to_index;
			int32 tag_memory_size;
			PAD64; // ?
			tag_string name;
			tag_string build_string;
			Enums::cache_file_type cache_type;
			PAD16;
			uint32 crc;
			PAD32; // ?

			PAD32; // Align so the yelo header starts on a 16B boundary
			s_cache_header_yelo yelo;
			byte padding[k_pad_size - (sizeof(uint32) + sizeof(s_cache_header_yelo))];

			tag footer_signature;

			bool ValidSignatures() const;
			bool ValidFileSize(int32 max) const;
			bool ValidName() const;

			// Is this header valid according to the stock game's standards?
			bool ValidForStock() const;
			// Is this header valid according to OS standards?
			bool ValidForYelo() const;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );

		struct s_cache_tag_instance
		{
			tag group_tag;			// 0x0
			tag parent_groups[2];	// 0x4
			datum_index handle;		// 0xC
			cstring name;			// 0x10
			union {
				void* base_address;	// 0x14
				int32 index_in_data_file;
			};
			BOOL bool_in_data_file;	// 0x18
			uint32 _unused;			// 0x1C

			template<typename T>
			T* Definition() const { return CAST_PTR(T*, base_address); }

			int32 GetAbsoluteIndex() const { return handle.index; }

			// Is this an instance of a certain tag group?
			// If this instance a child of a certain tag group?
			bool MatchesGroup(tag group_tag) const;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_instance) == 0x20 );

		struct s_cache_tag_header
		{
			enum {
				k_signature = 'tags'
			};

			void* tags_address;			// 0x0
			datum_index scenario_index;	// 0x4
			uint32 checksum;			// 0x8
			int32 count;				// 0xC
			struct {
				int32 count;
				int32 offset;
			}vertices, indices;			// 0x10, 0x18
			int32 model_data_size;		// 0x20
			tag signature;				// 0x24

			// nasty hack assumption
#pragma warning(push)
#pragma warning(disable : 4200)
			s_cache_tag_instance tags[];
#pragma warning(pop)
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_header) == 0x28 );
	};
};