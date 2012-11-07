/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/cache/cache_files_yelo_base.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header_yelo : public s_cache_header_yelo_base {
			enum {
				k_version = 1,
			};

			struct s_flags {
				word_flags uses_memory_upgrades : 1;// cache requires upgraded memory
				word_flags uses_mod_data_files : 1;	// cache relies on a set of 'mod' data files for it's resources
				word_flags is_protected : 1;		// cache has protection applied
				word_flags uses_game_state_upgrades : 1; // cache has tag data that either requires or needs OS-modified game state memory in order to (fully) function
			}flags; BOOST_STATIC_ASSERT( sizeof(s_flags) == 0x2 );

			struct {
				byte project_yellow;
				byte project_yellow_globals;
				PAD16;
			}tag_versioning; // versions of core tags
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

				// This actually isn't specific to CheApe, but the OS version values used when CheApe was built
				struct {
					byte maj;
					byte min;
					uint16 build;
				}cheape;
			}build_info; // User-defined build info

#if PLATFORM_IS_EDITOR
			void InitializeForNewMap();
			void InitializeBuildInfo(_enum stage, uint32 revision);
#endif

			// Is there a yelo header present?
			bool HasHeader() const;
			bool TagVersioningIsValid() const;
			// Is the yelo header valid?
			bool IsValid() const;

			bool BuiltWithOlderTools() const;
			bool BuiltWithNewerTools() const;
		};
	};
};