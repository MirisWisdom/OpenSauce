/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
			struct {
				bool show_hud;
				bool scale_hud;
				bool show_crosshair;
				bool enable_scaling;
			}m_flags;

			TextBlock* m_menu_text;

			D3DXMATRIX m_scale_matrix;
		};
		static s_hud_globals g_hud_globals;

		bool& ScaleHUD() { return g_hud_globals.m_flags.scale_hud; }
		bool& ShowCrosshair() { return g_hud_globals.m_flags.show_crosshair; }

		void* HudGlobals() DPTR_IMP_GET2(hud_globals);

		// disable scaling when drawing screen space position arrows
		API_FUNC_NAKED static void PLATFORM_API GAME_ENGINE_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT()
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(RENDER_NAV_POINT);
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(GAME_ENGINE_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT)+5;

			__asm {
				mov		g_hud_globals.m_flags.enable_scaling, 0
				call	CALL_ADDR
				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED static void PLATFORM_API HUD_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT()
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(RENDER_NAV_POINT);
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT)+5;

			__asm {
				mov		g_hud_globals.m_flags.enable_scaling, 0
				call	CALL_ADDR
				jmp		RETN_ADDRESS
			}
		}

		static void PLATFORM_API HUD_DRAW_PLAYERS_CALL_HUD_DRAW_FRIENDLY_INDICATOR()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(HUD_DRAW_FRIENDLY_INDICATOR);

			g_hud_globals.m_flags.enable_scaling = false;
			__asm call	TEMP_CALL_ADDR
		}

		// disable scaling when drawing the cinematic black bars
		static void PLATFORM_API RASTERIZER_RENDER_WIDGET_CALL_CINEMATIC_RENDER()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CINEMATIC_RENDER);

			g_hud_globals.m_flags.enable_scaling = false;
			__asm call	TEMP_CALL_ADDR
		}

		// disable scaling when drawing the loading UI
		static void PLATFORM_API RASTERIZER_RENDER_WIDGET_CALL_DRAW_LOADING_UI()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DRAW_LOADING_UI);

			g_hud_globals.m_flags.enable_scaling = false;
			__asm call	TEMP_CALL_ADDR
		}

		// disable scaling when drawing full screen UI widgets
		API_FUNC_NAKED static void PLATFORM_API HOOK_RASTERIZER_RENDER_WIDGET_UI_WIDGET_BOUNDS()
		{
			static rectangle2d* WIDGET_BOUNDS = NULL;
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_RENDER_WIDGET_UI_WIDGET_BOUNDS_HOOK_RETN);

			_asm {
				mov		esi, [eax+ecx+14h]
				mov		eax, [ebp+30h]
				mov		WIDGET_BOUNDS, esi
				add		WIDGET_BOUNDS, 24h
				pushad
			};

			// doing this all on one go results in an exception
			g_hud_globals.m_flags.enable_scaling = (WIDGET_BOUNDS->right - WIDGET_BOUNDS->left != 640);// && (WIDGET_BOUNDS->bottom - WIDGET_BOUNDS->top != 480);
			g_hud_globals.m_flags.enable_scaling &= (WIDGET_BOUNDS->bottom - WIDGET_BOUNDS->top != 480);

			_asm {
				popad
				jmp		RETN_ADDRESS
			};
		}

		void EnableScaling()
		{
			g_hud_globals.m_flags.enable_scaling = true;
		}

		void DisableScaling()
		{
			g_hud_globals.m_flags.enable_scaling = false;
		}

		static HRESULT SetVertexShaderConstantF_ScreenProj(IDirect3DDevice9* device, UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
		{
			D3DXMATRIX* original_matrix = CAST_PTR(D3DXMATRIX*, (float*)pConstantData);

			if(g_hud_globals.m_flags.scale_hud && g_hud_globals.m_flags.enable_scaling)
			{
				original_matrix->_11 = g_hud_globals.m_scale_matrix._11;
				original_matrix->_14 = g_hud_globals.m_scale_matrix._14;
				original_matrix->_22 = g_hud_globals.m_scale_matrix._22;
				original_matrix->_24 = g_hud_globals.m_scale_matrix._24;
			}

			return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
		}

		void InitializeHUDSettings()
		{
			g_hud_globals.m_flags.show_hud = true;
			g_hud_globals.m_flags.show_crosshair = true;
			g_hud_globals.m_flags.scale_hud = false;
			g_hud_globals.m_flags.enable_scaling = true;

			D3DXMatrixIdentity(&g_hud_globals.m_scale_matrix);
		}

		void UpdateUIScale(const uint16 screen_width, const uint16 screen_height)
		{
			real_point2d scale_amount;
			scale_amount.x = 1;
			scale_amount.y = 1;

			const real old_ratio = (real)4/3;
			const real new_ratio = (real)screen_width / screen_height;

			if (new_ratio > old_ratio)
				scale_amount.x = old_ratio / new_ratio;
			else
				scale_amount.y = new_ratio / old_ratio;

			g_hud_globals.m_scale_matrix._11 = (2.0f / 640.0f) * scale_amount.x;	// x projection
			g_hud_globals.m_scale_matrix._22 = (-2.0f / 480.0f) * scale_amount.y;	// y projection
			g_hud_globals.m_scale_matrix._14 = -scale_amount.x;						// x offset
			g_hud_globals.m_scale_matrix._24 = scale_amount.y;						// y offset
		}

		void Initialize()
		{
			// hook the screen projection matrix for widgets, text and the motion tracker
			for(int i = 0; i < NUMBEROF(K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS); i++)
			{
				void*& call_address = K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS[i];

				Memory::WriteRelativeCall(SetVertexShaderConstantF_ScreenProj, call_address, true);
				byte* nop_offset = CAST_PTR(byte*, call_address) + 5;
				*nop_offset = Enums::_x86_opcode_nop;
			}

			// hooks for disabling scaling
			Memory::WriteRelativeJmp(
				GAME_ENGINE_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT,
				GET_FUNC_VPTR(GAME_ENGINE_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT), true);

			Memory::WriteRelativeJmp(
				HUD_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT,
				GET_FUNC_VPTR(HUD_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT), true);

			Memory::WriteRelativeCall(
				HUD_DRAW_PLAYERS_CALL_HUD_DRAW_FRIENDLY_INDICATOR,
				GET_FUNC_VPTR(HUD_DRAW_PLAYERS_CALL_HOOK_HUD_DRAW_FRIENDLY_INDICATOR));

			Memory::WriteRelativeCall(
				RASTERIZER_RENDER_WIDGET_CALL_CINEMATIC_RENDER,
				GET_FUNC_VPTR(RASTERIZER_RENDER_WIDGET_CALL_HOOK_CINEMATIC_RENDER), true);

			Memory::WriteRelativeJmp(
				HOOK_RASTERIZER_RENDER_WIDGET_UI_WIDGET_BOUNDS,
				GET_FUNC_VPTR(RASTERIZER_RENDER_WIDGET_UI_WIDGET_BOUNDS_HOOK), true);

			for(int i = 0; i < NUMBEROF(K_RASTERIZER_RENDER_WIDGET_DRAW_LOADING_UI_CALLS); i++)
				Memory::WriteRelativeCall(
					RASTERIZER_RENDER_WIDGET_CALL_DRAW_LOADING_UI,
					K_RASTERIZER_RENDER_WIDGET_DRAW_LOADING_UI_CALLS[i], true);
		}

		void Dispose() {}

		void Update()
		{
			if(g_hud_globals.m_flags.show_crosshair)
				GET_PTR(render_crosshairs_jump_asm) = 0x840f;	// set to jz
			else
				GET_PTR(render_crosshairs_jump_asm) = 0xE990;	// set to nop, relative jmp

			Memory::FunctionProcessRenderHudIsDisabled() = !g_hud_globals.m_flags.show_hud;
		}


		static void AdjustSettings_Render()
		{
			wchar_t text[256];
			size_t current_length = 0;
			wchar_t* itr = text;

#define ADD_MENU_ITEM(format,...)									\
				swprintf_s(itr,NUMBEROF(text)-current_length,format,__VA_ARGS__);	\
				current_length = wcslen(text);						\
				itr = text + current_length;

			ADD_MENU_ITEM(L"\x2081. HUD				(%s)\n",	g_hud_globals.m_flags.show_hud ?		L"On" : L"Off");
			ADD_MENU_ITEM(L"\x2082. HUD Scaling		(%s)\n",	g_hud_globals.m_flags.scale_hud ?	L"On" : L"Off");
			ADD_MENU_ITEM(L"\nLeft-Click to Save");
			g_hud_globals.m_menu_text->SetText(text);
			g_hud_globals.m_menu_text->Refresh();
			g_hud_globals.m_menu_text->Render();
#undef ADD_MENU_ITEM
		}

		bool AdjustSettings()
		{
			if (Input::GetKeyState(Enums::_Key1) == 1)
				g_hud_globals.m_flags.show_hud = !g_hud_globals.m_flags.show_hud;

			if (Input::GetKeyState(Enums::_Key2) == 1)
				g_hud_globals.m_flags.scale_hud = !g_hud_globals.m_flags.scale_hud;

			AdjustSettings_Render();

			return false;
		}

		void LoadSettings(TiXmlElement* hud_element)
		{
			InitializeHUDSettings();

			if(hud_element != NULL)
			{
				g_hud_globals.m_flags.show_hud = Settings::ParseBoolean( hud_element->Attribute("show") );
				g_hud_globals.m_flags.scale_hud = Settings::ParseBoolean( hud_element->Attribute("scale") );
			}
		}

		void SaveSettings(TiXmlElement* hud_element)
		{
			hud_element->SetAttribute("show", Settings::BooleanToString(g_hud_globals.m_flags.show_hud));
			hud_element->SetAttribute("scale", Settings::BooleanToString(g_hud_globals.m_flags.scale_hud));
		}


#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
		{
			UpdateUIScale((uint16)pPP->BackBufferWidth, (uint16)pPP->BackBufferHeight);

			g_hud_globals.m_menu_text = new TextBlock(pDevice, pPP);
			g_hud_globals.m_menu_text->ApplyScheme();
			g_hud_globals.m_menu_text->SetDimensions(200,0);
			g_hud_globals.m_menu_text->Attach(Enums::_attach_method_center, 0,0,0,0);
			g_hud_globals.m_menu_text->SetTextAlign(DT_LEFT);

			g_hud_globals.m_menu_text->SetText(L"dumb");
			g_hud_globals.m_menu_text->Refresh();
		}

		void OnLostDevice()
		{
			g_hud_globals.m_menu_text->OnLostDevice();
		}

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
		{
			UpdateUIScale((uint16)pPP->BackBufferWidth, (uint16)pPP->BackBufferHeight);

			g_hud_globals.m_menu_text->OnResetDevice(pPP);
			g_hud_globals.m_menu_text->Refresh();
		}

		void Render() {}

		void Release() 
		{
			g_hud_globals.m_menu_text->Release();

			delete g_hud_globals.m_menu_text;
			g_hud_globals.m_menu_text = NULL;
		}
#endif
	};
};