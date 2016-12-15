/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/VersionCheckClient.hpp"

#if PLATFORM_IS_USER && defined(YELO_VERSION_CHECK_ENABLE)
#include <blamlib/Halo1/main/main_structures.hpp>

#include "Networking/HTTP/HTTPClient.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		/////////////////////////////////////////
		//c_version_check_manager_user
		/////////////////////////////////////////
		
		/////////////////////////////////////////
		//non-static functions
		/*!
		 * \brief
		 * Checks for a new version when the Main Menu is loaded.
		 * 
		 * If the game is loading the main menu, then check for the available online version.
		 * The update check will only occur once per day.
		 * If the update does not complete before a new map is loaded, the http requests
		 * are canceled, and the update is re-run the next time the game is in the main menu.
		 * If the update check has been completed, and a new version is available,
		 * the display manager will be set to display it's animation for 20 seconds, each time
		 * the main menu is loaded.
		 */
		void		c_version_check_manager_user::InitializeForNewMap()
		{
			// find out if we are on the main menu
			m_states_user.is_in_menu = GameState::MainGlobals()->map.main_menu_scenario_loaded;

			// if we are on the main menu and the update check hasn't been done yet, set it going
			// otherwise if we are loading a new map and the version check is not complete, cancel 
			// the requests
			if(m_states_user.is_in_menu && !m_states.checked_today)
				CheckForUpdates();
			else if(m_states.is_request_in_progress)
			{
				for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
					m_xml_sources[i].Stop();
				m_states.is_request_in_progress = false;
			}
		}
	}; };
};
#endif