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
#pragma once

#include <Blam/Halo1/BlamScriptingDefinitions.hpp>
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		struct scripting_block;
	};

	namespace Enums
	{
		enum {
			_hs_function_profile_load = 0x20A,

			_hs_function_profile_load_call_offset = 0x1F, // offset of the implementation call in the evaluation function (at least in sapien)
		};
	};

	namespace Scripting
	{
		void Initialize();
		void Dispose();

		hs_function_definition* GetFunctionDefintion(int16 function_index);

		// Copy the scripting definitions CheApe loaded into the tool code from the memory map
		// [only_internals] - Copy definitions that are part of the stock OpenSauce source code 
		// only, else copy only the user's definitions
		void ScriptingBlockAddDefinitions(
			TAG_TBLOCK(& script_block, TagGroups::scripting_block), bool only_internals);
	};
};