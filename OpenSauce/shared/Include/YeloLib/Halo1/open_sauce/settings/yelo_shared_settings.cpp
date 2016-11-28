/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>

#include <direct.h>
#include <YeloLib/files/files.hpp>
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

namespace Yelo
{
	namespace Settings
	{
		cstring K_USER_FILENAME_XML = "OS_Settings.User.xml";
		cstring K_SERVER_FILENAME_XML = "OS_Settings.Server.xml";
		cstring K_EDITOR_FILENAME_XML = "OS_Settings.Editor.xml";

		static struct {
			char CommonAppDataPath[MAX_PATH];
			char UserProfilePath[MAX_PATH];
			char UserSavedProfilesPath[MAX_PATH];
			char UserProfileMapsPath[MAX_PATH];
			char OpenSauceProfilePath[MAX_PATH];
			char ReportsPath[MAX_PATH];
			char WorkingDirectoryPath[MAX_PATH];
		}Internal;

		cstring CommonAppDataPath()		{ return Internal.CommonAppDataPath; }
		cstring UserProfilePath()		{ return Internal.UserProfilePath; }
		cstring UserSavedProfilesPath()	{ return Internal.UserSavedProfilesPath; }
		cstring UserProfileMapsPath()	{ return Internal.UserProfileMapsPath; }
		bool	UserProfileMapsPathExists()	{ return FileIO::PathExists(Internal.UserProfileMapsPath); }
		cstring OpenSauceProfilePath()	{ return Internal.OpenSauceProfilePath; }
		cstring ReportsPath()			{ return Internal.ReportsPath; }
		cstring WorkingDirectoryPath()	{ return Internal.WorkingDirectoryPath; }

		cstring PlatformUserMapsPath()
		{
#if PLATFORM_IS_EDITOR
			return Settings::c_settings_cheape::Profile().GetMapsPath();
#else
			return UserProfileMapsPath();
#endif
		}

		void SharedInitialize(cstring profile_path)
		{
			SHGetFolderPath(nullptr, CSIDL_COMMON_APPDATA, nullptr, 0, Internal.CommonAppDataPath);
			PathAppendA(Internal.CommonAppDataPath, "Kornner Studios\\Halo CE\\");

			if(profile_path[0] == '\0')
			{
				SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, 0, Internal.UserProfilePath);
				PathAppendA(Internal.UserProfilePath, "My Games\\Halo CE\\");
				profile_path = Internal.UserProfilePath;
			}
			else strcpy_s(Internal.UserProfilePath, profile_path);

			strcpy_s(Internal.UserSavedProfilesPath, Internal.UserProfilePath);
			PathAppendA(Internal.UserSavedProfilesPath, "savegames\\");

			strcpy_s(Internal.UserProfileMapsPath, Internal.UserProfilePath);
			PathAppendA(Internal.UserProfileMapsPath, Cache::K_MAP_FILES_DIRECTORY);

			strcpy_s(Internal.OpenSauceProfilePath, profile_path);
			PathAppendA(Internal.OpenSauceProfilePath, "OpenSauce\\");
			_mkdir(Internal.OpenSauceProfilePath); // make the OpenSauce subdirectory

			strcpy_s(Internal.ReportsPath, Internal.OpenSauceProfilePath);
			PathAppendA(Internal.ReportsPath, "Reports\\");
			_mkdir(Internal.ReportsPath);

			_getcwd(Internal.WorkingDirectoryPath, MAX_PATH);
			const char* string_end = strrchr(Internal.WorkingDirectoryPath, '\0');
			string_end--;
			// not using PathAppendA as it does not append empty paths
			if(string_end[0] != '\\')
				strcat_s(Internal.WorkingDirectoryPath, sizeof(Internal.WorkingDirectoryPath), "\\");
		}

		void SharedDispose()
		{
		}

		//////////////////////////////////////////////////////////////////////////

		bool PlayerProfileRead(cstring profile_name, _Out_ byte profile[Enums::k_player_profile_buffer_size])
		{
			bool success = false;
			memset(profile, 0, Enums::k_player_profile_buffer_size);

			char blam_path[MAX_PATH];
			success =			k_errnone == strcpy_s(blam_path, Internal.UserSavedProfilesPath);
			success = success&& k_errnone == strcat_s(blam_path, profile_name);
			success = success&& k_errnone == strcat_s(blam_path, "\\blam.sav");

			if (success && FileIO::PathExists(blam_path))
			{
				FILE* file = nullptr;
				fopen_s(&file, blam_path, "rb");

				success = fread_s(profile, Enums::k_player_profile_buffer_size, Enums::k_player_profile_buffer_size, 1, file) == 1;

				fclose(file);
			}

			return success;
		}

		bool GetSettingsFilePath(cstring filename, _Out_ char file_path[MAX_PATH])
		{
			sprintf_s(file_path, MAX_PATH, "%s%s", Internal.OpenSauceProfilePath, filename);

			return PathFileExistsA(file_path) != FALSE;
		}

		FILE* CreateReport(cstring filename, bool append, bool text, bool shared)
		{
			FILE* file = nullptr;
			bool success = false;

			char path[MAX_PATH];
			memset(path, 0, sizeof(path));

			success =			k_errnone == strcpy_s(path, ReportsPath());
			success = success&& k_errnone == strcat_s(path, filename);

			cstring mode;
			if(text)mode = append ? "at" : "wt";
			else	mode = append ? "ab" : "wb";

			if (success)
			{
				if (!shared)
					success = k_errnone == fopen_s(&file, path, mode);
				else
					file = _fsopen(filename, mode, _SH_DENYNO);
			}

			return file;
		}

		FILE* CreateUnicodeReport(wcstring filename, bool append, bool shared)
		{
			FILE* file = nullptr;
			bool success = false;

			wchar_t path[MAX_PATH];
			memset(path, 0, sizeof(path));

			success =			nullptr != string_to_wstring(path, NUMBEROF(path), ReportsPath());
			success = success&& k_errnone == wcscat_s(path, filename);

			wcstring mode = append ? L"at,ccs=UNICODE" : L"wt,ccs=UNICODE";

			if (success)
			{
				if (!shared)
					success = k_errnone == _wfopen_s(&file, path, mode);
				else
					file = _wfsopen(path, mode, _SH_DENYNO);
			}

			return file;
		}

		FILE* OpenSettings(cstring filename, bool text, bool open_for_writing)
		{
			FILE* file = nullptr;
			bool success = false;

			char path[MAX_PATH];
			memset(path, 0, sizeof(path));

			success =			k_errnone == strcpy_s(path, Internal.OpenSauceProfilePath);
			success = success&& k_errnone == strcat_s(path, filename);

			cstring mode;
			if(open_for_writing)mode = text ? "wt" : "wb";
			else				mode = text ? "rt" : "rb";

			success = success&& k_errnone == fopen_s(&file, path, mode);

			return file;
		}

		void ReplaceEnvironmentVariable(std::string& parse_string, const char* variable, const char* value)
		{
			if(!variable || !value)
				return;

			size_t var_len = strlen(variable);
			size_t offset;
			while((offset = parse_string.find(variable, 0)) != std::string::npos)
				parse_string.replace(offset, var_len, value);
		}
		void ParseEnvironmentVariables(std::string& parse_string)
		{
			ReplaceEnvironmentVariable(parse_string, "$(CommonAppData)", Settings::CommonAppDataPath());
			ReplaceEnvironmentVariable(parse_string, "$(UserProfile)", Settings::UserProfilePath());
			ReplaceEnvironmentVariable(parse_string, "$(UserSavedProfiles)", Settings::UserSavedProfilesPath());
			ReplaceEnvironmentVariable(parse_string, "$(OpenSauceProfile)", Settings::OpenSauceProfilePath());
			ReplaceEnvironmentVariable(parse_string, "$(Reports)", Settings::ReportsPath());
			ReplaceEnvironmentVariable(parse_string, "$(WorkingDirectory)", Settings::WorkingDirectoryPath());
		}
	};
};