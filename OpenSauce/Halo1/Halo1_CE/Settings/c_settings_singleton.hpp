/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/c_configuration_singleton.hpp>
#include "Settings/YeloSettings.hpp"

namespace Yelo
{
	namespace Settings
	{
		template<typename Container, typename Singleton>
		class c_settings_singleton
			: public Configuration::c_configuration_singleton<Container, Singleton>
		{
		public:
			static void Register()
			{
				RegisterConfigurationContainer(Instance().GetPtr(),
					[](){ Instance().PreLoad(); },
					[](){ Instance().PostLoad(); },
					[](){ Instance().PreSave(); },
					[](){ Instance().PostSave(); }
				);
			}

			static void Unregister()
			{
				UnregisterConfigurationContainer(Instance().GetPtr());
			}

			virtual void PreLoad() { }
			virtual void PostLoad() { }
			virtual void PreSave() { }
			virtual void PostSave() { }
		};
	};
};