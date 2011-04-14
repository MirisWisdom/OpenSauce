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
		class c_version_display_manager : public c_version_display_manager_base
		{
		public:
			/*!
			 * \brief
			 * Initialises the classes variables and allocates any extra memory (if needed).
			 */
			void		Initialize()
			{
				memset(this, 0, sizeof(*this));
			}
			/*!
			 * \brief
			 * Deletes the memory allocated by Initialize (if any)
			 */
			void		Dispose()
			{
			}

			void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters) {}
			void		OnLostDevice() {}
			void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters) {}
			void		Render() {}
			void		Release() {}

			void		Update(real delta_time) {}

			void		SetCurrentVersionString(wcstring version_string)	{ SetCurrentVersionStringImpl(version_string); }
			void		SetAvailableVersionString(wcstring version_string)	{ SetAvailableVersionStringImpl(version_string); }

			void		StartUpdateDisplay(const real time) {}
			void		ResetDisplay() {}

			static c_version_display_manager g_instance;
		};
	}; };
};
#endif