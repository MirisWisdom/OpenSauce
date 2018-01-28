/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Camera.hpp"

#if PLATFORM_IS_USER

#include <Pointers/Game.Camera.hpp>
#include <blamlib/cache/cache_files_globals.hpp>
#include <blamlib/camera/director_structures.hpp>
#include <blamlib/game/player_control.hpp>
#include <blamlib/objects/objects.hpp>
#include <blamlib/units/unit_structures.hpp>
#include <Pointers/Game.Fov.hpp>
#include <Rasterizer/Rasterizer.hpp>
#include <Settings/Settings.hpp>
#include <TagGroups/CacheFiles.hpp>
#include <yelolib/cache/shared_cache_files.hpp>
#include <yelolib/units/units_yelo.hpp>

namespace Yelo
{
	namespace Camera
	{
		s_observer* Observer() PTR_IMP_GET2(observers);
		s_camera_script_globals_data* CameraScriptGlobals() PTR_IMP_GET2(camera_script_globals);
		s_director_scripting_data* DirectorScripting() DPTR_IMP_GET(director_scripting);
		s_cinematic_globals_data* CinematicGlobals() DPTR_IMP_GET(cinematic_globals);
		s_director_data* GlobalDirector() PTR_IMP_GET2(global_director);

		int16 PLATFORM_API DirectorDesiredPerspective(
			const datum_index unit_index,
			Enums::game_perspective& perspective)
		{
			if (!unit_index.IsNull())
			{
				auto& unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);

				if (unit_datum.unit.animation.state == Enums::_unit_animation_state_yelo_unit_mounted)
				{
					auto* unit_extension_definition = Objects::GetUnitExtensionDefinition(unit_index);
					if (unit_extension_definition && (unit_extension_definition->mounted_state.count == 1))
					{
						auto& mounted_state_definition = unit_extension_definition->mounted_state[0];

						if (TEST_FLAG(mounted_state_definition.flags, Flags::_unit_mounted_state_flags_third_person_camera))
						{
							perspective = Enums::_game_perspective_following_camera;
							return Enums::_game_perspective_following_camera;
						}
					}
				}
			}

			return blam::director_desired_perspective(unit_index, perspective);
		}

		API_FUNC_NAKED int16 PLATFORM_API DirectorChooseGamePerspectiveHook(
			Enums::game_perspective& perspective)
		{
			API_FUNC_NAKED_START()
				push perspective
				push ecx
				call DirectorDesiredPerspective
				add esp, 8
				pop ebp
			API_FUNC_NAKED_END_()
		}

		void Initialize()
		{
			#pragma region old third person shit
			#if 0 // enable for old 3rd person camera hack
			static byte asm_change_short1[] = {0x01, 0x00};
			static byte asm_change_short0[] = {0x00, 0x00};
			static byte asm_change_long0[] = {0x00, 0x00, 0x00, 0x00};

			Memory::CreateHookRelativeCall(&Camera::UpdateFor3rd, GET_FUNC_VPTR(OBSERVER_UPDATE_COMMAND_HOOK), Enums::_x86_opcode_retn);
			Memory::WriteMemory(GET_FUNC_VPTR(DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET), asm_change_short1, 2);
			Memory::WriteMemory(GET_FUNC_VPTR(HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD), asm_change_short0, 2);
			Memory::WriteMemory(GET_FUNC_VPTR(HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG), asm_change_long0, 4);
			#endif
			#pragma endregion

			Memory::WriteRelativeCall(&DirectorChooseGamePerspectiveHook, GET_FUNC_VPTR(DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK), true);

			Fov::Initialize();
		}

		void Dispose()
		{
			Fov::Dispose();
		}

		void Update() { }

		#pragma region old third person shit
		#if 0 // enable for old 3rd person camera hack
		void PLATFORM_API UpdateFor3rd()
		{
			if (*((byte *)0x400001EE)!=0x5C && *((int16 *)0x64741C)==0 && *((int16 *)0x64741E)==1)
			//if(GameState::GameGlobals()->data[0x1A] != 0x5C && 
		//	Camera::Observer()->timer._unknown4 == 0 &&
			//	Camera::Observer()->timer._unknown4_pad[1] == 1)
			{
				Camera::ObserverGlobals()->command.offset.y = 0.4f;
				Camera::ObserverGlobals()->command.offset.z = 0.2f;
				Camera::ObserverGlobals()->command.depth = 2.0f;
			}
		}
		#endif
		#pragma endregion

		void DumpViewState(
			cstring name)
		{
			if (!name)
				name = "view_state.bin";

			FILE* f = Settings::CreateReport(name, false, false);

			if (f != nullptr)
			{
				fwrite(&Render::RenderGlobals()->camera, sizeof(Render::s_render_camera), 1, f);
				fwrite(&Render::RenderGlobals()->frustum, sizeof(Render::s_render_frustum), 1, f);
				fclose(f);
			}
		}

		void* DumpViewStateEvaluate(
			void** arguments)
		{
			//YELO_DEBUG(Settings::ReportsPath(), true);
			// 			struct s_arguments {
			// 				cstring name;
			// 			}* args = CAST_PTR(s_arguments*, arguments);
			// 
			// 			Engine::Console::Print(args->name);
			DumpViewState();

			return nullptr;
		}
	}
}

namespace Yelo
{
	namespace Fov
	{
		static real g_fov_scale = 1.0f;

		#pragma region Settings
		c_settings_container::c_settings_container()
			: c_configuration_container("Camera")
			, m_field_of_view("FieldOfView", 70.0f)
			, m_ignore_fov_change_in_cinematics("IgnoreFOVChangeInCinematics", true)
			, m_ignore_fov_change_in_main_menu("IgnoreFOVChangeInMainMenu", true) { }

		std::vector<Configuration::i_configuration_value*> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value*> {&m_field_of_view, &m_ignore_fov_change_in_cinematics, &m_ignore_fov_change_in_main_menu};
		}

		void c_settings_fov::PostLoad()
		{
			real base_fov = __min(Get().m_field_of_view, 160.0f);
			base_fov = __max(base_fov, 40.0f);

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

			bool in_main_menu = Cache::CacheFileGlobals()->cache_header.cache_type == Enums::_shared_cache_type_main_menu;
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

			real player_fov = Camera::Observer()->command_update->fov;
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
