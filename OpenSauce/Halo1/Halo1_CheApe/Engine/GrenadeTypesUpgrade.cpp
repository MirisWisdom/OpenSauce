/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"

#include <TagGroups/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/item_definitions.hpp>

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Objects { namespace Items {

		ENGINE_PTR(string_list, global_grenade_type_enum, 0x9D1728, 0x6DE850, 0xA496AC);
		ENGINE_PTR(tag_block_definition, grenades_block, 0x9D237C, 0x6DF4A4, 0xA4A2FC);

		static cstring global_grenade_type_yelo_enum_strings[Enums::_grenade_type_yelo] = {
			"",
			"",
			"custom 2",
			"custom 3",
		};
		static string_list global_grenade_type_yelo_enum = {
			NUMBEROF(global_grenade_type_yelo_enum_strings),
			global_grenade_type_yelo_enum_strings
		};

		void GrenadeTypesUpgradeInitialize()
		{
			using namespace Enums;

			string_list* types_enum = GET_PTR2(global_grenade_type_enum);
			tag_block_definition* block_definition = GET_PTR2(grenades_block);
			assert(types_enum->count == _grenade_type && block_definition->max_elements == _grenade_type);

			global_grenade_type_yelo_enum_strings[_grenade_type_frag] = types_enum->strings[_grenade_type_frag];
			global_grenade_type_yelo_enum_strings[_grenade_type_plasma] = types_enum->strings[_grenade_type_plasma];

#if PLATFORM_ID != PLATFORM_TOOL
			GrenadeTypesUpgrade(true);
#endif
		}
		void GrenadeTypesUpgradeDispose()
		{
		}


		static uintptr_t K_GLOBAL_GRENADE_TYPE_ENUM_STRINGS_REFS[] = {
			PLATFORM_VALUE(0x491376, 0x48D186, 0x5A7576),
		};
		static uintptr_t K_GLOBAL_GRENADE_TYPE_ENUM_REFS[] = {
			PLATFORM_VALUE(0x49134B, 0x48D15B, 0x5A754B), PLATFORM_VALUE(0x9C97E0, 0x6D7AA8, 0xA467A0),
			PLATFORM_VALUE(0x9D1774, 0x6DE89C, 0xA496F4), PLATFORM_VALUE(0x9D5E28, 0x6E2F50, 0xA4DDA8),
		};

		void GrenadeTypesUpgrade(bool enabled)
		{
			string_list* types_enum = enabled ? &global_grenade_type_yelo_enum : GET_PTR2(global_grenade_type_enum);

			for(int x = 0; x < NUMBEROF(K_GLOBAL_GRENADE_TYPE_ENUM_STRINGS_REFS); x++)
			{
				cstring*** strings_ref = CAST_PTR(cstring***, K_GLOBAL_GRENADE_TYPE_ENUM_STRINGS_REFS[x]);

				*strings_ref = &types_enum->strings;
			}

			for(int x = 0; x < NUMBEROF(K_GLOBAL_GRENADE_TYPE_ENUM_REFS); x++)
			{
				string_list** enum_ref = CAST_PTR(string_list**, K_GLOBAL_GRENADE_TYPE_ENUM_REFS[x]);

				*enum_ref = types_enum;
			}

			tag_block_definition* block_definition = GET_PTR2(grenades_block);
			block_definition->max_elements = types_enum->count;
		}

	}; };
};