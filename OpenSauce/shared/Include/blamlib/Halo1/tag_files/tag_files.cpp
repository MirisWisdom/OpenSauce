/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_files.hpp>

#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		bool TagFileRequiresByteSwap()
		{
			return true; // TODO: reference tag_file_globals
		}

		char* s_tag_file_globals::GetSingleFilePath(char buffer[Enums::k_maximum_filename_length+1]) const
		{
			return blam::file_reference_get_name(single_file_reference,
				FLAG(Flags::_name_directory_bit) | FLAG(Flags::_name_file_bit) | FLAG(Flags::_name_extension_bit),
				buffer);
		}

		void s_tag_file_globals::SingleFileClose()
		{
			if (!open)
				return;

			if (writing)
				WriteHeader(single_file_header);

			open = false;
			blam::file_close(single_file_reference);
		}

		void s_tag_file_globals::ReadHeaderFailure(const tag_group* expected_group) const
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			YELO_ERROR(_error_message_priority_assert, 
				"couldn't read header from the %s tag '%s' (%s)", 
				expected_group->name, single_file_name, GetSingleFilePath(debug_filename));
		}
		void s_tag_file_globals::ReadHeaderInvalidFormat(const tag_group* expected_group, cstring format_type) const
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			YELO_ERROR(_error_message_priority_assert, 
				"the %s tag '%s' (%s) had an invalid header (%s)", 
				expected_group->name, single_file_name, GetSingleFilePath(debug_filename),
				format_type);
		}
		void s_tag_file_globals::ReadHeaderGroupMismatch(const tag_group* expected_group, const tag_group* found_group) const
		{
			cstring found_name = found_group == nullptr
				? "UNKNOWN GROUP"
				: found_group->name;

			YELO_ERROR(_error_message_priority_assert, 
				"the %s tag '%s' was the wrong group type (%s)", 
				expected_group->name, single_file_name, found_name);
		}
		void s_tag_file_globals::ReadHeaderVersionMistmatch(const tag_group* expected_group, cstring reason) const
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			YELO_ERROR(_error_message_priority_assert, 
				"the %s tag '%s' has an invalid (%s) version", 
				expected_group->name, GetSingleFilePath(debug_filename), reason);
		}
		bool s_tag_file_globals::ReadHeader(tag group_tag)
		{
			bool result = false;
			const tag_group* group = blam::tag_group_get(group_tag);

			YELO_ASSERT(group);
			YELO_ASSERT(single_file_open);

			if (!blam::file_read_from_position(single_file_reference, 0, sizeof(single_file_header), &single_file_header))
			{
				ReadHeaderFailure(group);
				return false;
			}

			if (!single_file_header.IsYeloFormat())
			{
				blam::byte_swap_data(TagHeaderBsDefinition(), &single_file_header);
				if (!single_file_header.IsBungieFormat())
				{
					ReadHeaderInvalidFormat(group, "bungie-format");
					return false;
				}
			}

			const tag_group* file_group = blam::tag_group_get(single_file_header.group_tag);
			if (file_group == nullptr || file_group != group)
			{
				ReadHeaderGroupMismatch(group, file_group);
				return false;
			}

			if (single_file_header.version < group->version)
			{
				ReadHeaderVersionMistmatch(group, "older");
				return false;
			}
			else if (single_file_header.version > group->version)
			{
				ReadHeaderVersionMistmatch(group, "newer");
				return false;
			}
			else
				result = true;

			return result;
		}

		bool s_tag_file_globals::WriteHeader(s_tag_header& header)
		{
			YELO_ASSERT(single_file_open);

			bool byte_swap = header.IsBungieFormat();

			if (byte_swap)
				blam::byte_swap_data(TagHeaderBsDefinition(), &header);

			blam::file_write_to_position(single_file_reference, 0,
				sizeof(header), &header);

			if (byte_swap)
				blam::byte_swap_data(TagHeaderBsDefinition(), &header);

			memmove_s(&single_file_header, sizeof(single_file_header), 
				&header, sizeof(header));

			return true;
		}

		bool s_tag_file_globals::SingleFileOpen(tag group_tag, cstring name, bool for_writing, bool from_file_system)
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			bool result = false;
			const tag_group* group = blam::tag_group_get(group_tag);

			YELO_ASSERT(group);

			SingleFileClose();

			if (*name == '\\')
			{
				YELO_ERROR(_error_message_priority_assert,
					"the %s tag '%s' starts with a directory character",
					group->name, name);
				return false;
			}

			if (!blam::tag_file_get_file_reference(single_file_reference, group_tag, name) && !from_file_system)
			{
				YELO_ERROR(_error_message_priority_assert,
					"the %s tag '%s' was not found in the index file",
					group->name, name);

				return false;
			}

			strncpy_s(single_file_name, name, NUMBEROF(single_file_name)-1);
			single_file_name[NUMBEROF(single_file_name)-1] = '\0';

			if (from_file_system && !blam::file_exists(single_file_reference))
				return false;

			if (for_writing && blam::file_read_only(single_file_reference))
			{
				YELO_ERROR(_error_message_priority_assert, 
					"the %s tag '%s' cannot be opened for writing", 
					group->name, GetSingleFilePath(debug_filename));
				return false;
			}

			long_flags permissions = for_writing
				? FLAG(Flags::_permission_write_bit)
				: FLAG(Flags::_permission_read_bit);
			if (!blam::file_open(single_file_reference, permissions))
			{
				YELO_ERROR(_error_message_priority_assert, 
					"the %s tag '%s' could not be opened", 
					group->name, GetSingleFilePath(debug_filename));
				return false;
			}
			else
			{
				single_file_open = true;
				this->writing = for_writing;
				result = true;
			}

			return result;
		}
	};

	namespace blam
	{
	};
};