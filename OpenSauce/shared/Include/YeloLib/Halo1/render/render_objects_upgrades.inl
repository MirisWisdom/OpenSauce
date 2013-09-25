/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace render_objects_mods
{
	static void Initialize()
	{
#if PLATFORM_IS_USER || PLATFORM_ID == PLATFORM_SAPIEN
		static datum_index rendered_objects[Enums::k_maximum_rendered_objects_upgrade];

		for(auto ptr : K_MAXIMUM_RENDERED_OBJECTS_REFS_32bit)
			*CAST_PTR(int32*,ptr) = Enums::k_maximum_rendered_objects_upgrade;
		for(auto ptr : K_MAXIMUM_RENDERED_OBJECTS_REFS_16bit)
			*CAST_PTR(int16*,ptr) = Enums::k_maximum_rendered_objects_upgrade;

		for(auto ptr : K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REFS)
			*CAST_PTR(datum_index**,ptr) = rendered_objects;
#endif
	}
};