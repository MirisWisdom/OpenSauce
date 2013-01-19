/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/GameSpyApi.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_GAMESPY_API
#include "Memory/_EngineLayout.inl"

	namespace Networking
	{
		bool s_gamespy_qr2_keybuffer::add(Enums::gamespy_qr_field keyid)
		{
			if(numkeys >= Enums::k_max_gamespy_qr_registered_keys)
				return false;
			if(	keyid <= Enums::_gamespy_qr_field_reserved || 
				keyid > Enums::k_max_gamespy_qr_registered_keys)
				return false;

			keys[numkeys++] = keyid;
			return true;
		}

		bool s_gamespy_qr2_buffer::add(cstring value)
		{
			int32 copylen = (int32)strlen(value)+1;
			if(copylen > NUMBEROF(buffer))
				copylen = NUMBEROF(buffer);
			if(copylen <= 0)
				return false;

			memcpy_s(buffer+len, NUMBEROF(buffer)-len, value, (size_t)copylen);
			len += copylen;
			buffer[len-1] = '\0';

			return true;
		}
		bool s_gamespy_qr2_buffer::add(int32 value)
		{
			char buffer[20];
			sprintf_s(buffer, "%d", value);
			return add(buffer);
		}

		s_gamespy_socket* GsSocket()										DPTR_IMP_GET(gs_Socket);
		s_gamespy_socket* GsLoopbackSocket()								DPTR_IMP_GET(gs_LoopbackSocket);
		s_gamespy_config* GsConfig()										PTR_IMP_GET2(gamespy_config);
		s_gamespy_globals* GsGlobals()										PTR_IMP_GET2(gamespy_globals);
#if PLATFORM_IS_USER											// While the getter for this is only in User builds, I keep the definition around just for documentation's sake
		s_gamespy_server_browser_globals* GsServerBrowserGlobals()			PTR_IMP_GET2(gamespy_server_browser_globals);
		static bool* GsPatchCheckForUpdates()								PTR_IMP_GET2(g_gamespy_patch_check_for_updates);
#endif
		s_gamespy_qr2* GsQr2()												DPTR_IMP_GET(gamespy_qr2);

		s_gamespy_product* GsProducts()										PTR_IMP_GET2(gamespy_products_list);

		API_FUNC_NAKED s_gamespy_client* GsGetClient(int32 client_id)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH);

			API_FUNC_NAKED_START()
				push	ecx

				mov		eax, client_id
				call	TEMP_CALL_ADDR
				cmp		eax, GET_DATA_PTR(compiler_null_string)
				jnz		_return
				mov		eax, 4 // will cause us to return NULL. Yes, I r a hacka
				
_return:
				sub		eax, 4

				pop		ecx
			API_FUNC_NAKED_END(1)
		}


		namespace GameSpy
		{
			game_build_string_t& GetGameVer()
			{
				static game_build_string_t g_game_ver;

				return g_game_ver;
			}

			static void Qr2GetGameVer(char* buffer)
			{
				strcpy_s(buffer, 0x100, GetGameVer());
			}

			static int PLATFORM_API Qr2StringMatchesGameVer(const char* buffer)
			{
				return strcmp(buffer, GetGameVer())==0;
			}

			static int32 gamespy_patch_check_for_updates_sans_check()
			{
				return GsConfig()->check_for_updates_status = Enums::_gamespy_update_status_no_update;
			}
			void TurnOffUpdateCheck()
			{
#if PLATFORM_IS_USER
				*GsPatchCheckForUpdates() = false; // pretend we've checked for updates already

				Memory::WriteRelativeCall(&gamespy_patch_check_for_updates_sans_check, 
					GET_DATA_VPTR(GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL));
#endif
			}

			struct s_gamespy_yelo_settings {
				bool no_update_check;
				PAD24;
			}g_gamespy_yelo_settings;

			static void InitializeForNewQr2()
			{
				// TODO: override key callbacks
			}
			void Initialize()
			{
				// TODO: populate GetGameVer()

				if(g_gamespy_yelo_settings.no_update_check)
					TurnOffUpdateCheck();

				Memory::CreateHookRelativeCall(&InitializeForNewQr2, 
					GET_FUNC_VPTR(CREATE_GAMESPY_QR2_HOOK), Enums::_x86_opcode_ret);

#if FALSE // TODO
				qr2_register_key(Enums::_gamespy_qr_field_open_sauce_version, "os_ver");
				qr2_register_key(Enums::_gamespy_qr_field_open_sauce_map, "os_map");
#endif
			}

			void Dispose()
			{
			}

			void LoadSettings(TiXmlElement* xml_element)
			{
				if(xml_element == NULL) return;

				g_gamespy_yelo_settings.no_update_check = 
					Settings::ParseBoolean(xml_element->Attribute("gsNoUpdateCheck"));
			}
			void SaveSettings(TiXmlElement* xml_element)
			{
				xml_element->SetAttribute("gsNoUpdateCheck", 
					BooleanToString(g_gamespy_yelo_settings.no_update_check));
			}

			API_FUNC_NAKED void qr2_register_key(Enums::gamespy_qr_field keyid, cstring key)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(QR2_REGISTER_KEY);

				API_FUNC_NAKED_START()
					push	key
					push	keyid
					call	FUNCTION
				API_FUNC_NAKED_END_CDECL(2)
			}

			API_FUNC_NAKED _enum	SBServerGetBoolValue(s_gamespy_server* server, const char* key, _enum def)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(SBSERVER_GET_BOOL_VALUE);

				API_FUNC_NAKED_START()
					push	def
					push	key
					push	server
					call	FUNCTION
				API_FUNC_NAKED_END_CDECL(3)
			}
			API_FUNC_NAKED int		SBServerGetIntValue(s_gamespy_server* server, const char* key, int def)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(SBSERVER_GET_INT_VALUE);

				API_FUNC_NAKED_START()
					push	def
					push	key
					push	server
					call	FUNCTION
				API_FUNC_NAKED_END_CDECL(3)
			}
			API_FUNC_NAKED cstring	SBServerGetStringValue(s_gamespy_server* server, const char* key, cstring def)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(SBSERVER_GET_STRING_VALUE);

				API_FUNC_NAKED_START()
					push	def
					push	key
					push	server
					call	FUNCTION
				API_FUNC_NAKED_END_CDECL(3)
			}
		};
	};
};