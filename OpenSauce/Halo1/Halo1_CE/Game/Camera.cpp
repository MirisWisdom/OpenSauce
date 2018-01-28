/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Camera.hpp"
#include <Game/CameraFov.hpp>

#if PLATFORM_IS_USER

#include <Pointers/Game.Camera.hpp>
#include <blamlib/cache/cache_files_globals.hpp>
#include <blamlib/camera/director_structures.hpp>
#include <blamlib/objects/objects.hpp>
#include <blamlib/units/unit_structures.hpp>
#include <Rasterizer/Rasterizer.hpp>
#include <Settings/Settings.hpp>
#include <TagGroups/CacheFiles.hpp>
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
			Memory::WriteRelativeCall(&DirectorChooseGamePerspectiveHook, GET_FUNC_VPTR(DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK), true);

			Fov::Initialize();
		}

		void Dispose()
		{
			Fov::Dispose();
		}

		void Update() { }

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
			DumpViewState();

			return nullptr;
		}
	}
}
#endif
