/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\OpenSauce for specific license information
*/

#include <YeloLib/Halo1/cache/data_file_yelo.hpp>

namespace Yelo { namespace DataFiles {

	struct s_mod_set_info
	{
		const std::string mod_name;

		std::string bitmaps_path;
		std::string sounds_path;
		std::string loc_path;

		s_mod_set_info(cstring name) 
			: mod_name(name)
			, bitmaps_path()
			, sounds_path()
			, loc_path()
		{
		}

		bool Open()
		{
			return Cache::DataFilesOpen(bitmaps_path.c_str(), sounds_path.c_str(), loc_path.c_str());
		}

		struct hasher : std::unary_function<s_mod_set_info, size_t>
		{
			size_t operator()(const s_mod_set_info& info) const
			{
				std::hash<std::string> hasher;

				return hasher(info.mod_name);
			}
		};
		struct equal_to : std::binary_function<s_mod_set_info, s_mod_set_info, bool>
		{
			bool operator()(const s_mod_set_info& x, const s_mod_set_info& y) const
			{
				return x.mod_name == y.mod_name;
			}
		};
	};
	class c_mod_data_files_globals
	{
		std::string current_mod_set;
		std::unordered_map<std::string, s_mod_set_info> mod_sets;

	public:
		c_mod_data_files_globals()
			: current_mod_set("?") // set the current mod set to a value which can never appear in a mod name
			, mod_sets()
		{
		}

		bool TryAndUseModSet(cstring mod_name)
		{
			s_mod_set_info info(mod_name);
			if (info.mod_name == current_mod_set)
				return true;

			auto iter = mod_sets.find(info.mod_name);
			if (iter == mod_sets.end())
			{
				Cache::c_data_files_finder file_finder(mod_name);

				if (!file_finder.TryAndFind(info.bitmaps_path, info.sounds_path, info.loc_path))
					return false;

				iter = mod_sets.insert(std::make_pair(info.mod_name, info)).first;
			}

			current_mod_set = info.mod_name;

			bool success = blam::data_files_close() && iter->second.Open();
			return success;
		}

		void UseStockDataFiles()
		{
			if (!TryAndUseModSet(""))
				PrepareToDropError("Failed to find or load the stock bitmaps, sounds, and or loc data files");
		}
	}mod_data_files_globals;


	static void DataFilesOpenStock()
	{
		mod_data_files_globals.UseStockDataFiles();
	}

	API_FUNC_NAKED static void PLATFORM_API CacheFilesInitialize_DataFilesOpenHook()
	{
		static uintptr_t JMP_ADDRESS = GET_FUNC_PTR(CACHE_FILES_INITIALIZE_RETN);

		using namespace Cache; // can't use '::' in the asm code for struct offsets

		__asm {
			call	DataFilesOpenStock

			// we hook the cf-initialize code before it performs this
			call	Cache::CacheFileGlobals
			mov		[eax]s_cache_file_globals.open_map_file_index, NONE

			xor		ebx, ebx // rest of the function expects ebx to be zero, but we hook it before the xor instruction
			jmp		JMP_ADDRESS
		}
	}
	API_FUNC_NAKED static void PLATFORM_API CacheFilesDispose_DataFilesCloseHook()
	{
		static uintptr_t JMP_ADDRESS = GET_FUNC_PTR(CACHE_FILES_DISPOSE_RETN);

		__asm {
			call	blam::data_files_close
			jmp		JMP_ADDRESS
		}
	}

	API_FUNC_NAKED static void PLATFORM_API DataFileReadHook(void* buffer)
	{
		API_FUNC_NAKED_START()
			// esi data_file
			// eax position
			// edi buffer_size
			push	edi
			push	buffer
			push	eax
			mov		ecx, esi
			call	Cache::s_data_file::Read
		API_FUNC_NAKED_END(0)
	}


}; };