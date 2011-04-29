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
#include "Common/Precompile.hpp"
#include <Common/Halo1/YeloSharedSettings.hpp>

#include <direct.h>
#include <share.h>

namespace Yelo
{
	namespace Settings
	{
		static struct {
			char CommonAppDataPath[MAX_PATH];
			char UserProfilePath[MAX_PATH];
			char OpenSauceProfilePath[MAX_PATH];
			char ReportsPath[MAX_PATH];
			char WorkingDirectoryPath[MAX_PATH];
		}Internal;

		cstring CommonAppDataPath()		{ return Internal.CommonAppDataPath; }
		cstring UserProfilePath()		{ return Internal.UserProfilePath; }
		cstring OpenSauceProfilePath()	{ return Internal.OpenSauceProfilePath; }
		cstring ReportsPath()			{ return Internal.ReportsPath; }
		cstring WorkingDirectoryPath()	{ return Internal.WorkingDirectoryPath; }

		void SharedInitialize(cstring profile_path)
		{
			SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, Internal.CommonAppDataPath);
			PathAppendA(Internal.CommonAppDataPath, "Kornner Studios\\Halo CE\\");

			if(profile_path[0] == '\0')
			{
				SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, Internal.UserProfilePath);
				PathAppendA(Internal.UserProfilePath, "My Games\\Halo CE\\");
				profile_path = Internal.UserProfilePath;
			}
			else strcpy_s(Internal.UserProfilePath, profile_path);

			strcpy_s(Internal.OpenSauceProfilePath, profile_path);
			PathAppendA(Internal.OpenSauceProfilePath, "OpenSauce\\");
			_mkdir(Internal.OpenSauceProfilePath); // make the OpenSauce subdirectory

			strcpy_s(Internal.ReportsPath, Internal.OpenSauceProfilePath);
			PathAppendA(Internal.ReportsPath, "Reports\\");
			_mkdir(Internal.ReportsPath);

			_getcwd(Internal.WorkingDirectoryPath, MAX_PATH);
		}

		void SharedDispose()
		{
		}

		//////////////////////////////////////////////////////////////////////////

		bool GetSettingsFilePath(cstring filename, __out char file_path[MAX_PATH])
		{
			sprintf_s(file_path, MAX_PATH, "%s%s", Internal.OpenSauceProfilePath, filename);

			return PathFileExistsA(file_path) > 0;
		}

		FILE* CreateReport(cstring filename, bool append, bool text, bool shared)
		{
			char path[MAX_PATH];
			memset(path, 0, sizeof(path));

			strcpy_s(path, ReportsPath());
			strcat_s(path, filename);

			cstring mode;
			if(text)mode = append ? "at" : "wt";
			else	mode = append ? "ab" : "wb";

			FILE* ret = NULL;
			if(!shared)
				fopen_s(&ret, path, mode);
			else
				return _fsopen(filename, mode, _SH_DENYNO);

			return ret;
		}

		FILE* CreateUnicodeReport(wcstring filename, bool append, bool shared)
		{
			wchar_t path[MAX_PATH];
			memset(path, 0, sizeof(path));

			string_to_wstring(path, NUMBEROF(path), ReportsPath());
			wcscat_s(path, filename);

			wcstring mode = append ? L"at,ccs=UNICODE" : L"wt,ccs=UNICODE";

			FILE* ret = NULL;
			if(!shared)
				_wfopen_s(&ret, path, mode);
			else
				return _wfsopen(path, mode, _SH_DENYNO);

			return ret;
		}

		FILE* OpenSettings(cstring filename, bool text, bool open_for_writing)
		{
			char path[MAX_PATH];
			memset(path, 0, sizeof(path));

			strcpy_s(path, Internal.OpenSauceProfilePath);
			strcat_s(path, filename);

			cstring mode;
			if(open_for_writing)mode = text ? "wt" : "wb";
			else				mode = text ? "rt" : "rb";

			FILE* ret = NULL;
			fopen_s(&ret, path, mode);

			return ret;
		}


		TiXmlElement* GenericSettingsParse(TiXmlDocument& doc, char file_path[MAX_PATH], cstring branch_name)
		{
			return GenericSettingsParse("Halo1_CE", doc, file_path, branch_name);
		}
		TiXmlElement* GenericSettingsWrite(TiXmlDocument& doc, cstring root_name, cstring branch_name)
		{
			return GenericSettingsWrite("Halo1_CE", doc, root_name, branch_name);
		}
	};
};