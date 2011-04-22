/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Game/Camera.hpp"

#if PLATFORM_IS_USER
#include "Common/YeloSettings.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/EngineFunctions.hpp"

#include "Rasterizer/Rasterizer.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_CAMERA
#include "Memory/_EngineLayout.inl"

	namespace Camera
	{
		s_observer* Observer()									PTR_IMP_GET2(observers);
		s_camera_script_globals_data* CameraScriptGlobals()		PTR_IMP_GET2(camera_script_globals);
		s_director_scripting_data* DirectorScripting()			DPTR_IMP_GET(director_scripting);
		s_cinematic_globals_data* CinematicGlobals()			DPTR_IMP_GET(cinematic_globals);
		s_director_data* GlobalDirector()						PTR_IMP_GET2(global_director);



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


			Fov::Initialize();
		}

		void Dispose()
		{
			Fov::Dispose();
		}

		void Update()
		{
		}

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

		void DumpViewState(cstring name)
		{
			if(!name) name = "view_state.bin";

			FILE* f = Settings::CreateReport(name, false, false);

			if(f != NULL)
			{
				fwrite(&Rasterizer::RenderGlobals()->camera, sizeof(Rasterizer::s_render_camera), 1, f);
				fwrite(&Rasterizer::RenderGlobals()->frustum, sizeof(Rasterizer::s_render_frustum), 1, f);
				fclose(f);
			}
		}

		void* DumpViewStateEvaluate(void** arguments)
		{
			//YELO_DEBUG(Settings::ReportsPath(), true);
// 			struct s_arguments {
// 				cstring name;
// 			}* args = CAST_PTR(s_arguments*, arguments);
// 
// 			Engine::Console::Print(args->name);
 			DumpViewState();

			return NULL;
		}
	};
};

#include "Game/CameraFov.inl"
#endif