/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Interface/Controls.hpp"
#include "Game/Players.hpp"
#include "TagGroups/CacheFiles.hpp"

#include <YeloLib/Halo1/cache/shared_cache_files.hpp>
#include <BlamLib/Halo1/cache/cache_files.hpp>
#include <BlamLib/Halo1/cache/cache_files_globals.hpp>

namespace Yelo
{
	namespace Fov
	{
		static real g_fov_scale = 1.0f;

#pragma region Settings
		c_settings_container::c_settings_container()
			: Configuration::c_configuration_container("Camera")
			, m_field_of_view("FieldOfView", 70.0f)
			, m_ignore_fov_change_in_cinematics("IgnoreFOVChangeInCinematics", true)
			, m_ignore_fov_change_in_main_menu("IgnoreFOVChangeInMainMenu", true)
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value* const> { &m_field_of_view, &m_ignore_fov_change_in_cinematics, &m_ignore_fov_change_in_main_menu };
		}

		void c_settings_fov::PostLoad()
		{
			real base_fov = __min(Get().m_field_of_view, 160.0f);
			base_fov =		__max(base_fov, 40.0f);

			g_fov_scale = base_fov / 70.0f;
		}

		void c_settings_fov::PreSave()
		{
			Get().m_field_of_view = 70.0f * g_fov_scale;
		}
#pragma endregion

		static void PLATFORM_API observer_update_command_hook()
		{
			if (Players::PlayerControlGlobals()->local_players[0].zoom_level > -1)
			{
				return;
			}

			bool in_main_menu = Yelo::Cache::CacheFileGlobals()->cache_header.cache_type == Enums::_shared_cache_type_main_menu;
			if(in_main_menu && c_settings_fov::Instance()->m_ignore_fov_change_in_main_menu)
			{
				return;
			}

			if(!in_main_menu
				&& Camera::DirectorScripting()->initialized
				&& c_settings_fov::Instance()->m_ignore_fov_change_in_cinematics)
			{
				return;
			}

			real player_fov = Camera::Observer()->command_update->fov;
			if(player_fov != 0.0f)
			{
				Camera::Observer()->command.fov = player_fov * g_fov_scale;
			}
		}

		void Initialize()
		{
			// Overwrite the 'retn' at the observer_update_command with a jmp to our hook code
			Memory::WriteRelativeJmp(observer_update_command_hook, GET_FUNC_VPTR(OBSERVER_UPDATE_COMMAND_HOOK), true);

			// Disabled the max fov check
			GET_PTR(MAX_FOV_CHECK_JMP) = Enums::_x86_opcode_jmp_short;

			c_settings_fov::Register(Settings::Manager());
		}

		void Dispose()
		{
			c_settings_fov::Unregister(Settings::Manager());
		}

		real GetFieldOfView()
		{
			 return 70.0f * g_fov_scale;
		}

		void SetFieldOfView(const real fov)
		{
			g_fov_scale = fov / 70.0f;
		}

		void ResetFieldOfView()
		{
			g_fov_scale = 1.0f;
		}
	};
};