/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Interface/Controls.hpp"
#include "Interface/TextBlock.hpp"

#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Fov
	{
#define DEF_FOV_H	1.22173047065735f
#define DEF_FOV_V	(atanf(0.75f*tanf(DEF_FOV_H/2.f))*2.f)

		struct s_fov_globals
		{
			struct
			{
				real height;
				real vertical;
				real height_max;
			}fov;

			struct
			{
				real width, height, distance;
			}screen;

			void InitializeToDefaultSettings()
			{
				fov.vertical = DEF_FOV_V;
			}

			void Scale()
			{
				screen.distance = screen.height / tanf( fov.vertical / 2.f);
				fov.height = atanf( screen.width / screen.distance ) * 2.f;
			}
		};

		static s_fov_globals g_fov_globals
		{
			{
				DEF_FOV_H,
				0.f,
				3.14159f,
			}, // fov

			{
				0
			} // screen
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
				g_fov_globals.fov.vertical = Get().m_field_of_view;
 				g_fov_globals.Scale();
			}

			void PreSave() final override
			{
				Get().m_field_of_view = g_fov_globals.fov.vertical;
			}
		};

		static bool RequiresZoomFix()
		{
			real h = Camera::Observer()->origin.fov;
			real v = atanf(g_fov_globals.screen.height/g_fov_globals.screen.width * tanf(h/2.f)) * 2.f;

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

			GET_PTR(MAX) = &g_fov_globals.fov.height_max;
			
			g_fov_globals.InitializeToDefaultSettings();
			
			c_settings_fov::Register(Settings::Manager());
		}

		void Dispose()
		{
			c_settings_fov::Unregister(Settings::Manager());
		}

		void Update()
		{
			real player_fov = Camera::Observer()->command_update->fov;
			if(player_fov != 0.0f)
				Camera::Observer()->command.fov = g_fov_globals.fov.height * player_fov / DEF_FOV_H;
		}

		real GetFieldOfView()
		{
			 return g_fov_globals.fov.vertical * 180.f / 3.14159f;
		}

		void SetFieldOfView(const real fov)
		{
			g_fov_globals.fov.vertical = fov / (180.f / 3.14159f);
			g_fov_globals.Scale();
		}

		void ResetFieldOfView()
		{
			g_fov_globals.InitializeToDefaultSettings();
		}

#if defined(DX_WRAPPER)
		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP)
		{
			g_fov_globals.screen.width = real(pPP->BackBufferWidth);
			g_fov_globals.screen.height = real(pPP->BackBufferHeight);
			g_fov_globals.Scale();
		}

		void OnLostDevice() { }

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
		{
			g_fov_globals.screen.width = real(pPP->BackBufferWidth);
			g_fov_globals.screen.height = real(pPP->BackBufferHeight);
			g_fov_globals.Scale();
		}
		
		void Render() { }
		
		void Release() { }
#endif

#undef DEF_FOV_H
#undef DEF_FOV_V
	};
};