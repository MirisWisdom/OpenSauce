/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Scenario/Scenario.hpp"

#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_structures.hpp>
// There are places which use a bit-vector for the structure-bsps
// In the stock code, the bit-vector is a 16-bit word
// We can only, reasonably, upgrade the code to a 32-bit word
BOOST_STATIC_ASSERT( Yelo::Enums::k_maximum_structure_bsps_per_scenario_upgrade <= 32 );

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Scenario
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_SCENARIO
#define __EL_INCLUDE_FILE_ID	__EL_SCENARIO_SCENARIO
#include "Memory/_EngineLayout.inl"

		s_scenario_globals* ScenarioGlobals()						DPTR_IMP_GET(scenario_globals);

		TagGroups::scenario* Scenario()								PTR_IMP_GET(global_scenario);
		TagGroups::collision_bsp* Bsp3d()							PTR_IMP_GET(global_bsp3d);
		TagGroups::collision_bsp* CollisionBsp()					PTR_IMP_GET(global_collision_bsp);
		TagGroups::structure_bsp* StructureBsp()					PTR_IMP_GET(global_structure_bsp);

		datum_index ScenarioIndex()									PTR_IMP_GET(global_scenario_index);
		int16 StructureBspIndex()									PTR_IMP_GET(structure_bsp_index);

		datum_index* GlobalScenarioIndexReference()					PTR_IMP_GET2(global_scenario_index);
		TagGroups::scenario** GlobalScenarioReference()				PTR_IMP_GET2(global_scenario);
		int16* GlobalStructureBspIndexReference()					PTR_IMP_GET2(structure_bsp_index);
		TagGroups::structure_bsp** GlobalStructureBspReference()	PTR_IMP_GET2(global_structure_bsp);
		TagGroups::collision_bsp** GlobalBspReference()				PTR_IMP_GET2(global_bsp3d);
		TagGroups::collision_bsp** GlobalCollisionBspReference()	PTR_IMP_GET2(global_collision_bsp);

		static void InitializeMaximumStructureBspsPerScenarioUpgrade()
		{
			void* asm_bytes;

			// change 'movzx' to 'mov'
			asm_bytes = GET_FUNC_VPTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ);
			static const byte k_mod1_asm_bytes[] = {
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_mov_eax_y_offset
			};
			Memory::OverwriteMemorySansCopyArray(asm_bytes, k_mod1_asm_bytes);

			// change 'or offset, dx' to 'or offset, edx'
			asm_bytes = GET_FUNC_VPTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE);
			static const byte k_mod2_asm_bytes[] = {
				Enums::_x86_opcode_nop,
			};
			Memory::OverwriteMemorySansCopyArray(asm_bytes, k_mod2_asm_bytes);

			// change 'mov offset, bx' to 'mov offset, ebx'
			asm_bytes = GET_FUNC_VPTR(OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS);
			static const byte k_mod3_asm_bytes[] = {
				Enums::_x86_opcode_nop,
			};
			Memory::OverwriteMemorySansCopyArray(asm_bytes, k_mod3_asm_bytes);
		}
		void Initialize()
		{
			InitializeMaximumStructureBspsPerScenarioUpgrade();
		}

		void Dispose()
		{
		}
	};
};