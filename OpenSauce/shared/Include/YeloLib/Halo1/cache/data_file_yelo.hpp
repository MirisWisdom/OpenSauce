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
		bool FindSet(cstring name, 
			const char (*&bitmaps)[MAX_PATH],
			const char (*&sounds)[MAX_PATH],
			const char (*&locale)[MAX_PATH]
			);
		// Determines if the mod-set files exists.
		// If one file doesn't exist, this returns false, and the path details in [details_buffer]
		// The data held in [details_buffer] when this returns true is undefined, be careful
		bool VerifySetFilesExist(cstring bitmaps, cstring sounds, cstring locale, char details_buffer[MAX_PATH]);

#if PLATFORM_IS_EDITOR
		void SaveSettings();

		// EEXIST - file set exists, thus wasn't re-added
		// ENOSPC - no more file set slots available
		errno_t AddFileSet(cstring name);
#endif

		void SharedInitialize();
		void SharedDispose();
	};

#if PLATFORM_IS_EDITOR
	namespace Cache
	{
		class c_data_files
		{
			static const size_t k_name_length = 63;

			static cstring DataFileTypeToString(_enum df_type);
		public:
			char m_names[Enums::k_number_of_data_file_types][k_name_length+1];	// Names are all relative to the "maps\" directory (wherever it may be)

		public:
			// Initialize the data file system to either use a mod-set or the stock 
			// data files.
			// NOTE: [maps_path] will be modified if [using_mod_sets] is true!
			void InitializeForCache(bool using_mod_sets, cstring mod_name, char maps_path[MAX_PATH]);

		private:
			void CopyStockDataFile(cstring maps_path, _enum df_type);
		public:
			// Copy the stock data files that come with the game. These copies will then be used as 
			// the base input data files for the cache being built
			void CopyStock();
		};
	};
#endif
};