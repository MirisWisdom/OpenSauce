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
#include "Common/Precompile.hpp"
#include "Networking/VersionCheckDedi.hpp"

#if PLATFORM_IS_DEDI && defined(YELO_VERSION_CHECK_ENABLE)
#include "Game/EngineFunctions.hpp"

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		/*!
		* \brief
		* Runs the update check process when the server is run.
		*
		* Runs the update check process when the server is run.
		*/
		void		c_version_check_manager_dedi::Initialize()
		{
			c_version_check_manager_base::Initialize();

			// check for updates using blocking
			if(!m_states.checked_today)
			{
				Engine::Console::TerminalPrint("Checking for updates...");
				CheckForUpdates(true);
			}
		}
		/*!
		* \brief
		* Runs the update check if needed.
		*
		* Updates the date condition to see if a check has been performed today.
		* If it hasn't, an update check will be run. This check is not blocking,
		* so that normal running is not paused. If there is a new version available
		* The versions will be printed to the console when each map is loaded.
		*
		* \remarks
		* Since servers can be running for longer than 24 hours it's necessary
		* for the server to automatically check for updates at some point. Currently
		* this happens when a new map is loaded, however, if the server is running a
		* gametype that can run for a VERY long time, updates will be missed.
		*/
		void		c_version_check_manager_dedi::InitializeForNewMap()
		{
			// update the date state
			UpdateDateState();

			// if the version hasn't been checked today, check it without blocking
			// although, considering this is map load time a little delay for
			// getting the update might be ok
			if(m_states.is_request_in_progress) return;

			if(!m_states.checked_today)
				CheckForUpdates(false);
			else
				DisplayVersions(m_states.is_new_version);
		}
		/*!
		* \brief
		* Prints the available version if a new version is available.
		*
		* Prints the available version if a new version is available.
		*/
		void		c_version_check_manager_dedi::UpdateState()
		{
			c_version_check_manager_base::UpdateState();

			// if a new version is available, print it to the console
			DisplayVersions(m_states.is_new_version);
		}
		/*!
		* \brief
		* Prints the current and available versions to the console.
		*
		* Prints the current and available versions to the console.
		*/
		void		c_version_check_manager_dedi::DisplayVersions(bool is_new_version)
		{
			Engine::Console::TerminalPrint("");

			// if a new version is available display the versions
			if(is_new_version)
			{
				// print the current and available versions to the console
				// need to see how the console looks though to make sure we dont
				// mess up anything, could make it colourful too
				Engine::Console::TerminalPrint("New version of OpenSauce available!");
				Engine::Console::TerminalPrintF("Current version:\t\t%i.%i.%i", 
					m_current_version.m_major, m_current_version.m_minor, m_current_version.m_build);
				Engine::Console::TerminalPrintF("Available version:\t\t%i.%i.%i", 
					m_available_version.m_major, m_available_version.m_minor, m_available_version.m_build);
			}
			else
			{
				Engine::Console::TerminalPrint("No updates available");
			}

			Engine::Console::TerminalPrint("");
		}
	}; };
};
#endif