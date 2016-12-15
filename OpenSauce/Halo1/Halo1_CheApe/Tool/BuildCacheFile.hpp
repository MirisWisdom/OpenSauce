/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

#include "Tool/BuildCacheFile/BuildCacheFileEx.hpp"

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header;
		struct s_cache_header_yelo;
	};

	namespace Tool
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if tool is building a cache file. </summary>
		///
		/// <returns>	true if building a cache file, false if not. </returns>
		bool& is_building_cache_file();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Set's the minimum open sauce build for the current cache file. </summary>
		///
		/// <param name="major">	The major version. </param>
		/// <param name="minor">	The minor version. </param>
		/// <param name="build">	The build version. </param>
		void build_cache_file_set_minimum_os_build(const byte major, const byte minor, const uint16 build);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>We replace the stock build_cache_file_for_scenario implementation with our own to turn off things
		/// 	compliant with the stock game, like custom script definitions.
		/// </summary>
		///
		/// <param name="arguments">	. </param>
		void PLATFORM_API build_cache_file_for_scenario_stock_override(char* arguments[]);

		void PLATFORM_API build_cache_file_for_scenario_new(char* arguments[]);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>Code to execute before we commit the yelo header and the tool's code finishes building the cache.
		/// </summary>
		///
		/// <remarks>This function is called before we commit the yelo header to the resulting cache file's header. The
		/// 	implementing code which calls this (BuildCacheFileMemoryUpgrades.cpp, InterceptorEnd) calls the tool's
		/// 	build_cache_file_end function after this function completes.
		/// </remarks>
		///
		/// <param name="header">	[in,out] If non-null, the header. </param>
		/// <param name="ych">   	[in,out] Yelo's cache header data, which will be committed to the actual header later. </param>
		void build_cache_file_end_preprocess(Cache::s_cache_header* header, Cache::s_cache_header_yelo& ych);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Code to execute before the tool's code for building the cache begins. </summary>
		///
		/// <remarks>Since this code is executed before any real cache building code begins, we can do things like create stub
		/// 	tags, in the event the user doesn't define them. Or anything else we need to do/change in tags (e.g., rename)
		/// 	before they're committed to cache memory.
		/// </remarks>
		///
		/// <param name="scenario_name">	Tag name of the scenario which is being built into a cache. </param>
		void build_cache_file_begin_preprocess(cstring scenario_name);
	};
};
#endif