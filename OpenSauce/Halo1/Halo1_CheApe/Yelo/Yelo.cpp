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


	static Yelo::tag_group_definition* tag_db_definition = NULL;
	void Initialize()
	{
		c_memory_fixups::Initialize();
		Scripting::Initialize();
		// Call the *engine's* initializer
		TagGroups::Initialize();

		TagGroups::YeloDefinitionsInitialize();
		if(TagGroups::_yelo_definition_globals.initialized)
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Initialize();

		tag_db_definition = Yelo::tag_group_get(TagGroups::s_tag_database::k_group_tag);
		if(tag_db_definition)
		{
			Yelo::tag_block_definition* tag_database_entry_block_def = tag_db_definition->definition->fields[0].Definition<Yelo::tag_block_definition>();
			tag_database_entry_block_def->format_proc = &TagGroups::tag_database_entry_block_format;
		}
	}

	void Dispose()
	{
		if(TagGroups::_yelo_definition_globals.initialized)
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Dispose();

		TagGroups::YeloDefinitionsDispose();
		Scripting::Dispose();
		c_memory_fixups::Dispose();
	}
};