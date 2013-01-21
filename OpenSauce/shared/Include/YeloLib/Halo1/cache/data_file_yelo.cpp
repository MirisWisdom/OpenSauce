/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/cache/data_file_yelo.hpp>

#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

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


#if PLATFORM_IS_EDITOR
	namespace Cache
	{
		cstring c_data_files::DataFileTypeToString(_enum df_type)
		{
			switch(df_type)
			{
			case Enums::_data_file_type_bitmaps:return "bitmaps";
			case Enums::_data_file_type_sounds:	return "sounds";
			case Enums::_data_file_type_locale:	return "loc";
			default:							return "InvalidDataFileType";
			}
		}

		void c_data_files::InitializeForCache(bool using_mod_sets, cstring mod_name, char maps_path[MAX_PATH])
		{
			memset(m_names, 0, sizeof(m_names));

			if(using_mod_sets)
			{
				strcat_s(maps_path, MAX_PATH, "data_files\\");
				_mkdir(maps_path);

				// Prefix the data_file names with our data files directory and mod name
				for(int32 x = 0; x < NUMBEROF(m_names); x++)
				{
					strcpy_s(m_names[x], "data_files\\");
					strcat_s(m_names[x], mod_name);
				}
				strcat_s(m_names[Enums::_data_file_type_bitmaps],	"-bitmaps");
				strcat_s(m_names[Enums::_data_file_type_sounds],	"-sounds");
				strcat_s(m_names[Enums::_data_file_type_locale],	"-loc");
			}
			else	// we're using stock data files
			{
				strcat_s(m_names[Enums::_data_file_type_bitmaps],	"bitmaps");
				strcat_s(m_names[Enums::_data_file_type_sounds],	"sounds");
				strcat_s(m_names[Enums::_data_file_type_locale],	"loc");
			}
		}

		void c_data_files::CopyStockDataFile(cstring maps_path, _enum df_type)
		{
			#pragma region s_progress_report
			struct s_progress_report {
				bool stage_completed[100/25];
				BOOL cancel;

				static DWORD CALLBACK CopyProgressRoutine(
					LARGE_INTEGER TotalFileSize,	LARGE_INTEGER TotalBytesTransferred,
					LARGE_INTEGER StreamSize,		LARGE_INTEGER StreamBytesTransferred,
					DWORD dwStreamNumber,
					DWORD dwCallbackReason,
					HANDLE hSourceFile,	HANDLE hDestinationFile,
					LPVOID lpData)
				{
					s_progress_report* report = CAST_PTR(s_progress_report*, lpData);

					if(dwCallbackReason == CALLBACK_CHUNK_FINISHED)
					{
						LONGLONG percent = (LONGLONG)(((double)TotalBytesTransferred.QuadPart / (double)TotalFileSize.QuadPart)*100.0);

						if(percent >= 25 && !report->stage_completed[0])
						{
							printf_s("\t25%%...\n");
							report->stage_completed[0] = true;
						}
						else if(percent >= 50 && !report->stage_completed[1])
						{
							printf_s("\t50%%...\n");
							report->stage_completed[1] = true;
						}
						else if(percent >= 75 && percent < 100 && !report->stage_completed[2])
						{
							printf_s("\t75%%...\n");
							report->stage_completed[2] = true;
						}
					}

					return PROGRESS_CONTINUE;
				}
			};
			#pragma endregion

			s_progress_report report;
			memset(&report, 0, sizeof(report));

			cstring data_file_name = DataFileTypeToString(df_type);

			char source_file[MAX_PATH];	sprintf_s(source_file, "%s%s.map", maps_path, data_file_name);
			char target_file[MAX_PATH];	sprintf_s(target_file, "%s%s.map", maps_path, m_names[df_type]);

			printf_s("copying %s...\n", data_file_name);
			{
				if( !CopyFileExA(source_file,	target_file, 
					&s_progress_report::CopyProgressRoutine, &report, &report.cancel, 0) )
					printf_s("failed to copy! reason: %X\n", GetLastError());
			}
		}

		void c_data_files::CopyStock()
		{
			char maps_path[MAX_PATH];
			strcpy_s(maps_path, Settings::Get().GetMapsPath());

			CopyStockDataFile(maps_path, Enums::_data_file_type_bitmaps);
			CopyStockDataFile(maps_path, Enums::_data_file_type_sounds);
			CopyStockDataFile(maps_path, Enums::_data_file_type_locale);

			printf_s("done\n");
		}
	};
#endif
};