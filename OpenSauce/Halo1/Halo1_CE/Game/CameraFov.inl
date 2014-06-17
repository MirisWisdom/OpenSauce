/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Interface/Controls.hpp"
#include "Interface/TextBlock.hpp"

#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Fov
	{
#define DEF_FOV_H	1.22173047065735f
#define DEF_FOV_V	(atanf(0.75f*tanf(DEF_FOV_H/2.f))*2.f)

		struct {
			struct {
				real height;
				real vertical;
				real height_max;
			}fov;

			struct {
				real height;
				real distance;
			}weapon;

			struct {
				real width, height, distance;
			}screen;

			TextBlock* menu;
			ID3DXLine* window;


			void InitializeToDefaultSettings()
			{
				fov.vertical = DEF_FOV_V;
			}

			void Scale()
			{
				screen.distance = screen.height / tanf( fov.vertical / 2.f);
				fov.height = atanf( screen.width / screen.distance ) * 2.f;
			}

			void RenderWindow()
			{
				real w = screen.distance * tanf( DEF_FOV_H / 2.f );
				real h = screen.distance * tanf( DEF_FOV_V / 2.f );

				real lw = 2.f;	// line width
				window->SetWidth(lw);

				RECT rc;
				SetRect(&rc,
					int(screen.width/2.f	- w/2.f + lw/2.f),
					int(screen.height/2.f	- h/2.f + lw/2.f),
					int(screen.width/2.f	+ w/2.f - lw/2.f),
					int(screen.height/2.f	+ h/2.f - lw/2.f) );

				real bw = w * 0.05f; // bracket width
				D3DXVECTOR2 left_bracket[4];
				left_bracket[0].x = FLOAT(rc.left + LONG(bw));	left_bracket[0].y = FLOAT(rc.top);
				left_bracket[1].x = FLOAT(rc.left);				left_bracket[1].y = FLOAT(rc.top);
				left_bracket[2].x = FLOAT(rc.left);				left_bracket[2].y = FLOAT(rc.bottom);
				left_bracket[3].x = FLOAT(rc.left + LONG(bw));	left_bracket[3].y = FLOAT(rc.bottom);

				D3DXVECTOR2 right_bracket[4];
				right_bracket[0].x = FLOAT(rc.right - LONG(bw));right_bracket[0].y = FLOAT(rc.top);
				right_bracket[1].x = FLOAT(rc.right);			right_bracket[1].y = FLOAT(rc.top);
				right_bracket[2].x = FLOAT(rc.right);			right_bracket[2].y = FLOAT(rc.bottom);
				right_bracket[3].x = FLOAT(rc.right - LONG(bw));right_bracket[3].y = FLOAT(rc.bottom);

				window->Begin();
				window->Draw(left_bracket, NUMBEROF(left_bracket), D3DCOLOR_RGBA(255,255,255,90));
				window->Draw(right_bracket, NUMBEROF(right_bracket), D3DCOLOR_RGBA(255,255,255,90));
				window->End();
			}

		}_fov_globals = {
			{
				DEF_FOV_H,
				0.f,
				3.14159f,
			}, // fov

			{
				1/13.5f,
				0.f
			}, // weapon

			{
				0
			}, // screen

			nullptr,
			nullptr,
		};

		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<real> m_field_of_view;

			c_settings_container()
				: Configuration::c_configuration_container("Camera")
				, m_field_of_view("FieldOfView", DEF_FOV_V)
			{ }
			
		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override
			{
				return std::vector<i_configuration_value* const> { &m_field_of_view };
			}
		};

		class c_settings_fov
			: public Settings::c_settings_singleton<c_settings_container, c_settings_fov>
		{
		public:
			void PostLoad() final override
			{
				_fov_globals.fov.vertical = Get().m_field_of_view;
 				_fov_globals.weapon.distance = _fov_globals.weapon.height / tanf( DEF_FOV_V / 2.f );
 				_fov_globals.Scale();
			}

			void PreSave() final override
			{
				Get().m_field_of_view = _fov_globals.fov.vertical;
			}
		};

		static bool RequiresZoomFix()
		{
			real h = Camera::Observer()->origin.fov;
			real v = atanf(_fov_globals.screen.height/_fov_globals.screen.width * tanf(h/2.f)) * 2.f;

			return Camera::Observer()->command.fov < h && v > 1.28f;
		}

		static void PLATFORM_API observer_update_command_hook()
		{
			Fov::Update();
		}

		static void PLATFORM_API OBSERVER_UPDATE_POSITIONS()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(OBSERVER_UPDATE_POSITIONS);

			__asm call	FUNCTION

			GET_PTR(OBSERVER_UPDATE_POSITIONS_no_scope_blur) = Fov::RequiresZoomFix();
		}

		void Initialize()
		{
			// Overwrite the 'retn' at the observer_update_command with a jmp to our hook code
			Memory::WriteRelativeJmp(observer_update_command_hook, GET_FUNC_VPTR(OBSERVER_UPDATE_COMMAND_HOOK), true);
			Memory::WriteRelativeCall(OBSERVER_UPDATE_POSITIONS, GET_FUNC_VPTR(OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS));

			GET_PTR(MAX) = &_fov_globals.fov.height_max;
			
			_fov_globals.InitializeToDefaultSettings();
			
			c_settings_fov::Instance().Register(Settings::Manager());
		}

		void Dispose()
		{
			if(_fov_globals.menu != nullptr)
			{
				delete _fov_globals.menu;
				_fov_globals.menu = nullptr;
			}

			c_settings_fov::Instance().Unregister(Settings::Manager());
		}

		void Update()
		{
			real player_fov = Camera::Observer()->command_update->fov;
			if(player_fov != 0.0f)
				Camera::Observer()->command.fov = _fov_globals.fov.height * player_fov / DEF_FOV_H;
		}

		Enums::settings_adjustment_result AdjustSettings()
		{
			if (Input::GetMouseButtonState(Enums::_MouseButton3) == 1)
			{
				_fov_globals.InitializeToDefaultSettings();
				_fov_globals.Scale();
			}

			_fov_globals.fov.vertical -= Input::GetMouseAxisState(Enums::_MouseAxisY) / 1000.0f;
			_fov_globals.Scale();

			real v = _fov_globals.fov.vertical * 180.f / 3.14159f;
			real h = _fov_globals.fov.height * 180.f / 3.14159f;

			_fov_globals.RenderWindow();
			wchar_t text[128];
			swprintf_s(text, 
				L"Mouse \x2195 = FOV\n\n%d.%02d\x00B2 x %d.%02d\x00B2\n"
				L"\nLeft-Click to Save"
				L"\nRight-Click to Reset",
				(int32)h, (int32)((h - (int32)h)*100),
				(int32)v, (int32)((v - (int32)v)*100)
				);
			_fov_globals.menu->SetText(text);
			_fov_globals.menu->Refresh();
			_fov_globals.menu->Render();

			return Enums::_settings_adjustment_result_not_finished;
		}

#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
		{
			_fov_globals.screen.width = real(pPP->BackBufferWidth);
			_fov_globals.screen.height = real(pPP->BackBufferHeight);
			_fov_globals.Scale();

			_fov_globals.menu = new TextBlock(pDevice, pPP);
			_fov_globals.menu->ApplyScheme();
			_fov_globals.menu->SetDimensions(350, 0);
			_fov_globals.menu->Attach(Enums::_attach_method_center, 0,0,0,0);
			_fov_globals.menu->SetTextAlign(DT_CENTER);

			_fov_globals.menu->SetText(L"dumb");
			_fov_globals.menu->Refresh();

			D3DXCreateLine(pDevice, &_fov_globals.window);
		}

		void OnLostDevice()
		{
			_fov_globals.menu->OnLostDevice();
			_fov_globals.window->OnLostDevice();
		}

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
		{
			_fov_globals.screen.width = real(pPP->BackBufferWidth);
			_fov_globals.screen.height = real(pPP->BackBufferHeight);
			_fov_globals.Scale();

			_fov_globals.menu->OnResetDevice(pPP);
			_fov_globals.menu->Refresh();
			_fov_globals.window->OnResetDevice();
		}
		
		void Render()
		{
		}
		
		void Release()
		{
			_fov_globals.menu->Release();
			safe_release(_fov_globals.window);
		}
#endif

#undef DEF_FOV_H
#undef DEF_FOV_V
	};
};