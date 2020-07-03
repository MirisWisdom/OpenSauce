/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cache/cache_files_structures.hpp>

namespace Yelo
{
	struct s_tag_instance;

	namespace TagGroups
	{
		class c_cache_file_memory_gestalt;
	};

	namespace Enums
	{
		enum target_game_platform {
			_target_game_platform_pc,	// PC
			_target_game_platform_xbox,	// Xbox

			k_number_of_target_game_platforms,
		};

		enum target_game {
			_target_game_ce,			// Halo Custom Edition
			_target_game_stubbs,		// Stubbs the Zombie

			k_number_of_target_games,
		};
	};

	namespace Cache
	{
		s_cache_header_yelo& BuildCacheFileYeloCacheHeader();

		// Ran before blam::build_cache_file_begin, preprocesses OS-specific tags for use by the runtime
		bool BuildCacheFilePreprocessTagsForRuntime();
		// Ran after blam::build_cache_file_cull_tags, culls OS-specific tag data
		bool BuildCacheFileCullTagsYelo();
		// Ran after blam::build_cache_file_predicted_resources, for OS-specific tag data
		bool BuildCacheFilePredicatedResourcesYelo();
		// Ran before blam::build_cache_file_write_header_and_compress code is executed
		bool BuildCacheFileWriteHeaderPreprocess(s_cache_header& cache_header);

		class c_cache_file_builder_base
		{
			size_t BuildCacheTagIndexTable(_Out_ size_t& predicted_tag_names_buffer_size);

		protected:
			static const unsigned k_tag_memory_alignment_bit = Flags::_alignment_32_bit;

			std::vector<datum_index> m_tag_index_to_cache_tag_index_table;
			datum_index m_current_tag_index; // index of the tag currently being processed (streamed, etc)

			c_cache_file_builder_base();

		public:
			virtual ~c_cache_file_builder_base();

		protected:
			virtual bool TagInstanceNotSuitableForCache(const s_tag_instance& instance);
			void IdentifyCacheBoundTags();

		};

		class c_cache_file_builder : public c_cache_file_builder_base
		{
			struct s_cache_info {
				TagGroups::c_cache_file_memory_gestalt* memory_gestalt;

				uintptr_t virtual_base_address;
				byte* memory_buffer;
				size_t memory_buffer_size;
				byte* memory_buffer_cursor;

				s_cache_tag_header* tag_header;			// pointer into [memory_buffer]
				s_cache_tag_instance* tag_instances;	// pointer into [memory_buffer]
				char* tag_names_buffer;					// pointer into [memory_buffer]
				size_t tag_names_buffer_size;

				size_t largest_bsp_size;
				s_cache_header header;

				s_cache_info();
				~s_cache_info();
			}m_cache;

			void StreamTagBlockToBuffer(const tag_block* block);
			void StreamTagToBuffer(datum_index tag_index);

		public:
			c_cache_file_builder();
			virtual ~c_cache_file_builder() override;
		};
	};
};
#endif