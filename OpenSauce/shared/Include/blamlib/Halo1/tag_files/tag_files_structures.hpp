/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>

namespace Yelo
{
	struct tag_group;

	namespace TagGroups
	{
		struct s_tag_header : Memory::s_datum_base
		{
			enum {
				k_signature = 'blam',		// signature for files following Bungie's tag file format

				// signature for files following our custom tag file format:
				// * little endian
				k_signature_yelo = 'yelo',
			};

			byte_flags flags;	// UNUSED in Halo
			byte type;			// UNUSED in Halo

			tag_string name;	// UNUSED in Halo

			tag group_tag;
			uint32 crc;
			int32 offset;		// actual offset in the file
			int32 size;			// UNUSED in Halo

			uint32 user_data;	// UNUSED in Halo
			int16 version;

			sbyte foundation_tag_file_index;	// UNUSED in Halo
			sbyte owner_index;	// UNUSED in Halo. default value is NONE

			tag signature;

			bool IsBungieFormat() const { return signature == k_signature; }
			bool IsYeloFormat() const { return signature == k_signature_yelo; }

			void InitializeForBungieFormat(int16 tag_version);
			void InitializeForYeloFormat(int16 tag_version);
		private:
			void Initialize(int16 tag_version);
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_header) == 0x40 );


		struct s_tag_file_globals
		{
			bool open;
			bool single_file_open;
			bool writing;
			char single_file_name[Enums::k_max_tag_name_length+1];
			s_file_reference single_file_reference;
			byte_flags yelo_flags;
			s_tag_header single_file_header;

		private:
			char* GetSingleFilePath(char buffer[Enums::k_maximum_filename_length+1]) const;

			bool SingleFileCreate(tag group_tag, cstring name);
			void SingleFileClose();

			void ReadHeaderFailure(const tag_group* expected_group) const;
			void ReadHeaderInvalidFormat(const tag_group* expected_group, cstring format_type) const;
			void ReadHeaderGroupMismatch(const tag_group* expected_group, const tag_group* found_group) const;
			void ReadHeaderVersionMistmatch(const tag_group* expected_group, cstring reason) const;
		public:
			bool ReadHeader(tag group_tag);
			bool WriteHeader(s_tag_header& header);

			bool SingleFileOpen(tag group_tag, cstring name, bool for_writing, bool from_file_system);

			bool New(tag group_tag, cstring name);
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_file_globals) == 0x250 );
	};
};