/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/models/collision_model_definitions.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Objects
	{

static void PLATFORM_API object_damage_object_body_hook(datum_index object_index, 
	int32 region_index, int32 node_index, void* /*unknown*/,
	const TagGroups::s_damage_resistance* damage_resistance, const TagGroups::collision_model_material* damage_material, 
	const TagGroups::s_damage_definition* damage_definition, Objects::s_damage_data* damage_data,
	void*, __out real&, __out real&, real, bool)
{
#if 0
	//////////////////////////////////////////////////////////////////////////
	// Does this map have any extensions defined at all?
	const TAG_TBLOCK(& damage_extensions, TagGroups::s_object_damage_extension) = TagGroups::_global_yelo_globals->object_damage_extensions;
	if(damage_extensions.Count == 0)
		return;

	s_object_header_datum* object_header_datums = *Objects::ObjectHeader();
	s_object_data* object_data = object_header_datums[object_index.index]._object;


	//////////////////////////////////////////////////////////////////////////
	// Lookup the current object's definition in the damge_extensions block
	const datum_index* object_definition_index = object_data->GetTagDefinition();

	const TagGroups::s_object_damage_extension* dmg_ext = nullptr;
	for(int32 x = 0; x < damage_extensions.Count; x++)
	{
		const TagGroups::s_object_damage_extension* def = &damage_extensions[x];

		if(def->object.tag_index == *object_definition_index)
			dmg_ext = def;
	}

	//////////////////////////////////////////////////////////////////////////
	// If we found a definition, then the map has extensions defined for this object type
	// TODO: ChokingVictim's code is...choking me up. What he provided isn't making sense, holding off on the integration for this for the time being.
	if(false && dmg_ext != nullptr)
	{
		real max_vitality = *object_data->GetMaximumVitality();
		int32 region_vitality = (*object_data->GetRegionVitality()) + region_index;
		real current_region_damage = CAST(real, region_vitality) / max_vitality;

	#if 0
		int32 region_count = dmg_ext->regions.Count;
		const TagGroups::s_object_damage_region_extension& region_ext = dmg_ext->regions[region_index];

		// If the objects region count is greater than [k_maximum_regions_per_model], accommodate for its place in the region vitality/permutation array
		// Else [k_maximum_regions_per_model] is the max array element in the region vitality/permutation array
		byte* region_perm = object_data->GetRegionVitality() + 
			(region_count >= Enums::k_maximum_regions_per_model ? region_count : Enums::k_maximum_regions_per_model) +
			region_index;

		// loop through the object permutations and test the region's damage
		for(int32 x = *region_perm + 1; x < region_ext.permutations.Count; x++)
		{
			real current_region_damage_threshold = region_ext.permutations[x].damage_threshold;

			// if the current damage taken by this region is greater than or equal to the damage threshold, switch perms
			if (current_region_damage_threshold > 0.0f && current_region_damage >= current_region_damage_threshold)
				*region_perm = CAST(byte, x);
		}
	#endif
	}
#endif
}

static void DamageInitialize()
{
	Memory::WriteRelativeJmp(&object_damage_object_body_hook, GET_FUNC_VPTR(OBJECT_DAMAGE_OBJECT_BODY_HOOK), true);
}

static void DamageDispose()
{
}

	};
};