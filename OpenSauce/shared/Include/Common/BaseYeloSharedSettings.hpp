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

namespace Yelo
{
	namespace Settings
	{
		// Parses [file_path] into [doc], returning the element named [branch_name] 
		// or NULL if parsing fails
		TiXmlElement* GenericSettingsParse(cstring engine_name, TiXmlDocument& doc, char file_path[MAX_PATH], cstring branch_name);
		// Writes the XML header to [doc]. Also adds the engine id attribute data. 
		// Adds a new element named [branch_name] and returns its object data or NULL if any of this fails
		TiXmlElement* GenericSettingsWrite(cstring engine_name, TiXmlDocument& doc, cstring root_name, cstring branch_name);

		// True: "true", "on", "1"
		// False: "false", "off", "0", str == NULL
		bool ParseBoolean(cstring str);

		API_INLINE cstring BooleanToString(bool value)
		{
			return value ? "true" : "false";
		}

		// Parses an element for xyz or ijk attributes
		void XmlReadReal3D(const TiXmlElement* element, real* r3d, const bool is_vector = false);
		// Writes xyz or ijk attributes to an element
		void XmlWriteReal3D(TiXmlElement* element, const real* r3d, const bool is_vector = false);
	};
};