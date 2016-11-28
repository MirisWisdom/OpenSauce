/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_files.hpp>

#if PLATFORM_IS_EDITOR
#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>
#include <blamlib/Halo1/tag_files/tag_files_structures.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>
#endif

namespace Yelo
{
	namespace TagGroups
	{
#if PLATFORM_IS_EDITOR
		cstring K_TAG_FILES_DIRECTORY = "tags\\";
		cstring K_DATA_FILES_DIRECTORY = "data\\";

		void s_tag_header::Initialize(int16 tag_version)
		{
			ZeroMemory(this, sizeof(*this));

			crc = NONE;
			version = tag_version;
			owner_index = NONE;
			offset = sizeof(*this);
		}
		void s_tag_header::InitializeForBungieFormat(int16 tag_version)
		{
			Initialize(tag_version);

			signature = k_signature;
		}
		void s_tag_header::InitializeForYeloFormat(int16 tag_version)
		{
			Initialize(tag_version);

			signature = k_signature_yelo;
		}

		char* s_tag_file_globals::GetSingleFilePath(char buffer[Enums::k_maximum_filename_length+1]) const
		{
			return blam::file_reference_get_name(single_file_reference,
				Flags::_name_full_path_flags, buffer);
		}

		bool s_tag_file_globals::SingleFileCreate(tag group_tag, cstring name)
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			SingleFileClose();

			s_file_reference file_reference;
			blam::tag_file_get_file_reference(file_reference, group_tag, name);

			if (blam::tag_file_read_only(group_tag, name))
			{
				YELO_ERROR_FAILURE("couldn't overwrite read-only tag file '%s'", 
					blam::file_reference_get_name(single_file_reference,
					Flags::_name_full_path_flags, debug_filename));

				return false;
			}

			if (!blam::file_create(file_reference))
			{
				YELO_ERROR_FAILURE("couldn't create tag file '%s'", 
					blam::file_reference_get_name(single_file_reference,
					Flags::_name_full_path_flags, debug_filename));

				return false;
			}

			return true;
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

			YELO_ERROR_FAILURE("couldn't read header from the %s tag '%s' (%s)", 
				expected_group->name, single_file_name, GetSingleFilePath(debug_filename));
		}
		void s_tag_file_globals::ReadHeaderInvalidFormat(const tag_group* expected_group, cstring format_type) const
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			YELO_ERROR_FAILURE("the %s tag '%s' (%s) had an invalid header (%s)", 
				expected_group->name, single_file_name, GetSingleFilePath(debug_filename),
				format_type);
		}
		void s_tag_file_globals::ReadHeaderGroupMismatch(const tag_group* expected_group, const tag_group* found_group) const
		{
			cstring found_name = found_group == nullptr
				? "UNKNOWN GROUP"
				: found_group->name;

			YELO_ERROR_FAILURE("the %s tag '%s' was the wrong group type (%s)", 
				expected_group->name, single_file_name, found_name);
		}
		void s_tag_file_globals::ReadHeaderVersionMistmatch(const tag_group* expected_group, cstring reason) const
		{
			char debug_filename[Enums::k_maximum_filename_length+1];

			YELO_ERROR_FAILURE("the %s tag '%s' has an invalid (%s) version", 
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
				YELO_ERROR_FAILURE("the %s tag '%s' starts with a directory character",
					group->name, name);
				return false;
			}

			// TODO: need to rewrote these conditions for tag_file_index support
			if (!blam::tag_file_get_file_reference(single_file_reference, group_tag, name) && !from_file_system)
			{
				YELO_ERROR_FAILURE("the %s tag '%s' was not found in the index file",
					group->name, name);

				return false;
			}

			strncpy_s(single_file_name, name, NUMBEROF(single_file_name)-1);
			single_file_name[NUMBEROF(single_file_name)-1] = '\0';

			if (from_file_system && !blam::file_exists(single_file_reference))
				return false;

			if (for_writing && blam::file_read_only(single_file_reference))
			{
				YELO_ERROR_FAILURE("the %s tag '%s' cannot be opened for writing", 
					group->name, GetSingleFilePath(debug_filename));
				return false;
			}

			long_flags permissions = for_writing
				? FLAG(Flags::_permission_write_bit)
				: FLAG(Flags::_permission_read_bit);
			if (!blam::file_open(single_file_reference, permissions))
			{
				YELO_ERROR_FAILURE("the %s tag '%s' could not be opened", 
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

		bool s_tag_file_globals::New(tag group_tag, cstring name)
		{
			const tag_group* group = blam::tag_group_get(group_tag);

			YELO_ASSERT(open);
			YELO_ASSERT(group);

			// NOTE: engine actually does this after the directories have been created
			if (*name == '\0')
				return false;

			// populate the new file reference with the path to the tags directory
			s_file_reference new_file_reference;
			blam::file_reference_create(new_file_reference, Enums::_file_reference_location_tags);
			blam::file_reference_add_directory(new_file_reference,
				Settings::c_settings_cheape::Profile().GetTagsPath());
			// will create the tags directory if it doesn't exist
			if (!blam::file_exists(new_file_reference))
				blam::file_create(new_file_reference);

			// create a local copy of the tag name for use in strtok() operations
			char tag_path[Enums::k_maximum_filename_length+1];
			strncpy_s(tag_path, name, Enums::k_maximum_filename_length);
			tag_path[Enums::k_maximum_filename_length] = '\0';

			// create any directories between the tags folder and file name
			if (char* last_slash = strrchr(tag_path, '\\'))
			{
				*last_slash = '\0'; // terminate the last separator so we don't include the filename

				for (char* next_dir_token, *directory = strtok_s(tag_path, "\\", &next_dir_token);
					directory != nullptr;
					directory = strtok_s(nullptr, "\\", &next_dir_token))
				{
					blam::file_reference_add_directory(new_file_reference, directory);

					if (!blam::file_exists(new_file_reference))
						blam::file_create(new_file_reference);
				}
			}

			if (!SingleFileCreate(group_tag, name))
				return false;

			if (!SingleFileOpen(group_tag, name, true, false))
				return false;

			// NOTE: engine actually initializes this before the Open() call
			s_tag_header header;
			header.InitializeForBungieFormat(group->version);

			if (!WriteHeader(header))
				return false;

			return true;
		}

		bool TagFileRequiresByteSwap()
		{
			return true; // TODO: reference tag_file_globals
		}
#endif
	};

	namespace blam
	{
#if PLATFORM_IS_EDITOR
		bool PLATFORM_API tag_file_open_impl(tag group_tag, cstring filename,
			_Out_opt_ bool* is_readonly, _Out_opt_ uint32* crc, bool from_file_system)
		{
			auto& globals = *TagGroups::TagFileGlobals();

			if (!globals.SingleFileOpen(group_tag, filename, false, from_file_system) ||
				!globals.ReadHeader(group_tag))
			{
				return false;
			}

			if (is_readonly)
				*is_readonly = file_read_only(globals.single_file_reference);

			if (crc)
				*crc = globals.single_file_header.crc;

			return true;
		}
#endif

		cstring tag_name_strip_path(cstring name)
		{
			YELO_ASSERT(name);

			cstring sans_path = strrchr(name, '\\');

			return sans_path != nullptr
				? sans_path + 1
				: name;
		}
	};
};