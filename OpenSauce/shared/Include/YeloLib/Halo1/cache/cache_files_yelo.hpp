/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum data_file_reference_type : _enum;
	};

	namespace Cache
	{
		struct s_cache_header;
		struct s_cache_tag_header;
		struct s_cache_tag_instance;

		// The type of cache file which a map_path refers to
		enum class e_map_path_file_type : long_enum
		{
			invalid = NONE,

			map = 0,
			yelo,

			k_number_of
		};

		enum class e_cache_read_header_result : long_enum
		{
			success,

			file_not_found,
			open_failed,
			read_failed,
			header_invalid,
			yelo_header_invalid,

			k_number_of
		};

		extern cstring K_MAP_FILE_EXTENSION_YELO; // ".yelo"

		class c_map_file_finder
		{
			struct {
				cstring environment;	// The 'normal' path, usually a folder relative to the EXE or CWD
				cstring user_profile;	// The path defined by or in the user's profile
				cstring game_exe;		// The "EXE Path" value from the registry. DEDI ONLY!

				cstring final; // The path which the finder first found all the data files in, or NULL if it couldn't
			}m_maps_path;

			cstring m_map_name;
			cstring m_map_extension;

			bool SearchPath(cstring maps_path);
			bool SearchEnvironment();
			bool SearchUserProfile();
			bool SearchExePath();
		public:
			c_map_file_finder(cstring map_name);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Attempts to and find the map_name. </summary>
			///
			/// <param name="map_path">			[out] Full pathname of the map file, or empty if it wasn't found. </param>
			/// <param name="return_access">	[out] (Optional) If non-null, the result a call to _access() would return. </param>
			///
			/// <returns>	true if the map_name was resolved, false if it wasn't. </returns>
			bool TryAndFind(_Out_ std::string& map_path, _Out_opt_ errno_t* return_access = nullptr);

			// Should the finder search for .yelo files before .map?
			// This should be set when the user settings are loaded, else it is always false
			static bool g_search_for_yelo_first;
		};

		// utility for blam::cache_file_data_load
		struct s_cache_file_data_load_state
		{
			byte* base_address;
			size_t memory_available;

			s_cache_file_data_load_state(s_cache_header* cache_header, s_cache_tag_header* tag_header);

		private:
			byte* Alloc(s_cache_tag_instance* tag_instance, size_t bytes);
		public:
			byte* ReadExternalData(s_cache_tag_instance* tag_instance, Enums::data_file_reference_type data_file);
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of a map from a file path. </summary>
		///
		/// <param name="map_name">	[out] The (lowercase) name of the map, sans directory and extension </param>
		/// <param name="map_path"> Full pathname of the map file. </param>
		///
		/// <returns>	The type of cache file the path refers to, or invalid if it isn't a valid map path. </returns>
		e_map_path_file_type GetMapNameFromPath(_Out_ char (&map_name)[_MAX_FNAME], cstring map_path);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Reads a header. </summary>
		///
		/// <param name="map_path">	Full pathname of the map file. </param>
		/// <param name="header">  	[out] The file's header. </param>
		///
		/// <returns>	success if it reads the header, and the header is valid; else non-success. </returns>
		e_cache_read_header_result ReadHeader(cstring map_path, _Out_ s_cache_header& header);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for the first map file with the given name and tries to read its header. </summary>
		///
		/// <remarks>Searches the CWD and user profile (order is defined by user settings) maps folder for a map file (.map,
		/// 	etc) with the given name.
		/// </remarks>
		///
		/// <param name="map_name">	Name of the map. </param>
		/// <param name="header">  	[out] The file's header. </param>
		///
		/// <returns>	true if it finds the map, reads the header, and the header is valid; else non-success. </returns>
		e_cache_read_header_result FindMapFileAndReadHeader(cstring map_name, _Out_ s_cache_header& header);

		uint32 CalculateChecksum(cstring map_path);
		uint32 FindMapFileAndCalculateChecksum(cstring map_name);
	};
};