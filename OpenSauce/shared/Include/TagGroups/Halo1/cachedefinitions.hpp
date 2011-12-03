/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <TagGroups/CacheDefinitions.hpp>
#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include <Blam/Halo1/project_yellow_shared_definitions.hpp>

namespace Yelo
{
	struct s_cache_header_yelo : public s_cache_header_yelo_base {
		enum {
			k_version = 1,
		};

		struct s_flags {
			word_flags uses_memory_upgrades : 1;// cache requires upgraded memory
			word_flags uses_mod_data_files : 1;	// cache relies on a set of 'mod' data files for it's resources
			word_flags is_protected : 1;		// cache has protection applied
		}flags; BOOST_STATIC_ASSERT( sizeof(s_flags) == 0x2 );

		PAD32;
		real k_memory_upgrade_increase_amount;

		struct {
			uint32 size;
			uint32 decompressed_size;
			uint32 offset;
			PAD32;

			tag_string build_string; // Build string for the CheApe tools (ie, OS HEK)
		}cheape_definitions;

		tag_string mod_name; // if the map uses a specific mod's data_files, this equals the mod prefix

		struct {
			PAD16;
			_enum stage; // see TagEnums::production_build_stage
			uint32 revision;
			time_t timestamp;

			tag_string build_string;
		}build_info; // User-defined build info

#if PLATFORM_IS_EDITOR
		void InitializeForNewMap()
		{
			memset(this, 0, sizeof(*this));

			this->signature = k_signature;
			this->version = k_version;
		}

		void InitializeBuildInfo(_enum stage = TagEnums::_production_build_stage_ship, uint32 revision = 0)
		{
			build_info.stage = stage;
			build_info.revision = revision;
			time(&build_info.timestamp);

			cstring stage_string = "";
			switch(stage)
			{
			case TagEnums::_production_build_stage_ship:	stage_string = "ship";	break;
			case TagEnums::_production_build_stage_alpha:	stage_string = "alpha";	break;
			case TagEnums::_production_build_stage_beta:	stage_string = "beta";	break;
			case TagEnums::_production_build_stage_delta:	stage_string = "delta";	break;
			case TagEnums::_production_build_stage_epsilon:	stage_string = "epsilon";	break;
			case TagEnums::_production_build_stage_release:	stage_string = "release";	break;
			}

			tm date_tm;
			localtime_s(&date_tm, &build_info.timestamp); // Convert time to struct tm form
			// ######.YY.MM.DD.HHMM.stage
			sprintf_s(build_info.build_string, "%06u." "%02i" "%02i.%02i." "%02i%02i." "%s", 
				revision, 
				date_tm.tm_year - 100, // days since 1900, and we want a number relative to 2000
				date_tm.tm_mon, date_tm.tm_mday, 
				date_tm.tm_hour, date_tm.tm_sec,
				stage_string);
		}
#endif

		// Is there no yelo header present?
		bool NoHeader() const
		{
			return signature == 0 && version == 0;
		}
		// Is the yelo header valid?
		bool IsValid() const
		{
			return !NoHeader() || (
				signature == k_signature && version == k_version && 
				k_memory_upgrade_increase_amount <= K_MEMORY_UPGRADE_INCREASE_AMOUNT
				);
		}
	};

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

		bool ValidSignatures() const		{ return header_signature == k_header_signature && footer_signature == k_footer_signature; }
		bool ValidFileSize(int32 max) const	{ return file_length >= 0 && file_length <= max; }
		bool ValidName() const				{ return strnlen_s(name, Enums::k_tag_string_length) <= Enums::k_tag_string_length; }
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
		bool MatchesGroup(tag group_tag) const
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
		uint32 crc;				// 0x20
		tag signature;			// 0x24

		// nasty hack assumption
#pragma warning(push)
#pragma warning(disable : 4200)
		s_cache_tag_instance tags[];
#pragma warning(pop)
	}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_header) == 0x28 );
};