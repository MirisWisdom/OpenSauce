/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "CheApeInterface.hpp"

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
		void Dispose();
	};


	static void IntializeBeforeTagGroupsInitalize()
	{
		PLATFORM_VALUE(Guerilla, Tool, Sapien)::IntializeBeforeTagGroupsInitalize();
	}
	static void InitializeForTagFilesOpen()
	{
		IntializeBeforeTagGroupsInitalize();

		// Call the *engine's* initializer
		TagGroups::Initialize();

		TagGroups::YeloDefinitionsInitialize();
		if(TagGroups::_yelo_definition_globals.initialized)
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Initialize();

		// TODO: we many want to move this into IntializeBeforeTagGroupsInitalize later
		// However, we won't be able to use some tag_group related functions in the fixing code
		TagGroups::InitializeFixes();
	}
	static void OverrideTagFilesOpen()
	{
		static uint32 TAG_FILES_OPEN_CALLS[] = {
#if PLATFORM_ID == PLATFORM_TOOL
			0x42DC10
#elif PLATFORM_ID == PLATFORM_GUERILLA
			0x421C30,
			0x41D4F5
#elif PLATFORM_ID == PLATFORM_SAPIEN
			0x418A50,
			0x4141E5	// for no render cases
#endif
		};

		for(int32 x = 0; x < NUMBEROF(TAG_FILES_OPEN_CALLS); x++)
			Yelo::Memory::WriteRelativeCall(InitializeForTagFilesOpen, CAST_PTR(void*,TAG_FILES_OPEN_CALLS[x]));
	}

	void Initialize()
	{
		Debug::DumpInitialize();
		c_memory_fixups::Initialize();
		Objects::Items::GrenadeTypesUpgradeInitialize();
		Animation::AnimationUpgradeInitialize();
		Scripting::Initialize();
#if PLATFORM_ID != PLATFORM_TOOL
		Scripting::InitializeCustomScriptingDefinitions();
#endif

		OverrideTagFilesOpen();

#ifndef PLATFORM_NO_DX9
		Rasterizer::Initialize();
#endif
	}

	void Dispose()
	{
#ifndef PLATFORM_NO_DX9
		Rasterizer::Dispose();
#endif

		// Initializer called in InitializeForTagFilesOpen
		if(TagGroups::_yelo_definition_globals.initialized)
			PLATFORM_VALUE(Guerilla, Tool, Sapien)::Dispose();

		TagGroups::YeloDefinitionsDispose();
		Scripting::Dispose();
		Objects::Items::GrenadeTypesUpgradeDispose();
		Animation::AnimationUpgradeDispose();
		c_memory_fixups::Dispose();
		Debug::DumpDispose();
	}
};