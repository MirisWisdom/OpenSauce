/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Cache
	{
		extern cstring K_MAP_FILE_EXTENSION_YELO;

		class c_map_file_finder
		{
			struct {
				cstring environment; // The 'normal' path, usually a folder relative to the EXE or CWD
				cstring user_profile;// The path defined by or in the user's profile

				cstring final; // The path which the finder first found all the data files in, or NULL if it couldn't
			}m_maps_path;

			cstring m_map_name;
			cstring m_map_extension;
			bool m_search_for_yelo_first;

			bool SearchPath(cstring maps_path);
			bool SearchEnvironment();
			bool SearchUserProfile();
		public:
			c_map_file_finder(cstring map_name, bool search_for_yelo_first);

			bool TryAndFind(std::string& map_path);
		};
	};
};