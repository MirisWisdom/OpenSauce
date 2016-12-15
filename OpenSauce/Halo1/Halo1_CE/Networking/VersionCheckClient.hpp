/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_USER && defined(YELO_VERSION_CHECK_ENABLE)
#include "Networking/VersionCheck.hpp"

namespace Yelo
{
	class TextBlock;

	namespace Networking { namespace VersionCheck
	{
		/*!
		 * \brief
		 * Handles checking for a new version of OS on the client setup.
		 * 
		 * This class will check for updates when the main menu is loaded
		 * and displays the result to the user.
		 */
		class c_version_check_manager_user : public c_version_check_manager_base
		{
			struct {
				/// Is the game currently on the main menu
				bool is_in_menu;
				PAD24;
			}m_states_user;

		public:
			void			InitializeForNewMap();
		};
	}; };
};
#endif