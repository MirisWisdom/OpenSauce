/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/VersionCheckDedi.hpp"

#if PLATFORM_IS_DEDI && defined(YELO_VERSION_CHECK_ENABLE)
#include <blamlib/Halo1/main/console.hpp>

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
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
				CheckForUpdates();
			else
				DisplayVersions(m_states.is_new_version_available);
		}

		void		c_version_check_manager_dedi::TestForUpdate()
		{
			c_version_check_manager_base::TestForUpdate();

			// check for updates using blocking
			if(!m_states.checked_today)
			{
				blam::console_printf(false, "Checking for updates...");
				CheckForUpdates();
			}
		}

		/*!
		* \brief
		* Prints the available version if a new version is available.
		*
		* Prints the available version if a new version is available.
		*/
		void		c_version_check_manager_dedi::UpdateVersion()
		{
			c_version_check_manager_base::UpdateVersion();

			// if a new version is available, print it to the console
			DisplayVersions(m_states.is_new_version_available);
		}
		/*!
		* \brief
		* Prints the current and available versions to the console.
		*
		* Prints the current and available versions to the console.
		*/
		void		c_version_check_manager_dedi::DisplayVersions(bool is_new_version)
		{
			blam::console_printf(false, "");

			// if a new version is available display the versions
			if(is_new_version)
			{
				// print the current and available versions to the console
				// need to see how the console looks though to make sure we dont
				// mess up anything, could make it colourful too
				blam::console_printf(false, "New version of OpenSauce available!");
				blam::console_printf(false, "Current version:\t\t%i.%i.%i (%s)",
					m_current_version.m_major,
					m_current_version.m_minor,
					m_current_version.m_build,
					m_current_version.m_build_date);

				if(is_null_or_empty(m_available_version.m_build_date))
				{
					blam::console_printf(false, "Available version:\t\t%i.%i.%i",
						m_available_version.m_major,
						m_available_version.m_minor,
						m_available_version.m_build);
				}
				else
				{
					blam::console_printf(false, "Available version:\t\t%i.%i.%i (%s)",
						m_available_version.m_major,
						m_available_version.m_minor,
						m_available_version.m_build,
						m_available_version.m_build_date);
				}
			}
			else
			{
				blam::console_printf(false, "No updates available");
			}

			blam::console_printf(false, "");
		}
	}; };
};
#endif