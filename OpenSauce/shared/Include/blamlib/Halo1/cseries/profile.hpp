/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		// Debug builds of the engine support this many different profiling sections during execution
		// If the code where a section is defined is never reached or executed, it won't be counted towards this limit
		// Number of profile_enter_private refereneces:
		//	Guerilla:	64
		//	Tool:		64
		//	Sapien:		84
		// This leaves the Yelo codebase with (MAXIMUM - NUMBER OF REFS) of usable profiling sections (if we want)
		// However, not all of these sections will ever be reached. Eg, the "render_objects" code, and thus section,
		// in Guerilla will never be reached, meaning we have more like (256 - 63) profiling sections available
		//
		// For HaloPC, GBX seemed to have added profile_service.c. At first glance, it looks to be unbounded,
		// and would appear to provide more interesting details (should you use the -codeprofile cmdline argument)
		enum {
			k_maximum_profile_sections = 256, // MAXIMUM_PROFILE_SECTIONS
		};
	};

	namespace Debug
	{
		struct s_profile_section
		{
			cstring name;
			int32 index;
			bool active;
			PAD8; // alignment
			int16 stack_depth;

			PAD32; // alignment
			UNKNOWN_TYPE(int64); // ticks
			UNKNOWN_TYPE(int32); // count
			PAD32; // alignment
			UNKNOWN_TYPE(int64); // ticks
			byte field_28 [sizeof(int32) * 120]; // counts
			byte field_208[sizeof(int64) * 120]; // ticks
			UNKNOWN_TYPE(int32); // element count for the above arrays

			UNKNOWN_TYPE(int32); // count
			UNKNOWN_TYPE(int64); // ticks

			UNKNOWN_TYPE(int32); // count
			PAD32; // alignment
			UNKNOWN_TYPE(int64); // ticks

			UNKNOWN_TYPE(int32); // count
			PAD32; // alignment
			UNKNOWN_TYPE(int64); // ticks
		}; BOOST_STATIC_ASSERT( sizeof(s_profile_section) == 0x5F8 ); // 0x730

		class c_profile_section_scope
		{
			s_profile_section& m_section;

		public:
			c_profile_section_scope(s_profile_section& section);
			~c_profile_section_scope();
		};
	};

	namespace blam
	{
		bool profiling_enabled(); // interfaces to a global in the engine

		void PLATFORM_API profile_enter_private(Debug::s_profile_section& section);
		void PLATFORM_API profile_exit_private(Debug::s_profile_section& section);
	};
};

// implementation detail, don't use directly
#define blam__profile_section_cname(section_name)							\
	profile_section##section_name

// implementation detail, don't use directly
#define blam__profile_section_define(section_name)							\
	static Yelo::Debug::s_profile_section									\
		blam__profile_section_cname(section_name) = {						\
		#section_name, NONE, true											\
	};

#define blam__profile_enter(section_name)									\
	blam__profile_section_define(section_name);								\
	if (Yelo::blam::profiling_enabled() &&									\
		blam__profile_section_cname(section_name).active)					\
	{																		\
		Yelo::blam::profile_enter_private(									\
			blam__profile_section_cname(section_name) );					\
	}

#define blam__profile_exit(section_name)									\
	if (Yelo::blam::profiling_enabled() &&									\
		blam__profile_section_cname(section_name).active)					\
	{																		\
		Yelo::blam::profile_exit_private(									\
			blam__profile_section_cname(section_name) );					\
	}

#define blam__profile_begin(section_name)	{								\
	blam__profile_section_define(section_name);								\
	Yelo::Debug::c_profile_section_scope									\
		profile_section_scope##section_name(								\
			blam__profile_section_cname(section_name) );					\

#define blam__profile_end()					}