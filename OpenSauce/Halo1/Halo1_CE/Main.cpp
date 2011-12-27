/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"

#ifdef API_DEBUG
#include "Common/DebugMemory.hpp"
#endif

#include <psapi.h>
#pragma comment (lib, "psapi.lib")

#include "Memory/MemoryInterface.hpp"
#include "Common/GameSystems.hpp"

#if PLATFORM_IS_USER && defined(DX_WRAPPER)
	#include "Rasterizer/DX9/DxWrapper.hpp"
#endif


namespace Yelo
{
	namespace Enums
	{
		enum version_result_code : long_enum
		{
			_version_result_code_invalid,	// An unsupported program is trying to run us. Go to hell.
			_version_result_code_valid,		// A supported version of Halo is trying to run it
			_version_result_code_dx9_app,	// A known DX9 application, ie sapien, is trying to run this

			_version_result_code
		};
	};

	namespace Main
	{
		static struct {
			bool enabled;
			HMODULE module_handle;
		}_main_globals = {
			false,
			NULL
		};

		bool IsYeloEnabled()
		{
			return _main_globals.enabled;
		}

		HMODULE& YeloModuleHandle()
		{
			return _main_globals.module_handle;
		}

		static bool IsVersionInfoValid()
		{
			ENGINE_PTR(VS_FIXEDFILEINFO, version_info, 0x81D598, 0x6E9598);

			const __int64 version = 
#if PLATFORM_VERSION == 0x1080
			0x0008026800010000 // 01.00.08.0616
#elif PLATFORM_VERSION == 0x1090
			0x0009026C00010000 // 01.00.09.0620
#endif
			;

			if (!memcmp(CAST_PTR(__int64*, &GET_PTR2(version_info)->dwFileVersionMS), 
				&version, sizeof(version))) return true;

			return false;
		}

		static Enums::version_result_code GetVersionResultCode()
		{
			Enums::version_result_code result_code = Enums::_version_result_code_invalid;

			char dir[MAX_PATH];
			GetCurrentDirectory(NUMBEROF(dir), dir);

			char name[64];
			GetModuleBaseName(GetCurrentProcess(), GetModuleHandle(NULL), name, NUMBEROF(name));
			_strlwr_s(name);

			// Ok, the warning message will get annoying after a while for sapien (and tool) users
			if ( strstr(name,"sapien") != NULL || strstr(name,"hobo") != NULL || 
				 strstr(name,"tool") != NULL )
				return Enums::_version_result_code_dx9_app;

			char warning[MAX_PATH*3]; // we print a buffer with MAX_PATH twice, so just in case...

			// if the application name doesn't include the game name in it, assume the worst
			if ( strstr(name,PLATFORM_VALUE("haloce","haloceded")) == NULL )
			{
#if PLATFORM_IS_USER && defined(DX_WRAPPER)
				sprintf_s(warning,
					"An application (%s) tried to run Yelo (Open Sauce) which is a plugin that is only compatible with Halo (CE) "
					"v" BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE) ". \n"
					"The plugin's interface to DirectX 9 will continue to run, but note that this is a kludge."
					"\n\n"
					"If you don't wish to see this message anymore, then remove this file to uninstall the plugin: \n"
					"%s\\d3d9.dll",
					name, dir);
#elif PLATFORM_IS_DEDI
				sprintf_s(warning,
					"An application (%s) tried to run Yelo (Open Sauce) which is a plugin that is only compatible with Halo Dedi (CE) "
					"v" BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE) ". \n"
					"Plugin will not be loaded, this is just warning.",
					name);
#endif
			}
			else if(IsVersionInfoValid())
			{
				_main_globals.enabled = true;
				result_code = Enums::_version_result_code_valid;
			}
			else
			{
#if PLATFORM_IS_USER && defined(DX_WRAPPER)
				sprintf_s(warning,
					"Yelo (Open Sauce) is a plugin that is only compatible with Halo (CE) v" BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE) ". Your version is incompatible."
					"\n\n"
					"If you wish to use Yelo, you must update by running this file: \n"
					"%s\\haloupdate.exe"
					"\n\n"
					"If you don't wish to use Yelo, then remove this file to uninstall it: \n"
					"%s\\d3d9.dll",
					dir, dir);
#elif PLATFORM_IS_DEDI
				sprintf_s(warning,
					"Yelo (Open Sauce) is a plugin that is only compatible with Halo Dedi (CE) v" BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE) ". Your version is incompatible."
					"\n\n"
					"If you wish to use Yelo, you must update or downgrade your haloded.exe file");
#endif
			}

			if(result_code == Enums::_version_result_code_invalid)
				MessageBox(NULL, warning, "Notice", MB_OK | MB_ICONWARNING);

			return result_code;
		}
	};
};



bool WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	static bool done = false;

	if( dwReason == DLL_PROCESS_ATTACH && 
		Yelo::Main::GetVersionResultCode() == Yelo::Enums::_version_result_code_invalid )
		return false;

	if(dwReason == DLL_PROCESS_ATTACH && !done)
	{
		Yelo::Main::YeloModuleHandle() = hModule;

#if PLATFORM_IS_USER && defined(DX_WRAPPER)
		if(!LoadDX9(&hModule))
		{
			char error[128];
			sprintf_s(error,
				"Yelo (Open Sauce) failed to load DirectX (or the DirectX proxy)."
				"\n\n"
				"Nothing left to do but crash now, good bye!");
			MessageBox(NULL, error, "Oh noes!", MB_OK | MB_ICONERROR);
			return false;
		}
#endif

		if(Yelo::Main::IsYeloEnabled())
		{
			Yelo::Main::s_project_component* components;
			const Yelo::int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(Yelo::int32 x = 0; x <= component_count; x++)
				components[x].Initialize();
		}
		done = true;
	}
	else if(dwReason == DLL_PROCESS_DETACH && done)
	{
		if(Yelo::Main::IsYeloEnabled())
		{
			Yelo::Main::s_project_component* components;
			const Yelo::int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(Yelo::int32 x = component_count; x >= 0; x--)
 				components[x].Dispose();
		}

#ifdef API_DEBUG_MEMORY
		DumpAllocatedMemory();
#endif
		Yelo::Main::YeloModuleHandle() = NULL;

		done = false;

#if PLATFORM_IS_USER && defined(DX_WRAPPER)
		FreeDX9(hModule);
#endif
	}

	return true;
}