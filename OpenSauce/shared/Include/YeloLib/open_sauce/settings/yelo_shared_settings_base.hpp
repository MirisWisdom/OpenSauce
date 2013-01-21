/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Settings
	{
		// Define in the specific OS implementation
		extern cstring K_DEBUG_FILENAME;
		// Define in the specific OS implementation
		extern cstring K_EVENT_LOG_FILENAME;

		typedef void (API_FUNC* proc_settings_read)(TiXmlElement* element);
		typedef void (API_FUNC* proc_settings_save)(TiXmlElement* element);


		// Parses [file_path] into [doc], returning the element named [branch_name] 
		// or NULL if parsing fails
		TiXmlElement* GenericSettingsParse(cstring engine_name, TiXmlDocument& doc, char file_path[MAX_PATH], cstring branch_name);
		// Writes the XML header to [doc]. Also adds the engine id attribute data. 
		// Adds a new element named [branch_name] and returns its object data or NULL if any of this fails
		TiXmlElement* GenericSettingsWrite(cstring engine_name, TiXmlDocument& doc, cstring root_name, cstring branch_name);

		// True: "true", "on", "1"
		// False: "false", "off", "0", str == NULL
		bool ParseBoolean(cstring str);

		// Parses an element for xyz or ijk attributes
		void XmlReadReal3D(const TiXmlElement* element, real* r3d, const bool is_vector = false);
		// Writes xyz or ijk attributes to an element
		void XmlWriteReal3D(TiXmlElement* element, const real* r3d, const bool is_vector = false);
	};
};