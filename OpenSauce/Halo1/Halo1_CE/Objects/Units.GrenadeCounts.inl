/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/units/unit_structures.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Networking/MDP.hpp"
#include "Networking/MessageDeltas.hpp"

namespace Yelo { namespace Objects { namespace Units {

// DEBUG: Before each memory write when we're enabling, we assert we're overwriting the expected value or bytes
#include <YeloLib/Halo1/units/units_grenade_count_upgrade.inl>

static void InitializeGrenadeCounts_UnitGrenadeCounts(bool enabled)
{
	//////////////////////////////////////////////////////////////////////////
	// actor_died
	{
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::actor_died__unit_grenade_count_word,
														unit_grenade_counts_mods::actor_died__unit_grenade_count_dword>
			code_writer;

		void* code_addr = GET_FUNC_VPTR(ACTOR_DIED_UNIT_GRENADE_COUNT_MOD);

		if(enabled)	code_writer::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer::Undo(code_addr);
	}
	//////////////////////////////////////////////////////////////////////////
	// player_add_equipment
	{
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::player_add_equipment__unit_grenade_count_word,
														unit_grenade_counts_mods::player_add_equipment__unit_grenade_count_dword>
			code_writer;

		void* code_addr = GET_FUNC_VPTR(PLAYER_ADD_EQUIPMENT_UNIT_GRENADE_COUNT_MOD);

		if(enabled)	code_writer::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer::Undo(code_addr);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_new_from_network
	{
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::biped_new_from_network__unit_grenade_count_word,
														unit_grenade_counts_mods::biped_new_from_network__unit_grenade_count_dword>
			code_writer;

		void* code_addr = GET_FUNC_VPTR(BIPED_NEW_FROM_NETWORK_UNIT_GRENADE_COUNT_MOD);

		if(enabled)	code_writer::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer::Undo(code_addr);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_update_baseline
	{
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::biped_update_baseline__unit_grenade_count_word1,
														unit_grenade_counts_mods::biped_update_baseline__unit_grenade_count_dword1>
			code_writer1;
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::biped_update_baseline__unit_grenade_count_word2,
														unit_grenade_counts_mods::biped_update_baseline__unit_grenade_count_dword2>
			code_writer2;

		void* code_addr = GET_FUNC_VPTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD1);

		if(enabled)	code_writer1::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer1::Undo(code_addr);

		code_addr = GET_FUNC_VPTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD2);

		if(enabled)	code_writer2::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer2::Undo(code_addr);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_build_update_delta
	{
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::biped_build_update_delta__unit_grenade_count_word1,
														unit_grenade_counts_mods::biped_build_update_delta__unit_grenade_count_dword1>
			code_writer1;
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::biped_build_update_delta__unit_grenade_count_word2,
														unit_grenade_counts_mods::biped_build_update_delta__unit_grenade_count_dword2>
			code_writer2;

		void* code_addr = GET_FUNC_VPTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD1);

		if(enabled)	code_writer1::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer1::Undo(code_addr);

		code_addr = GET_FUNC_VPTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD2);

		if(enabled)	code_writer2::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer2::Undo(code_addr);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_process_update_delta
	{
		typedef Memory::c_naked_func_writer_with_undo<	unit_grenade_counts_mods::biped_process_update_delta__unit_grenade_count_word,
														unit_grenade_counts_mods::biped_process_update_delta__unit_grenade_count_dword>
			code_writer;

		void* code_addr = GET_FUNC_VPTR(BIPED_PROCESS_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD);

		if(enabled)	code_writer::Write(code_addr DebugOnly(, "GrenadeCounts asm mismatch"));
		else		code_writer::Undo(code_addr);
	}

	// TODO: crosshairs_draw
}


static void InitializeGrenadeCounts_MessageDeltaGrenadeCounts(bool enabled)
{
	using namespace MessageDeltas;

	field_type_definition& grenade_counts_type = GlobalFieldTypeList()[Enums::_field_type_grenade_counts];
	field_properties_definition* grenade_counts_props = MDP_GET_FIELD_TYPE_DEFINITION(grenade_counts);

	field_type_definition& fixed_width_grenade_index_type = GlobalFieldTypeList()[Enums::_field_type_fixed_width_grenade_index];
	field_properties_definition* fixed_width_grenade_index_props = MDP_GET_FIELD_TYPE_DEFINITION(fixed_width_grenade_index);

	// local copies of the original game definitions
	static struct {
		mdp_field_type_maximum_size_calculator maximum_size_calculator;
		mdp_field_encode encoder;
		mdp_field_decode decoder;

		void Initialize(const field_type_definition& type, const field_properties_definition* props)
		{
			maximum_size_calculator = type.proc_maximum_size_calculator;
			encoder = props->proc_encode;
			decoder = props->proc_decode;
		}
		// Returns [props] so we can fluently modify its maximum size
		field_properties_definition* CopyTo(field_type_definition& type, field_properties_definition* props) const
		{
			type.proc_maximum_size_calculator = maximum_size_calculator;
			props->proc_encode = encoder;
			props->proc_decode = decoder;

			return props;
		}
	}	g_stock_grenade_counts, g_stock_fixed_width_grenade_index, 
		g_fixed_width_weapon_index;

	if(g_stock_grenade_counts.maximum_size_calculator == nullptr)
	{
		field_type_definition& fixed_width_weapon_index_type = GlobalFieldTypeList()[Enums::_field_type_fixed_width_weapon_index];
		field_properties_definition* fixed_width_weapon_index_props = MDP_GET_FIELD_TYPE_DEFINITION(fixed_width_weapon_index);

		g_stock_grenade_counts
			.Initialize(grenade_counts_type, grenade_counts_props);
		g_stock_fixed_width_grenade_index
			.Initialize(fixed_width_grenade_index_type, fixed_width_grenade_index_props);
		g_fixed_width_weapon_index
			.Initialize(fixed_width_weapon_index_type, fixed_width_weapon_index_props);
	}

	// We only define a custom grenade_counts impl as we re-use fixed_width_weapon_index for fixed_width_grenade_index
	class mdp_grenade_counts_upgrade
	{
	public:
		static int32 PLATFORM_API maximum_size_calculator(field_properties_definition*)
		{
			return Enums::k_bits_in_grenade_counts_type_upgrade;
		}

		static int32 PLATFORM_API encoder(field_properties_definition* field_properties, const void* baseline_data, const void* source_data, Memory::s_bitstream* output_stream)
		{
			int32 bits_written = 0;

			BOOST_STATIC_ASSERT( sizeof(grenade_counts) == sizeof(byte)*2 );

			const auto* baseline_data_gc = CAST_PTR(const grenade_counts*, baseline_data); // should always be NULL...
			const auto* source_data_gc = CAST_PTR(const grenade_counts*, source_data);

			bits_written =  g_stock_grenade_counts.encoder(field_properties, baseline_data_gc, source_data_gc,   output_stream);

			baseline_data_gc = baseline_data_gc != nullptr 
				? baseline_data_gc+1 
				: nullptr;
			source_data_gc += 1;

			bits_written += g_stock_grenade_counts.encoder(field_properties, baseline_data_gc, source_data_gc+1, output_stream);

			return bits_written;
		}

		static int32 PLATFORM_API decoder(field_properties_definition* field_properties, void* baseline_data, void* destination_data, Memory::s_bitstream* input_stream)
		{
			int32 bits_read = 0;

			auto* baseline_data_gc = CAST_PTR(grenade_counts*, baseline_data); // should always be NULL...
			auto* destination_data_gc = CAST_PTR(grenade_counts*, destination_data);

			// stock decoder doesn't use baseline_data when decoding
			bits_read =  g_stock_grenade_counts.decoder(field_properties, baseline_data_gc, destination_data_gc, input_stream);

			baseline_data_gc = baseline_data_gc != nullptr 
				? baseline_data_gc+1 
				: nullptr;
			destination_data_gc += 1;

			bits_read += g_stock_grenade_counts.decoder(field_properties, baseline_data_gc, destination_data_gc, input_stream);

			return bits_read;
		}
	};

	if(enabled)
	{
		grenade_counts_type.proc_maximum_size_calculator = mdp_grenade_counts_upgrade::maximum_size_calculator;
		grenade_counts_props->proc_encode = mdp_grenade_counts_upgrade::encoder;
		grenade_counts_props->proc_decode = mdp_grenade_counts_upgrade::decoder;
		grenade_counts_props->maximum_size = Enums::k_bits_in_grenade_counts_type_upgrade;

		// We re-use weapon_index's field type definitions as it's limited to 4 indexes as well
		g_fixed_width_weapon_index
			.CopyTo(fixed_width_grenade_index_type, fixed_width_grenade_index_props)
			->maximum_size = g_fixed_width_weapon_index.maximum_size_calculator(nullptr); // release builds don't use the prop parameter in this field type
	}
	else
	{
		g_stock_grenade_counts
			.CopyTo(grenade_counts_type, grenade_counts_props)
			->maximum_size = Enums::k_bits_in_grenade_counts_type;
		g_stock_fixed_width_grenade_index
			.CopyTo(fixed_width_grenade_index_type, fixed_width_grenade_index_props)
			->maximum_size = g_stock_fixed_width_grenade_index.maximum_size_calculator(nullptr); // release builds don't use the prop parameter in this field type
	}
}

}; }; };