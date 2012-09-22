/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum game_build_number_index
		{
			_game_build_number_index_100,
			_game_build_number_index_107,
			_game_build_number_index_108,
			_game_build_number_index_109,

			k_max_game_build_number_index,
		};
	};

	namespace BuildNumber
	{
		char*		GameBuildString();
		char*		GamespyGameBuildString();


		void Initialize();
		void Dispose();
		void InitializeForNewMap();


		// Returns true if build_number matches one of the numbers in g_game_build_numbers
		bool StringIsValid(cstring build_number);

		// Parses a major.minor build version string, eg "1.07", to the full build number
		// Returns NULL if given an unidentified version
		cstring VersionToBuildNumberString(cstring maj_minor_str);

		// If [and_game_build] is true, it will also change the GameState::GameBuildStrings
		bool ChangeAdvertisedVersion(cstring version_str, bool and_game_build);
	};
};