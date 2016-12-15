/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "CheApeInterface.hpp"

#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Common/DebugDump.hpp"
#include "Engine/EngineFunctions.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "TagGroups/TagGroups.hpp"
#include "TagGroups/yelo_definitions.hpp"

#include "Engine/AnimationUpgrade.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "Yelo/MemoryFixups.hpp"

namespace Yelo
{
	// Maybe we should just rename the individual namespaces 
	// to just 'HEK' ?
	namespace PLATFORM_VALUE(Guerilla, Tool, Sapien)
	{
		// Called before tag_files_open
		void IntializeBeforeTagGroupsInitalize();
		// Called after tag_files_open and yelo defs are initialized
		void Initialize();
		// Called before cseries_dispose's logic ran. This includes the debug manager's heap_dump.txt output (which logs assumed leaks)
		// NOTE: this code will never be reached in Guerilla or when Sapien is executed with "SAPIEN_NO_RENDER"
		void DisposeBeforeCSeries();
		void Dispose();
	};


	static void InitializeAfterCSeries()
	{
		Scripting::Initialize();

#ifndef PLATFORM_NO_DX9
		Rasterizer::Initialize();
#endif
	}
	static void DisposeBeforeCSeries()
	{
#if PLATFORM_TYPE == PLATFORM_TOOL || PLATFORM_TYPE == PLATFORM_SAPIEN
		PLATFORM_VALUE(__noop, Tool, Sapien)::DisposeBeforeCSeries();
#endif

		// this must come last. this is the code we overwrote to create the hook to this function
		blam::debug_dump_memory();
	}

	static void IntializeBeforeTagGroupsInitalize()
	{
		Objects::Items::GrenadeTypesUpgradeInitialize();
		Animations::AnimationUpgradesInitialize();
		PLATFORM_VALUE(Guerilla, Tool, Sapien)::IntializeBeforeTagGroupsInitalize();
	}
	static void InitializeForTagFilesOpen()
	{
		// tag_files_open is called after the engine shell initializes its cseries and errors code, so we can just do the call here
		InitializeAfterCSeries();
		IntializeBeforeTagGroupsInitalize();

		// Call the *engine's* initializer
		TagGroups::Initialize();

		TagGroups::YeloDefinitionsInitialize();
		if (TagGroups::YeloDefinitionsInitialized())
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Initialize();

#if PLATFORM_TYPE != PLATFORM_TOOL
		// cmd line args read here as they ned to happen late in Yelo's init
		Settings::ReadCmdLineSettings();
#endif
	}
	static void OverrideTagFilesOpen()
	{
		static uintptr_t TAG_FILES_OPEN_CALLS[] = {
#if PLATFORM_TYPE == PLATFORM_TOOL
			0x42DC10
#elif PLATFORM_TYPE == PLATFORM_GUERILLA
			0x421C30,
			0x41D4F5
#elif PLATFORM_TYPE == PLATFORM_SAPIEN
			0x418A50,
			0x4141E5	// for no render cases
#endif
		};

		for(auto ptr : TAG_FILES_OPEN_CALLS)
			Yelo::Memory::WriteRelativeCall(InitializeForTagFilesOpen, CAST_PTR(void*,ptr));
	}

	void Initialize()
	{
		FUNC_PTR(CSERIES_DISPOSE,							0x42C6A0, 0x435E20, 0x4F6E60);

		// Due to how early we hook the tools, ANYTHING you call in here can't use the engine's
		// error system. It must use the interfaces in cseries/errors_yelo.hpp instead
		Debug::DumpInitialize();
		c_memory_fixups::Initialize();

		OverrideTagFilesOpen();

#if PLATFORM_TYPE == PLATFORM_TOOL || PLATFORM_TYPE == PLATFORM_SAPIEN
		Memory::WriteRelativeJmp(DisposeBeforeCSeries, GET_FUNC_VPTR(CSERIES_DISPOSE));
#endif
	}

	void Dispose()
	{
		// Initializer called in InitializeForTagFilesOpen
		if (TagGroups::YeloDefinitionsInitialized())
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Dispose();

		//////////////////////////////////////////////////////////////////////////
		// see InitializeAfterCSeries
#ifndef PLATFORM_NO_DX9
		Rasterizer::Dispose();
#endif
		TagGroups::YeloDefinitionsDispose();
		Scripting::Dispose();
		//////////////////////////////////////////////////////////////////////////
		// see IntializeBeforeTagGroupsInitalize
		Objects::Items::GrenadeTypesUpgradeDispose();
		Animations::AnimationUpgradesDispose();

		c_memory_fixups::Dispose();
		Debug::DumpDispose();
	}
};