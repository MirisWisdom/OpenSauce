/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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

#if PLATFORM_IS_DEDI && defined(YELO_VERSION_CHECK_ENABLE)
#include "Networking/VersionCheck.hpp"

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		class c_version_check_manager_dedi : public c_version_check_manager_base
		{
			void		Initialize();
			void		InitializeForNewMap();
			void		UpdateState();

		private:
			void		DisplayVersions(bool is_new_version);
		};
	}; };
};
#endif