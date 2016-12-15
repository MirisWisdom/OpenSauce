/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/Statistics.hpp"

#include "Memory/Runtime.hpp"
#include "Cache/Cache.hpp"

// alpha	retail
// 0x1C7F80	0x196470
// 0x1C8180	0x196780

// 0x1C81C0	0x1967D0
// 0x1C8360	0x1968B0
// 0x1C8520	0x196960

// 0x1E9CE0, 0x1E9DF0 stat proxy

// 0x241620 - ctf\bomb returns
// 0x2BCC10 - oddball, records kill
// 0x2BDA60 - king, records hill shit
// 0x1922A0 - juggernaut, kills
// 0x192420 - juggernaut, control time
// 0x2C0740 - territories, records lost\taken

// 0x15BA90, 0x15BB20 - seconds alive

// 0x1B7FE1	0x232D77

static Yelo::Memory::Opcode::s_call_ret UNWIND_STATISTICS_UPDATE;

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_STATISTICS
#include "Memory/_EngineLayout.inl"

	namespace Enums
	{
		enum {
			k_statistics_pcr_buffer_size = 0x2000,
		};
	};

	namespace Statistics
	{
		void Initialize()
		{
			YELO_MEM_WRITE(OverwriteJmp, 
				&UNWIND_STATISTICS_UPDATE, 
				GET_FUNC_VPTR(GAME_RESULTS_FINALIZE_HOOK), 
				&Yelo::Statistics::OnBuildPcr);
		}

		void Dispose()
		{
			YELO_MEM_WRITE(OverwriteJmp, 
				&UNWIND_STATISTICS_UPDATE, 
				GET_FUNC_VPTR(GAME_RESULTS_FINALIZE_HOOK), 
				CAST_PTR(void*, UNWIND_STATISTICS_UPDATE.Address));
		}

		static void BuildPcrBuffer(byte buffer[Enums::k_statistics_pcr_buffer_size])
		{
			enum
			{
				_offset_Timestamp = 0,
				_offset_Mapname = _offset_Timestamp + sizeof(FILETIME),
				_offset_GameType = _offset_Mapname + sizeof(tag_string),
				_offset_Players = _offset_GameType + sizeof(uint32),
			};

			// copies filetime
			ApiGetSystemTimeAsFileTime(
				(LPFILETIME)&buffer[_offset_Timestamp]
			);

			// copies map name
			XboxLib::_memcpy(
				&buffer[_offset_Mapname],
				GameState::_CacheHeader()->Name,
				sizeof(tag_string)
				);

			// copies gametype enum
			*CAST_PTR(uint32*, &buffer[_offset_GameType]) = *CAST_PTR(uint32*,
				GET_DATA_VPTR(GAME_RESULTS_GAME_TYPE)
				);

			// copies player and team stats
			XboxLib::_memcpy(
				&buffer[_offset_Players],
				GameState::_PcrPlayerStats(),
				sizeof(pcr_stat_player)*16 // 0x1140
				);
		}

		void PersistPcr(char drive, const char file[24])
		{
			// allocate temp buffer for building stats
 			byte* address = CAST_PTR(byte*,
 				KrnlMmAllocateContiguousMemory(Enums::k_statistics_pcr_buffer_size));

			BuildPcrBuffer(address);

			// Drive: 3 chars
			// Extension: 4 chars
			// Terminator: 1 char
			// File path: 24 chars
			// Total: 32 chars
			// build the file path
			char buffer[32];
			memset(buffer, 0, sizeof(buffer));

			// append the drive path
			buffer[0] = drive;
			buffer[1] = ':';
			buffer[2] = '\\';

			tag* ext = (tag*)XboxLib::_memcpy(
				&buffer[3], file, strlen(file)); // copy file path to our buffer and get the new string cursor
			
			*ext = 'tad.'; // append our file extension, .dat
			
			buffer[sizeof(buffer)-1] = '\0'; // make sure we have no overruns

			// open file
			HANDLE file_handle = 
				ApiCreateFile(buffer, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			HANDLE result = file_handle;
			if(result != (HANDLE)NULL_HANDLE)
			{
				// seek to the end of the file
				result = (HANDLE)ApiSetFilePointer(file_handle, NULL, NULL, FILE_END);
				if(result == (HANDLE)NULL_HANDLE) goto DISPLAY_LAST_ERROR;

				// write file
				DWORD num_written;
				result = (HANDLE)ApiWriteFile(file_handle, address, Enums::k_statistics_pcr_buffer_size, &num_written, NULL);
				if(result == (HANDLE)NULL) goto DISPLAY_LAST_ERROR;

				// close file
				result = (HANDLE)ApiCloseHandle(file_handle);
				if(result == (HANDLE)NULL) goto DISPLAY_LAST_ERROR;
			}
			else
			{
DISPLAY_LAST_ERROR:
				XboxLib::DisplayLastError();
			}

			// frees temp buffer
			KrnlMmFreeContiguousMemory(address);
		}



		void OnBuildPcr()
		{
			#pragma region Goto the games function
			static proc_update_no_args build_postgame_carnage_report = CAST_PTR(proc_update_no_args, GET_FUNC_PTR(BUILD_POST_GAME_CARNAGE_REPORT));
			build_postgame_carnage_report();
			#pragma endregion
		}
	};

	namespace GameState
	{
		Statistics::pcr_stat_player* _PcrPlayerStats()	DPTR_IMP_GET(_PcrPlayerStats);
	};
};