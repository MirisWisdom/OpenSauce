/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/c_configuration_singleton.hpp>
#include <YeloLib/open_sauce/settings/c_settings_manager.hpp>

namespace Yelo
{
	namespace Settings
	{
		template<typename Container, typename Singleton>
		class c_settings_singleton
			: public Configuration::c_configuration_singleton<Container, Singleton>
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Registers this with the target settings manager. </summary>
			///
			/// <param name="manager">	[in] The settings manager to use. </param>
			void RegisterInstance(c_settings_manager& manager)
			{
				manager.RegisterConfigurationContainer(Instance().Get(),
					[](){ Instance().PreLoad(); },
					[](){ Instance().PostLoad(); },
					[](){ Instance().PreSave(); },
					[](){ Instance().PostSave(); }
				);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Deregisters this object from the target settings manager. </summary>
			///
			/// <param name="manager">	[in] The settings manager to use. </param>
			void UnregisterInstance(c_settings_manager& manager)
			{
				manager.UnregisterConfigurationContainer(Instance().Get());
			}

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Registers this with the target settings manager. </summary>
			///
			/// <param name="manager">	[in] The settings manager to use. </param>
			static void Register(c_settings_manager& manager)
			{
				Instance().RegisterInstance(manager);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Deregisters this object from the target settings manager. </summary>
			///
			/// <param name="manager">	[in] The settings manager to use. </param>
			static void Unregister(c_settings_manager& manager)
			{
				Instance().UnregisterInstance(manager);
			}

			/// <summary>	Called before the settings are loaded. </summary>
			virtual void PreLoad() { }
			/// <summary>	Called after the settings are loaded. </summary>
			virtual void PostLoad() { }
			/// <summary>	Called before the settings are saved. </summary>
			virtual void PreSave() { }
			/// <summary>	Called after the settings are saved. </summary>
			virtual void PostSave() { }
		};
	};
};