/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/data_file.hpp>

namespace Yelo
{
	namespace DataFiles
	{
	};

	namespace Cache
	{
		class c_data_files_name_utils
		{
		public:
			enum {
				k_name_length = 63,

				// maximum length of the filename suffix on mod-set files
				// minus one, as null terminator is counted by NUMBEROF
				k_max_mod_set_name_suffix_length = NUMBEROF("-bitmaps") - 1,
				// maximum length of the actual mod-set name
				k_max_mod_set_name_length = Enums::k_tag_string_length - k_max_mod_set_name_suffix_length,
			};


			// Names are all relative to the "maps\" directory (wherever it may be)
			char m_names[Enums::k_number_of_data_file_types][k_name_length+1];

			// validate and fix the mod name if it is too long for a tag_string
			static void SanitizeModSetName(_Inout_ tag_string& mod_set_name);

		protected:
			static void BuildName(cstring mod_name, Enums::data_file_type type,
				char name[k_name_length+1]);

			void BuildNames(cstring mod_name);
		};

		class c_data_files_finder : protected c_data_files_name_utils
		{
			struct {
				cstring environment; // The 'normal' path, usually a folder relative to the EXE or CWD
				cstring user_profile;// The path defined by or in the user's profile

				cstring final; // The path which the finder first found all the data files in, or NULL if it couldn't
			}m_maps_path;

			bool SearchPath(cstring maps_path);
			bool SearchEnvironment();
			bool SearchUserProfile();
		public:
			c_data_files_finder(cstring mod_name = "");

			bool TryAndFind(std::string& bitmaps_path, std::string& sounds_path, std::string& locale_path);
		};

#if PLATFORM_IS_EDITOR && PLATFORM_TYPE == PLATFORM_TOOL
		class c_data_files : public c_data_files_name_utils
		{
		public:
			// Initialize the data file system to either use a mod-set or the stock 
			// data files.
			// NOTE: [maps_path] will be modified if [using_mod_sets] is true!
			void InitializeForCache(bool using_mod_sets, cstring mod_name, char maps_path[MAX_PATH]);

		private:
			void CopyStockDataFile(cstring maps_path, Enums::data_file_type df_type);
		public:
			// Copy the stock data files that come with the game. These copies will then be used as 
			// the base input data files for the cache being built
			void CopyStock();
		};
#endif
	};
};