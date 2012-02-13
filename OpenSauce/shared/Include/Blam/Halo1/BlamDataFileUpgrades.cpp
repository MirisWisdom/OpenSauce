/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include <Common/Precompile.hpp>
#include <Blam/Halo1/BlamDataFileUpgrades.hpp>

#include <Common/Halo1/YeloSharedSettings.hpp>

namespace Yelo
{
	namespace DataFiles
	{
		static cstring K_SETTINGS_FILENAME_XML = "OS_Settings.ModSets.xml";

		struct s_globals {

			struct s_mod {
				int32 Count;
				struct {
					char Name[MAX_PATH];
					char Bitmaps[MAX_PATH];
					char Sounds[MAX_PATH];
					char Locale[MAX_PATH];

					void Initialize()
					{
						strcpy_s(Bitmaps,	"data_files\\");
						strcpy_s(Sounds,	"data_files\\");
						strcpy_s(Locale,	"data_files\\");

						strcat_s(Bitmaps,	Name);
						strcat_s(Sounds,	Name);
						strcat_s(Locale,	Name);

						strcat_s(Bitmaps,	"-bitmaps");
						strcat_s(Sounds,	"-sounds");
						strcat_s(Locale,	"-loc");
					}
				}FileSet[Enums::k_engine_mod_max_data_file_sets];

				bool LoadSettings()
				{
					TiXmlElement* root = NULL;
					TiXmlDocument xml_settings = TiXmlDocument();

					char file_path[MAX_PATH];
					if(!Settings::GetSettingsFilePath(K_SETTINGS_FILENAME_XML, file_path))
						return false;
					else if( (root = Settings::GenericSettingsParse(xml_settings, file_path, "modSets")) == NULL)
						return false;

					for(TiXmlElement* entry = root->FirstChildElement();
						this->Count < Enums::k_engine_mod_max_data_file_sets && entry != NULL;
						entry = entry->NextSiblingElement())
					{
						cstring name = entry->Attribute("name");

						if(name != NULL)
						{
							strcpy_s(this->FileSet[this->Count].Name, name);
							this->FileSet[this->Count++].Initialize();
						}
					}

					return true;
				}

#if PLATFORM_IS_EDITOR
				void SaveSettings()
				{
					char file_path[MAX_PATH];
					Settings::GetSettingsFilePath(K_SETTINGS_FILENAME_XML, file_path);
					TiXmlDocument xml_settings(file_path);
					TiXmlElement* root = Settings::GenericSettingsWrite(xml_settings, "osModSetSettings", "modSets");

					for(int32 x = 0; x < this->Count; x++)
					{
						TiXmlElement* entry = new TiXmlElement("ModSet");
							root->LinkEndChild(entry);

						entry->SetAttribute("name", this->FileSet[x].Name);
					}

					xml_settings.SaveFile(file_path);
				}

				errno_t AddFileSet(cstring name)
				{
					bool fs_exists = false;
					for(int32 x = 0; x < this->Count && !fs_exists; x++)
						fs_exists = !strcmp(name, this->FileSet[x].Name);

					if(fs_exists) return EEXIST;

					if(this->Count == Enums::k_engine_mod_max_data_file_sets)
						return ENOSPC;

					strcpy_s(this->FileSet[this->Count++].Name, name);

					return k_errnone;
				}
#endif

				bool FindSet(cstring name, 
					const char (*&bitmaps)[MAX_PATH],
					const char (*&sounds)[MAX_PATH],
					const char (*&locale)[MAX_PATH]
					)
				{
					for(int32 x = 0; x < this->Count; x++)
						if( !strcmp(name, this->FileSet[x].Name) )
						{
							bitmaps = &FileSet[x].Bitmaps;
							sounds = &FileSet[x].Sounds;
							locale = &FileSet[x].Locale;
							return true;
						}

					bitmaps = sounds = locale = NULL;
					return false;
				}
			}Mods;

		}Globals;


		bool FindSet(cstring name, 
			const char (*&bitmaps)[MAX_PATH],
			const char (*&sounds)[MAX_PATH],
			const char (*&locale)[MAX_PATH]
			)
		{			
			return Globals.Mods.FindSet(name, bitmaps, sounds, locale);
		}

		bool VerifySetFilesExist(cstring bitmaps, cstring sounds, cstring locale,
			char details_buffer[MAX_PATH])
		{			
			// The names given to use are raw data-set names that don't include the extension or maps folder path
			cstring set_files[3] = {
				bitmaps, sounds, locale
			};
			for(int x = 0; x < NUMBEROF(set_files); x++)
			{
				// TODO: We assume the data files are stored within the maps directory
				sprintf_s(details_buffer, MAX_PATH, "maps\\%s.map", set_files[x]);

				int access = _access_s(details_buffer, 0);
				if(access == ENOENT) return false;
			}

			return true;
		}

#if PLATFORM_IS_EDITOR
		void SaveSettings()
		{
			Globals.Mods.SaveSettings();
		}

		errno_t AddFileSet(cstring name)
		{
			return Globals.Mods.AddFileSet(name);
		}
#endif

		void SharedInitialize()
		{
			Globals.Mods.LoadSettings();
		}

		void SharedDispose()
		{
		}
	};
};