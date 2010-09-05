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
#include "Interface/Controls.hpp"
#include "Interface/TextBlock.hpp"
#include "Game/EngineFunctions.hpp"
#include "Memory/FunctionInterface.hpp"

namespace Yelo
{
	namespace Hud
	{
		struct s_hud_globals {
			bool show_hud;
			bool scale_hud;
			bool show_chat;		// not supported anymore, as Battery implementation interfered with chat-logging
			bool show_kills;	// not supported anymore, as Battery implementation interfered with chat-logging
			bool show_kd_msg;	// supplied Battery source didn't include this wtf...(not implemented)

			bool show_crosshair;
			PAD16;

			TextBlock* menu;
			struct {
				int32 width, height;
			}screen;

			struct {
				bool is_anchor;
				bool is_defined;
				PAD16;
				real_point2d scale, anchor;
			}anchor;

			void Dispose()
			{
				if(menu != NULL)
				{
					delete menu;
					menu = NULL;
				}
			}

			void InitializeToDefaultSettings()
			{
				show_hud = true;
				scale_hud = false;
				show_chat = true;
				show_kills = false;
				show_kd_msg = false;
			}

			static void ShowCrosshair(bool visible)
			{
				static uint16 opcode_jz = 0x840F;
				static uint16 opcode_jmp = 0xE990; // little-endian hack: nop, jmp
				Memory::WriteMemory(GET_FUNC_VPTR(RENDER_CROSSHAIRS_DISABLE_MOD), visible ? &opcode_jz : &opcode_jmp, sizeof(uint16));
			}

			//////////////////////////////////////////////////////////////////////////
			// show_hud
			static void ShowHud(bool visible)
			{
				Memory::FunctionProcessRenderHudIsDisabled() = !visible;
			}

			//////////////////////////////////////////////////////////////////////////
			// scale_hud
			void UpdateScale()
			{
				anchor.scale.x = 1;
				anchor.scale.y = 1;

				const real old_ratio = (real)4/3;
				real new_ratio = (real)screen.width/screen.height;

				if (new_ratio > old_ratio)
					anchor.scale.x = old_ratio / new_ratio;
				else
					anchor.scale.y = new_ratio / old_ratio;
			}

			void Scale(void* ref_ptr, void *pVertexStreamZeroData)
			{
				if (!scale_hud) return;

				bool good_ref = false;
				for (int32 i=0; i<NUMBEROF(Scale_allowed_refs); i++)
					if (ref_ptr == Scale_allowed_refs[i])
						good_ref = true;

				if (!good_ref)
					return;

				TEXTURE_VERTEX* vertices = CAST_PTR(TEXTURE_VERTEX*,pVertexStreamZeroData);

				// allow fullscreen textures to be stretched (except the Halo title logo)
				if (vertices[0].x == 0 && vertices[2].x == 640 &&
					vertices[0].y != 28 && vertices[2].y != 284)
					return;

				if (anchor.is_anchor)
				{
					// scale size and maintain position (i.e. navpoints)
					if (!anchor.is_defined)
					{
						anchor.anchor.x = 0;
						anchor.anchor.y = 0;
						for (int32 i=0; i<4; i++)
						{
							anchor.anchor.x += vertices[i].x;
							anchor.anchor.y += vertices[i].y;
						}
						anchor.anchor.x /= 4;
						anchor.anchor.y /= 4;

						anchor.is_defined = true;
					}

					for (int i=0; i<4; i++)
					{
						vertices[i].x = anchor.anchor.x - (anchor.anchor.x - vertices[i].x) * anchor.scale.x;
						vertices[i].y = anchor.anchor.y - (anchor.anchor.y - vertices[i].y) * anchor.scale.y;
					}
				}
				else
				{
					// scale size and center position (i.e. non-navpoints)
					real shift_x = 640/2 * (1-anchor.scale.x);
					real shift_y = 480/2 * (1-anchor.scale.y);

					for (int32 i=0; i<4; i++)
					{
						vertices[i].x *= anchor.scale.x;
						vertices[i].y *= anchor.scale.y;
						vertices[i].x += shift_x;
						vertices[i].y += shift_y;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// show_kd_msg

		}_hud_globals = {
			true, false, true, false, false,
			true,

			NULL,
			{0,0},

			{
				false, false,
				{.0f, .0f},
				{.0f, .0f},
			},
		};


#pragma region render nav point
		API_FUNC_NAKED static void PLATFORM_API GAME_ENGINE_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(RENDER_NAV_POINT);
			static uint32 jmp_back = GET_FUNC_PTR(GAME_ENGINE_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT)+5;

			__asm {
				mov		_hud_globals.anchor.is_anchor, 1
				mov		_hud_globals.anchor.is_defined, 0
				call	TEMP_CALL_ADDR
				mov		_hud_globals.anchor.is_anchor, 0
				jmp		jmp_back // jmp back to GAME_ENGINE_RENDER_NAV_POINTS
			}
		}


		API_FUNC_NAKED static void PLATFORM_API HUD_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(RENDER_NAV_POINT);
			static uint32 jmp_back = GET_FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT)+5;

			__asm {
				mov		_hud_globals.anchor.is_anchor, 1
				mov		_hud_globals.anchor.is_defined, 0
				call	TEMP_CALL_ADDR
				mov		_hud_globals.anchor.is_anchor, 0
				jmp		jmp_back // jmp back to HUD_RENDER_NAV_POINTS
			}
		}
#pragma endregion

#pragma region render team indicator
		static void PLATFORM_API HUD_DRAW_PLAYERS_CALL_HUD_DRAW_FRIENDLY_INDICATOR()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(HUD_DRAW_FRIENDLY_INDICATOR);

			_hud_globals.anchor.is_anchor = true;
			_hud_globals.anchor.is_defined = false;
			__asm call	TEMP_CALL_ADDR
			_hud_globals.anchor.is_anchor = false;
		}
#pragma endregion


		void Initialize()
		{
			Memory::WriteRelativeJmp(
				GAME_ENGINE_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT,
				GET_FUNC_VPTR(GAME_ENGINE_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT), true);
			Memory::WriteRelativeJmp(
				HUD_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT,
				GET_FUNC_VPTR(HUD_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT), true);

			Memory::WriteRelativeCall(
				HUD_DRAW_PLAYERS_CALL_HUD_DRAW_FRIENDLY_INDICATOR,
				GET_FUNC_VPTR(HUD_DRAW_PLAYERS_CALL_HOOK_HUD_DRAW_FRIENDLY_INDICATOR));
		}

		void Dispose()
		{
			_hud_globals.Dispose();
		}


		bool HudIsScaled() { return _hud_globals.scale_hud; }
		void* HudGlobals() DPTR_IMP_GET2(hud_globals);

		static void AdjustSettings_Render()
		{
			wchar_t text[256];
			size_t current_length = 0;
			wchar_t* itr = text;

#define ADD_MENU_ITEM(format,...)									\
				swprintf_s(itr,NUMBEROF(text)-current_length,format,__VA_ARGS__);	\
				current_length = wcslen(text);						\
				itr = text + current_length;

			ADD_MENU_ITEM(L"\x2081. HUD				(%s)\n",	_hud_globals.show_hud ?		L"On" : L"Off");
			ADD_MENU_ITEM(L"\x2082. HUD Scaling		(%s)\n",	_hud_globals.scale_hud ?	L"On" : L"Off");
			//ADD_MENU_ITEM(L"\x2083. Chat Messages	(%s)\n",	_hud_globals.show_chat ?	L"On" : L"Off");
			//ADD_MENU_ITEM(L"\x2084. Death Reports	(%s)\n",	_hud_globals.show_kills ?	L"On" : L"Off");
			//ADD_MENU_ITEM(L"\x2085. My HUD Reports	(%s)\n",	_hud_globals.show_kd_msg ?	L"On" : L"Off");
			ADD_MENU_ITEM(L"\nLeft-Click to Save");
			_hud_globals.menu->SetText(text);
			_hud_globals.menu->Refresh();
			_hud_globals.menu->Render();
#undef ADD_MENU_ITEM
		}
		bool AdjustSettings()
		{
			if (Input::GetKeyState(Enums::_Key1) == 1)
				_hud_globals.ShowHud(_hud_globals.show_hud = !_hud_globals.show_hud);

			if (Input::GetKeyState(Enums::_Key2) == 1)
				_hud_globals.scale_hud = !_hud_globals.scale_hud;

			//if (Input::GetKeyState(Enums::_Key3) == 1)
			//	_hud_globals.show_chat = !_hud_globals.show_chat;

			//if (Input::GetKeyState(Enums::_Key4) == 1)
			//	_hud_globals.show_kills = !_hud_globals.show_kills;

			//if (Input::GetKeyState(Enums::_Key5) == 1)
			//	_hud_globals.show_kd_msg = !_hud_globals.show_kd_msg;

			AdjustSettings_Render();

			return false;
		}

		void LoadSettings(TiXmlElement* hud_element)
		{
			_hud_globals.InitializeToDefaultSettings();

			if(hud_element != NULL)
			{
				_hud_globals.show_hud = Settings::ParseBoolean( hud_element->Attribute("show") );
				_hud_globals.scale_hud = Settings::ParseBoolean( hud_element->Attribute("scale") );
				_hud_globals.show_chat = Settings::ParseBoolean( hud_element->Attribute("showChat") );

				TiXmlElement* reports = hud_element->FirstChildElement("Reports");
				if(reports != NULL)
				{
					_hud_globals.show_kills = Settings::ParseBoolean( reports->Attribute("showDeaths") );
					_hud_globals.show_kd_msg = Settings::ParseBoolean( reports->Attribute("showMyKillDeaths") );
				}

				// This was causing the game to crash. Placed the code in Initialize3D.
				// Code now works. Go figure.
				//_hud_globals.ShowHud(_hud_globals.show_hud);
			}
		}

		void SaveSettings(TiXmlElement* hud_element)
		{
			hud_element->SetAttribute("show", Settings::BooleanToString(_hud_globals.show_hud));
			hud_element->SetAttribute("scale", Settings::BooleanToString(_hud_globals.scale_hud));
			hud_element->SetAttribute("showChat", Settings::BooleanToString(_hud_globals.show_chat));

			TiXmlElement* reports = new TiXmlElement("Reports");
					hud_element->LinkEndChild(reports);
			reports->SetAttribute("showDeaths", Settings::BooleanToString(_hud_globals.show_kills));
			reports->SetAttribute("showMyKillDeaths", Settings::BooleanToString(_hud_globals.show_kd_msg));
		}


#if defined(DX_WRAPPER)
		void Scale(void* ref_ptr, void *pVertexStreamZeroData)
		{
			_hud_globals.Scale(ref_ptr, pVertexStreamZeroData);
		}

		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
		{
			_hud_globals.ShowHud(_hud_globals.show_hud);

			_hud_globals.screen.width = pPP->BackBufferWidth;
			_hud_globals.screen.height = pPP->BackBufferHeight;
			_hud_globals.UpdateScale();

			_hud_globals.menu = new TextBlock(pDevice,pPP);
			_hud_globals.menu->ApplyScheme();
			_hud_globals.menu->SetDimensions(200,0);
			_hud_globals.menu->Attach(Enums::_attach_method_center, 0,0,0,0);
			_hud_globals.menu->SetTextAlign(DT_LEFT);

			_hud_globals.menu->SetText(L"dumb");
			_hud_globals.menu->Refresh();
		}

		void OnLostDevice()
		{
			_hud_globals.menu->OnLostDevice();
		}

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
		{
			_hud_globals.screen.width = pPP->BackBufferWidth;
			_hud_globals.screen.height = pPP->BackBufferHeight;
			_hud_globals.UpdateScale();

			_hud_globals.menu->OnResetDevice(pPP);
			_hud_globals.menu->Refresh();
		}

		void Render() { /* nothing */ }
		void Release() 
		{
			_hud_globals.menu->Release();
		}
#endif
	};
};