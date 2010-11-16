/*
    Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition
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
#include "Common/GameSystems.hpp"

#include "Game/Main.hpp"
#include "Memory/Config.hpp"
#include "Interface/Input.hpp"
#include "Cache/Cache.hpp"
#include "AI/AI.hpp"
#include "Game/Players.hpp"
#include "Networking/Statistics.hpp"
#include "Game/Director.hpp"

//#define API_YELO_NO_PROJECT_COMPONENTS


namespace Yelo
{
	namespace Main
	{
		int32 GetProjectComponents(s_project_component*& out_components)
		{
			out_components = NULL;
			int32 components_count = NONE;

#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_component k_components[] = {
				#include "Common/GameSystems.ProjComponents.inl"
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1;
#endif

			return components_count;
		}
	};
};


#undef API_YELO_NO_PROJECT_COMPONENTS