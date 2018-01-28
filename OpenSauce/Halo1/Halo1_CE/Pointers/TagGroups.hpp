/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.TagGroups.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.TagGroups.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.TagGroups.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.TagGroups.inl"
#endif

#include <blamlib/cache/cache_files_structures.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		ENGINE_DPTR(Cache::s_cache_tag_instance, tag_instances, DUO_PTR(K_TAG_INSTANCES));
	}
}
