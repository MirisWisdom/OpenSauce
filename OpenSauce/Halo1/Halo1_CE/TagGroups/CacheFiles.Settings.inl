/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

class c_settings_container
	: public Configuration::c_configuration_container
{
public:
	Configuration::c_configuration_value<bool> m_check_yelo_files_first;
	Configuration::c_configuration_value<std::string> m_mainmenu_scenario;

	c_settings_container()
		: Configuration::c_configuration_container("CacheFiles")
		, m_check_yelo_files_first("CheckYeloFilesFirst", true)
		, m_mainmenu_scenario("MainMenuScenario", "")
	{ }
			
protected:
	const std::vector<i_configuration_value* const> GetMembers() final override
	{
		return std::vector<i_configuration_value* const>
		{
			&m_check_yelo_files_first,
			&m_mainmenu_scenario
		};
	}
};

class c_settings_cache
	: public Settings::c_settings_singleton<c_settings_container, c_settings_cache>
{
public:
	void PostLoad() final override
	{
#if PLATFORM_IS_USER
		g_yelo_settings.InitializeMainmenuOverride(Get().m_mainmenu_scenario);
#endif
	}
};