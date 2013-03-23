/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_data_array;
	};

	namespace MessageDeltas
	{
		struct s_index_resolution_table;
	};
};

namespace Yelo
{
	namespace Engine
	{
		void GatherException(const void* data = NULL, int32 arg_0 = 0x89, int32 arg_4 = 0x7E, int32 arg_8 = 1);


		// NOTE: we should just use GS's md5.c functions, but the GS SDK is no longer 'open' heh
		void GSMD5Digest(byte* input, unsigned int input_length, char output[33]);
		bool CompareMD5(byte* input, unsigned int input_length, const char* comparison_md5);

		bool GetCmdLineParameter(cstring parameter, cstring* value_out = NULL);

		namespace Cache
		{
			cstring GetMapExtension();

			void MapListAddMap(cstring map_name, cstring extension = ".map", int32 map_index = 0x13);

			int GetMapEntryIndexFromName(cstring name);
		};

		namespace Console
		{
			// Process a fully qualified string of script functions
			void ProcessCommand(cstring command);
			void ProcessRemoteCommand(int32 machine_index, cstring command);

			// Prints [msg] to the console
			void TerminalPrint(cstring msg);
			// Formats and prints to the console
			void TerminalPrintF(cstring format, ...);

			void Print(cstring msg);

			// Prints [msg] to the console with red coloring
			void Warning(cstring msg);
		};

		namespace Game
		{
			// Plays the [bink] file
			void PlayVideo(cstring bink);
		};

		namespace Memory
		{
			Yelo::Memory::s_data_array* DataNewAndMakeValid(cstring name, int32 maximum_count, size_t datum_size);
		};

		namespace Networking
		{
#ifndef YELO_NO_NETWORK
			// TODO:
			// TranslatedIndexClientRegister
			// TranslatedIndexUnregister

			// Translate a local object_index to a translated_index (ie, network)
			int32 TranslateObject(datum_index local_object_index);
			// Translate a local player_index to a translated_index (ie, network)
			int32 TranslatePlayer(datum_index local_player_index);

			int32 TranslatedIndexRegister(MessageDeltas::s_index_resolution_table& table, datum_index local_index);
#endif
		};
	};
};