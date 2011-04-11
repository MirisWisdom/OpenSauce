/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
#include "Common/Precompile.hpp"
#include "CheApeInterface.hpp"

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/tag_database_definitions.hpp"

#include "Engine/Scripting.hpp"
#include "Yelo/MemoryFixups.hpp"

namespace Yelo
{
	// Maybe we should just rename the individual namespaces 
	// to just 'HEK' ?
	namespace PLATFORM_VALUE(Guerilla, Tool, Sapien)
	{
		void Initialize();
		void Dispose();
	};


	static void InitializeForTagFilesOpen()
	{
		// Call the *engine's* initializer
		TagGroups::Initialize();

		TagGroups::YeloDefinitionsInitialize();
		if(TagGroups::_yelo_definition_globals.initialized)
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Initialize();
	}
	static void OverrideTagFilesOpen()
	{
		static uint32 TAG_FILES_OPEN_CALLS[] = {
#if PLATFORM_ID == PLATFORM_TOOL
			0x42DC10
#elif PLATFORM_ID == PLATFORM_GUERILLA
			0x421C30,
			0x41D4F5
#elif PLATFORM_ID == PLATFORM_SAPIEN
			0x418A50,
			0x4141E5	// for no render cases
#endif
		};

		for(int32 x = 0; x < NUMBEROF(TAG_FILES_OPEN_CALLS); x++)
			Yelo::Memory::WriteRelativeCall(InitializeForTagFilesOpen, CAST_PTR(void*,TAG_FILES_OPEN_CALLS[x]));
	}

	void Initialize()
	{
		c_memory_fixups::Initialize();
		Scripting::Initialize();

		OverrideTagFilesOpen();
	}

	void Dispose()
	{
		// Initializer called in InitializeForTagFilesOpen
		if(TagGroups::_yelo_definition_globals.initialized)
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Dispose();

		TagGroups::YeloDefinitionsDispose();
		Scripting::Dispose();
		c_memory_fixups::Dispose();
	}
};