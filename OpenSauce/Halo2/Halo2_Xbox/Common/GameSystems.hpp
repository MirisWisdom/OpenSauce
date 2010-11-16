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
#pragma once

namespace Yelo
{
	namespace Main
	{
		struct s_project_component
		{
			// Called from Main on startup
			proc_initialize				Initialize;
			// Called from Main on shutdown
			proc_dispose				Dispose;
		};

		//	[out_components] - Returns a list of components, or NULL if no components exist
		//	returns: Number of components in [out_components] minus 1, or just 0 if no components exist
		int32 GetProjectComponents(s_project_component*& out_components);
	};
};