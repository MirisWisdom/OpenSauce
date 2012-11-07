/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/open_sauce/settings/yelo_shared_settings_base.hpp>

namespace Yelo
{
	namespace Settings
	{
#if PLATFORM_IS_EDITOR
		cstring K_DEBUG_FILENAME = "Editor.debug.log";
		cstring K_EVENT_LOG_FILENAME = "Editor.Log.txt";
#else
		cstring K_DEBUG_FILENAME = "Game.debug.log";
		cstring K_EVENT_LOG_FILENAME = "Game.Log.txt";
#endif


		TiXmlElement* GenericSettingsParse(cstring engine_name, TiXmlDocument& doc, char file_path[MAX_PATH], cstring branch_name)
		{
			TiXmlElement* result = NULL;
			if(doc.LoadFile(file_path))
			{
				TiXmlElement* root_element = doc.RootElement();

				if(root_element != NULL)
				{
					cstring value = root_element->Attribute("engine");

					if(value != NULL && !strcmp(value, engine_name))
						result = branch_name != NULL ?
							root_element->FirstChildElement(branch_name) : root_element;
				}
			}

			return result;
		}
		TiXmlElement* GenericSettingsWrite(cstring engine_name, TiXmlDocument& doc, cstring root_name, cstring branch_name)
		{
			TiXmlElement* result;

			TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
				doc.LinkEndChild(decl);

			TiXmlElement* root_element = new TiXmlElement(root_name);
				doc.LinkEndChild(root_element);

			root_element->SetAttribute("engine", engine_name);

			if(branch_name != NULL)
			{
				TiXmlElement* element = new TiXmlElement(branch_name);
					root_element->LinkEndChild(element);

				result = element;
			}
			else result = root_element;

			return result;
		}

		bool ParseBoolean(cstring str)
		{
			bool result = false;

			if(str != NULL)
			{
				if( !_stricmp(str, "false") |
					!_stricmp(str, "off") |
					!strcmp  (str, "0") )
					result = false;
				else if(!_stricmp(str, "true") |
						!_stricmp(str, "on") | 
						!strcmp  (str, "1") )
					result = true;
			}

			return result;
		}

		void XmlReadReal3D(const TiXmlElement* element, real* r3d, const bool is_vector)
		{
			r3d[0] = r3d[1] = r3d[2] = 0.0f;

			if(element != NULL)
			{
				element->QueryFloatAttribute(is_vector ? "i" : "x", &r3d[0]);
				element->QueryFloatAttribute(is_vector ? "j" : "y", &r3d[1]);
				element->QueryFloatAttribute(is_vector ? "k" : "z", &r3d[2]);
			}
		}

		void XmlWriteReal3D(TiXmlElement* element, const real* r3d, const bool is_vector)
		{
			element->SetDoubleAttribute(is_vector ? "i" : "x", r3d[0]);
			element->SetDoubleAttribute(is_vector ? "j" : "y", r3d[1]);
			element->SetDoubleAttribute(is_vector ? "k" : "z", r3d[2]);
		}
	};
};