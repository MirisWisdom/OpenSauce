/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
			void		InitializeForNewMap();
			void		UpdateVersion();
			void		TestForUpdate();

		private:
			void		DisplayVersions(bool is_new_version);
		};
	}; };
};
#endif