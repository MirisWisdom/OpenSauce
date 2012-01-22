/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Game/Camera.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Menu
	{
		struct {
			TextBlock* menu;
			Enums::settings_menu selection;
			bool returning;
			PAD24;

			void Dispose()
			{
				if(menu != NULL)
				{
					delete menu;
					menu = NULL;
				}
			}

		private:
			Enums::settings_menu GetSelection()
			{
				for (int32 i = Enums::k_settings_menu_key_first_catagory; 
					i <= Enums::k_settings_menu_key_last_catagory; 
					i++)
				{
					if (Input::GetKeyState( CAST(Enums::Key,i) ))
						return CAST(Enums::settings_menu, i - Enums::k_settings_menu_base_key);
				}

				if (Input::GetKeyState( (Enums::Key)Enums::k_settings_menu_key ) == 1)
					return Enums::_settings_menu_off;

				return Enums::_settings_menu_on;
			}

			void WaitForUnclick()
			{
				if(Input::GetMouseButtonState(Enums::_MouseButton1) == 0)
					returning = false;

				if (!returning)
					Input::AllowMovement(true);
			}

			void WaitForReturn(bool (API_FUNC* AdjustSettings)())
			{
				Hud::ShowCrosshair() = false;
				Input::AllowMovement(false);

				bool is_done = AdjustSettings();

				// Save
				if (is_done || 
					Input::GetMouseButtonState(Enums::_MouseButton1) == 1 ||
					Input::IsInMenu() || Input::IsInChat())
				{
					Settings::SaveSettings();
					Hud::ShowCrosshair() = true;
					selection = Enums::_settings_menu_on;
					returning = true;
				}
			}

		public:
			void Render()
			{
				if (selection < Enums::_settings_menu__first_catagory)
					if (Input::IsInMenu() || Input::IsInChat())
						return;

				switch (selection)
				{
					// Menu Functions
				case Enums::_settings_menu_fov:		WaitForReturn(Fov::AdjustSettings);				break;
				case Enums::_settings_menu_weapons:	WaitForReturn(Objects::Weapon::AdjustSettings);	break;
				case Enums::_settings_menu_hud:		WaitForReturn(Hud::AdjustSettings);				break;
				case Enums::_settings_menu_vehicles:WaitForReturn(Objects::Vehicle::AdjustSettings);break;
					// TODO: add your menu's AdjustSettings handlers here
				}

				switch (selection)
				{
					// Menu Off
				case Enums::_settings_menu_off:
					if (Input::GetKeyState( CAST(Enums::Key,Enums::k_settings_menu_key) ) == 1)
						selection = Enums::_settings_menu_on;
					break;

					// Menu On
				case Enums::_settings_menu_on:
					menu->Render();
					selection = GetSelection();
					WaitForUnclick();
					break;
				}
			}

		}_menu_globals = {
			NULL,
			Enums::_settings_menu_off,
			false,
		};

		void Initialize()	{}

		void Dispose()
		{
			_menu_globals.Dispose();
		}


#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
		{
			_menu_globals.menu = new TextBlock(pDevice, pPP);
			_menu_globals.menu->ApplyScheme();
			_menu_globals.menu->SetDimensions(160, 0);
			_menu_globals.menu->Attach(Enums::_attach_method_top_center, 0,0,0,0);
			_menu_globals.menu->SetTextAlign(DT_LEFT);

			// TODO: add your menu's text display here
			_menu_globals.menu->SetText(
				L"\x2081. Field of View\n"
				L"\x2082. Weapon Views\n"
				L"\x2083. HUD Options\n"
	#if YELO_HAS_VEHICLE_VIEWS == TRUE
				L"\x2084. Vehicle Views"
	#endif
				);
			_menu_globals.menu->Refresh();
		}

		void OnLostDevice()
		{
			_menu_globals.menu->OnLostDevice();
		}

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
		{
			_menu_globals.menu->OnResetDevice(pPP);
			_menu_globals.menu->Refresh();
		}

		void Render()
		{
			_menu_globals.Render();
		}
		void Release() 
		{
			_menu_globals.menu->Release();
		}
#endif
	};
};