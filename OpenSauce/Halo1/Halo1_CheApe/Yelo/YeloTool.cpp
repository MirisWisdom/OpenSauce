/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <blamlib/Halo1/tool/tool.hpp>

#include <YeloLib/Halo1/cache/data_file_yelo.hpp>

#include "Settings/Settings.hpp"
#include "Engine/EngineFunctions.hpp"
#include "Engine/GameState.hpp"
#include "Tool/Tool_AnimationFixups.hpp"

#include "Tool/BuildCacheFile.hpp"
#include "Tool/ImportLightmapUV.hpp"
#include "Tool/ShaderPostprocess.hpp"
#include "Tool/BuildCppDefinition.hpp"
#include "Tool/RuntimeCacheView.hpp"
#include "Tool/TagMaintenance.hpp"
#include "Tool/PackedFile/BuildPackedFiles.hpp"
#include "Tool/ToolMisc.hpp"
#include "Tool/RemoveOSTagData.hpp"

namespace Yelo
{
	namespace Tool
	{
		int __cdecl s_import_class::CompareProc(void*, const s_import_class* lhs, const s_import_class* rhs)
		{
			return strcmp(lhs->name, rhs->name);
		}
		int __cdecl s_import_class::SearchByNameProc(void*, cstring key, const s_import_class* element)
		{
			return strcmp(key, element->name);
		}

		enum {
			k_number_of_tool_import_classes = 0x15,
		};

		#include "Tool/Help.inl"


//////////////////////////////////////////////////////////////////////////
// Import classes

		static const s_import_class yelo_import_classes[] = {
			#include "Tool/ImportClassDefinitions.inl"
		};
		// The new import class list which is made up of tool's and [yelo_import_classes]
		static s_import_class g_import_classes[k_number_of_tool_import_classes + NUMBEROF(yelo_import_classes)];

		void ImportClassesInitialize()
		{
			// Tool's original import classes
			static const auto* tool_import_classes = CAST_PTR(s_import_class*, 0x6B01B0);

			// copy official import classes
			ArrayCopyPtr(	g_import_classes, 
							tool_import_classes, 
							k_number_of_tool_import_classes);
			// copy yelo import classes
			ArrayCopy(		g_import_classes, k_number_of_tool_import_classes, 
							yelo_import_classes, 0);
			// sort all the import classes by name from a to z
			Qsort(g_import_classes, s_import_class::CompareProc);

			// Modify build-cache-file to use our own implementation
			{
				auto* bcf_definition = Bsearch("build-cache-file",
					g_import_classes, s_import_class::SearchByNameProc);

				if(bcf_definition != nullptr)
					bcf_definition->import_proc = build_cache_file_for_scenario_stock_override;
				else
					YELO_WARN("CheApe: failed to override build-cache-file :s");
			}
		}

		class c_tool_main
		{
			static const s_import_class* FindImportClass(cstring verb)
			{
				char lowercase_verb[64]; // engine uses a 512 buffer...but why would a name be longer than 64 characters?
				strcpy_s(lowercase_verb, verb);
				_strlwr_s(lowercase_verb);

				return Bsearch(lowercase_verb, g_import_classes, s_import_class::SearchByNameProc);
			}

			static void PrintImportClasses()
			{
				for (const auto& clazz : g_import_classes)
					printf_s("  %s\n", clazz.usage);
			}
			static void PrintUsage(cstring exe_name, const s_import_class* clazz = nullptr)
			{
				cstring useage = clazz != nullptr
					? clazz->usage
					: "<options..>";

				printf_s("usage: %s %s\n", exe_name, useage);

				if (clazz == nullptr)
					PrintImportClasses();
			}
		public:
			static const uintptr_t HOOK_TRAMPOLINE_START =	0x415366;
			static const uintptr_t HOOK_TRAMPOLINE_END =	0x415432;

			static void MainHook(int argc, char* argv[], char* /*envp*/[])
			{
				char* exe_path = argv[0];
				// try and get the exe name without the parent directory
				if (char* proper_name = strrchr(exe_path, '\\'))
					exe_path = proper_name+1;

				char exe_name[64];
				if (strcpy_s(exe_name, exe_path) != k_errnone)
					YELO_ERROR_FAILURE("Who the hell names their tool exe longer than %d characters? %s",
						NUMBEROF(exe_name), exe_path);

				// null terminate at the start of the file extension
				if (char* extension = strrchr(exe_name, '.'))
					*extension = '\0';

				const s_import_class* clazz = nullptr;

				bool success = false;
				do {
					if (argc < 2)
						break;

					// find the first argument not prefixed with a dash. should be an import verb
					int arg_start = 1;
					for (; arg_start < argc; arg_start++)
					{
						if (*argv[arg_start] != '-')
							break;
					}

					// reach the end of the arg list without finding a non-dash arg
					if (arg_start == argc)
						break;

					cstring verb = argv[arg_start++];
					clazz = FindImportClass(verb);

					int verb_argc = argc - arg_start;
					// the verb given wasn't found or the number of arguments left are not exact, break
					if (clazz == nullptr || verb_argc != clazz->argument_count)
						break;

					clazz->import_proc(&argv[arg_start]);

					success = true;
				} while (false);

				if (!success)
					PrintUsage(exe_name, clazz);
			}
		};
		NAKED_FUNC_WRITER_ASM_BEGIN(main__hook_trampoline)
			push	[ebp+0x10] // envp
			push	[ebp+0x0C] // argv
			push	[ebp+0x08] // argc
			mov		eax, c_tool_main::MainHook // can't call directly, as it doesn't assemble into an absolute call
			call	eax
			jmp		c_tool_main::HOOK_TRAMPOLINE_END
			NAKED_FUNC_WRITER_ASM_END();

//////////////////////////////////////////////////////////////////////////

		void IntializeBeforeTagGroupsInitalize()
		{
		}

		void Initialize()
		{
			c_animation_fixups::Initialize();

			ImportClassesInitialize();

			size_t tramp_byte_count = Memory::c_naked_func_writer<main__hook_trampoline>::
				Write(CAST_PTR(void*, c_tool_main::HOOK_TRAMPOLINE_START));
			assert(tramp_byte_count <= c_tool_main::HOOK_TRAMPOLINE_END-c_tool_main::HOOK_TRAMPOLINE_START);

			GameState::Initialize();
		}

		void DisposeBeforeCSeries()
		{
			// NOTE: call any dispose routines which allocate using the engine's debug memory manager here
			
			c_animation_fixups::Dispose();
		}
		void Dispose()
		{
			GameState::Dispose();
		}
	};
};

#endif