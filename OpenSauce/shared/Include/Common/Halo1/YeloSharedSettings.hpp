/*
    Yelo: Open Sauce SDK

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
#pragma once

#include <Common/BaseYeloSharedSettings.hpp>
#include <Common/Halo1/YeloSettingsDefinitions.hpp>

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

		// Path to the systems common application data folder
		cstring CommonAppDataPath();
		cstring UserProfilePath();
		// 'savegames' path
		cstring UserSavedProfilesPath();
		// The OpenSauce path to use that is under the User's game profile
		cstring OpenSauceProfilePath();
		// Path which we store our reports in
		cstring ReportsPath();
		// The current working directory
		cstring WorkingDirectoryPath();

		void SharedInitialize(cstring profile_path);
		void SharedDispose();


		bool PlayerProfileRead(cstring profile_name, __out byte profile[Enums::k_player_profile_buffer_size]);

		// Format the settings path & [filename] into [file_path].
		// Returns: true if [file_path] exists on disk
		bool GetSettingsFilePath(cstring filename, __out char file_path[MAX_PATH]);

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


		// Parses [file_path] into [doc], returning the element named [branch_name] 
		// or NULL if parsing fails
		TiXmlElement* GenericSettingsParse(TiXmlDocument& doc, char file_path[MAX_PATH], cstring branch_name);
		// Writes the XML header to [doc]. Also adds the engine id attribute data. 
		// Adds a new element named [branch_name] and returns its object data or NULL if any of this fails
		TiXmlElement* GenericSettingsWrite(TiXmlDocument& doc, cstring root_name, cstring branch_name);

		void ReplaceEnvironmentVariable(std::string& parse_string, const char* variable, const char* value);
		void ParseEnvironmentVariables(std::string& parse_string);
	};
};