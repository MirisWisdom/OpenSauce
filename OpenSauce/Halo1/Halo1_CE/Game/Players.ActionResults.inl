/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

enum {
	k_number_of_player_examine_nearby_objects_jmp_entries = Yelo::Enums::_object_type_control, // types above control don't have a jmp entry
};

namespace ActionResults
{
	static void* player_examine_nearby_objects_server_jmp_table[k_number_of_player_examine_nearby_objects_jmp_entries];
	static void* player_examine_nearby_objects_client_jmp_table[k_number_of_player_examine_nearby_objects_jmp_entries];

	static void InitializePENOJmpTable(void*** jmp_table_src_ref, void* jmp_table_dst[])
	{
		for(int x = 0; x < k_number_of_player_examine_nearby_objects_jmp_entries; x++)
		{
			void** table = *jmp_table_src_ref;

			jmp_table_dst[x] = table[x];
		}

		*jmp_table_src_ref = jmp_table_dst;
	}

	void Initialize()
	{
	}
};