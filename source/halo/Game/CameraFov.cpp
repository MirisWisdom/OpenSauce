/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <Common/Precompile.hpp>
#include <Game/CameraFov.hpp>

#if PLATFORM_IS_USER

#include <blamlib/cache/cache_files_globals.hpp>
#include <blamlib/game/player_control.hpp>
#include <Game/Camera.hpp>
#include <Pointers/Game.Fov.hpp>
#include <TagGroups/CacheFiles.hpp>
#include <yelolib/cache/shared_cache_files.hpp>
#include <yelolib/memory/naked_func_writer.hpp>

namespace Yelo
{
	namespace Fov
	{
		static real g_fov_scale = 1.0f;

		c_settings_container::c_settings_container()
			: c_configuration_container("Camera")
			, m_field_of_view("FieldOfView", 70.0f)
			, m_ignore_fov_change_in_cinematics("IgnoreFOVChangeInCinematics", true)
			, m_ignore_fov_change_in_main_menu("IgnoreFOVChangeInMainMenu", true) { }

		std::vector<Configuration::i_configuration_value*> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value*> {
				&m_field_of_view,
				&m_ignore_fov_change_in_cinematics,
				&m_ignore_fov_change_in_main_menu
			};
		}

		void c_settings_fov::PostLoad()
		{
			auto base_fov = __min(Get().m_field_of_view, 160.0f);
			base_fov = __max(base_fov, 40.0f);

			g_fov_scale = base_fov / 70.0f;
		}

		void c_settings_fov::PreSave()
		{
			Get().m_field_of_view = 70.0f * g_fov_scale;
		}

		static void PLATFORM_API observer_update_command_hook()
		{
			if (Players::PlayerControlGlobals()->local_players[0].zoom_level > -1)
			{
				return;
			}

			const auto in_main_menu = Cache::CacheFileGlobals()->cache_header.cache_type == Enums::_shared_cache_type_main_menu;
			if (in_main_menu && c_settings_fov::Instance()->m_ignore_fov_change_in_main_menu)
			{
				return;
			}

			if (!in_main_menu
				&& Camera::DirectorScripting()->initialized
				&& c_settings_fov::Instance()->m_ignore_fov_change_in_cinematics)
			{
				return;
			}

			const auto player_fov = Camera::Observer()->command_update->fov;
			if (player_fov != 0.0f)
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

		void SetFieldOfView(
			const real fov)
		{
			g_fov_scale = fov / 70.0f;
		}

		void ResetFieldOfView()
		{
			g_fov_scale = 1.0f;
		}
	}
}
#endif