/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/open_sauce/settings/yelo_shared_settings_base.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_settings_definitions.hpp>

#include <string>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_player_profile_buffer_size = 0x1FFC,
		};
	};

	namespace Settings
	{
		extern cstring K_USER_FILENAME_XML;
		extern cstring K_SERVER_FILENAME_XML;
		extern cstring K_EDITOR_FILENAME_XML;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Path to the systems common application data folder. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring CommonAppDataPath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	User profile path. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring UserProfilePath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	User's saved profiles path. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring UserSavedProfilesPath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Maps folder under the user's profile. </summary>
		///
		/// <remarks>
		/// 	Use UserProfileMapsPathExists to check the folder exists before interacting iwth it.
		/// </remarks>
		///
		/// <returns>	A cstring. </returns>
		cstring UserProfileMapsPath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Queries if a user's profile maps path exists. </summary>
		///
		/// <returns>	true if it exists, false if it does not. </returns>
		bool	UserProfileMapsPathExists();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The OpenSauce path to use that is under the User's game profile. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring OpenSauceProfilePath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Path which we store our reports in. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring ReportsPath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	The current working directory. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring WorkingDirectoryPath();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	User's maps\ path for the defined PLATFORM_TYPE. </summary>
		///
		/// <returns>	A cstring. </returns>
		cstring PlatformUserMapsPath();

		void SharedInitialize(cstring profile_path);
		void SharedDispose();


		bool PlayerProfileRead(cstring profile_name, _Out_ byte profile[Enums::k_player_profile_buffer_size]);

		// Format the settings path & [filename] into [file_path].
		// Returns: true if [file_path] exists on disk
		bool GetSettingsFilePath(cstring filename, _Out_ char file_path[MAX_PATH]);

		// Open a report file for writing
		// [append] - keep existing report data
		// [text] - are we writing textual or binary data?
		// [shared] - should the report be accessible while halo is running?
		FILE* CreateReport(cstring filename, bool append, bool text, bool shared = false);

		// Open a report file for writing unicode strings
		// [append] - keep existing report data
		// [shared] - should the report be accessible while halo is running?
		FILE* CreateUnicodeReport(wcstring filename, bool append, bool shared = false);

		// Open a settings file (for reading)
		// [text] - are we reading textual or binary data?
		FILE* OpenSettings(cstring filename, bool text, bool open_for_writing = false);

		void ReplaceEnvironmentVariable(std::string& parse_string, const char* variable, const char* value);
		void ParseEnvironmentVariables(std::string& parse_string);
	};
};