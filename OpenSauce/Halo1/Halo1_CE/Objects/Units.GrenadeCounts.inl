/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

// DEBUG: Before each memory write when we're enabling, we assert we're overwriting the expected value or bytes

static void InitializeForYeloGameState_UnitZoomLevelRefs(bool enabled)
{
	uint32 offset = enabled ? s_unit_data::k_offset_ZoomLevel_Yelo : s_unit_data::k_offset_ZoomLevel;

	for(int x = 0; x < NUMBEROF(K_UNIT_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugOnly( if(enabled) ASSERT( s_unit_data::k_offset_ZoomLevel == *offset_ref, "GrenadeCounts asm mismtach!" ) );
		*offset_ref = offset;
	}
}
static void InitializeForYeloGameState_UnitDesiredZoomLevelRefs(bool enabled)
{
	uint32 offset = enabled ? s_unit_data::k_offset_DesiredZoomLevel_Yelo : s_unit_data::k_offset_DesiredZoomLevel;

	for(int x = 0; x < NUMBEROF(K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugOnly( if(enabled) ASSERT( s_unit_data::k_offset_DesiredZoomLevel == *offset_ref, "GrenadeCounts asm mismtach!" ) );
		*offset_ref = offset;
	}
}
static void InitializeForYeloGameState_NumberOfUnitGrenadeTypes(bool enabled)
{
	uint32 count = enabled ? Enums::k_unit_grenade_types_count_yelo : Enums::k_unit_grenade_types_count;

	for(int x = 0; x < NUMBEROF(K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS); x++)
	{
		uint32* count_ref = CAST_PTR(uint32*, K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[x]);

		DebugOnly( if(enabled) ASSERT( Enums::k_unit_grenade_types_count == *count_ref, "GrenadeCounts asm mismtach!" ) );
		*count_ref = count;
	}
}



static void InitializeForYeloGameState_UnitGrenadeCounts(bool enabled)
{
	//////////////////////////////////////////////////////////////////////////
	// actor_died
	{
		// mov	word ptr [ebx+31Eh], 0
		static const byte k_unit_grenade_count_word[] = { 0x66, 0xC7, 0x83, 0x1E, 0x03, 0x00, 0x00, 0x00, 0x00, };
		// xor	ecx, ecx
		// mov	dword ptr [ebx+31Eh], ecx
		static const byte k_unit_grenade_count_dword[] = { 0x33, 0xC9, 
			0x89, 0x8B, 0x1E, 0x03, 0x00, 0x00, Enums::_x86_opcode_nop };
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word) == sizeof(k_unit_grenade_count_dword) );

		size_t asm_code_size = sizeof(k_unit_grenade_count_word);
		const void* asm_code = enabled ? k_unit_grenade_count_dword : k_unit_grenade_count_word;
		void* code_addr = GET_FUNC_VPTR(ACTOR_DIED_UNIT_GRENADE_COUNT_MOD);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);
	}
	//////////////////////////////////////////////////////////////////////////
	// player_add_equipment
	{
		// mov	[ebp+31Eh], ax
		static const byte k_unit_grenade_count_word[] = { 0x66, 0x89, 0x85, 0x1E, 0x03, 0x00, 0x00 };
		// mov	[ebp+31Eh], eax
		static const byte k_unit_grenade_count_dword[] = {		0x89, 0x85, 0x1E, 0x03, 0x00, 0x00, Enums::_x86_opcode_nop };
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word) == sizeof(k_unit_grenade_count_dword) );

		size_t asm_code_size = sizeof(k_unit_grenade_count_word);
		const void* asm_code = enabled ? k_unit_grenade_count_dword : k_unit_grenade_count_word;
		void* code_addr = GET_FUNC_VPTR(PLAYER_ADD_EQUIPMENT_UNIT_GRENADE_COUNT_MOD);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_new_from_network
	{
		// mov	dx, [eax+52Ch]
		// mov	[eax+31Eh], dx
		static const byte k_unit_grenade_count_word[] = {
			0x66,	0x8B, 0x90, 0x2C, 0x05, 0x00, 0x00,
			0x66,	0x89, 0x90, 0x1E, 0x03, 0x00, 0x00,
		};
		static const byte k_unit_grenade_count_dword[] = {
					0x8B, 0x90, 0x2C, 0x05, 0x00, 0x00, Enums::_x86_opcode_nop,
					0x89, 0x90, 0x1E, 0x03, 0x00, 0x00, Enums::_x86_opcode_nop,
		};
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word) == sizeof(k_unit_grenade_count_dword) );

		size_t asm_code_size = sizeof(k_unit_grenade_count_word);
		const void* asm_code = enabled ? k_unit_grenade_count_dword : k_unit_grenade_count_word;
		void* code_addr = GET_FUNC_VPTR(BIPED_NEW_FROM_NETWORK_UNIT_GRENADE_COUNT_MOD);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_update_baseline
	{
		// mov	cx, [eax+31Eh]
		static const byte k_unit_grenade_count_word1[] = { 0x66,0x8B, 0x88, 0x1E, 0x03, 0x00, 0x00 };
		// mov	[eax+52Ch], cx
		static const byte k_unit_grenade_count_word2[] = { 0x66,0x89, 0x88, 0x2C, 0x05, 0x00, 0x00 };

		static const byte k_unit_grenade_count_dword1[] = {		0x8B, 0x88, 0x1E, 0x03, 0x00, 0x00, Enums::_x86_opcode_nop };
		static const byte k_unit_grenade_count_dword2[] = {		0x89, 0x88, 0x2C, 0x05, 0x00, 0x00, Enums::_x86_opcode_nop };
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word1) == sizeof(k_unit_grenade_count_dword1) );
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word2) == sizeof(k_unit_grenade_count_dword2) );

		size_t asm_code_size = sizeof(k_unit_grenade_count_word1);
		const void* asm_code = enabled ? k_unit_grenade_count_dword1 : k_unit_grenade_count_word1;
		void* code_addr = GET_FUNC_VPTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD1);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word1, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);

		asm_code_size = sizeof(k_unit_grenade_count_word2);
		asm_code = enabled ? k_unit_grenade_count_dword2 : k_unit_grenade_count_word2;
		code_addr = GET_FUNC_VPTR(BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD2);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word2, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_build_update_delta
	{
		// mov	dx, [edi+31Eh]
		static const byte k_unit_grenade_count_word1[] = { 0x66,0x8B, 0x97, 0x1E, 0x03, 0x00, 0x00 };
		// mov	[esp+44h+var_10], dx
		static const byte k_unit_grenade_count_word2[] = { 0x66,0x89, 0x54, 0x24, 0x34, };

		static const byte k_unit_grenade_count_dword1[] = {		0x8B, 0x97, 0x1E, 0x03, 0x00, 0x00, Enums::_x86_opcode_nop };
		static const byte k_unit_grenade_count_dword2[] = {		0x89, 0x54, 0x24, 0x34, Enums::_x86_opcode_nop };
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word1) == sizeof(k_unit_grenade_count_dword1) );
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word2) == sizeof(k_unit_grenade_count_dword2) );

		size_t asm_code_size = sizeof(k_unit_grenade_count_word1);
		const void* asm_code = enabled ? k_unit_grenade_count_dword1 : k_unit_grenade_count_word1;
		void* code_addr = GET_FUNC_VPTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD1);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word1, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);

		asm_code_size = sizeof(k_unit_grenade_count_word2);
		asm_code = enabled ? k_unit_grenade_count_dword2 : k_unit_grenade_count_word2;
		code_addr = GET_FUNC_VPTR(BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD2);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word2, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);
	}
	//////////////////////////////////////////////////////////////////////////
	// biped_process_update_delta
	{
		// mov	word ptr [esi+31Eh], ax
		static const byte k_unit_grenade_count_word[] = { 0x66, 0x89, 0x86, 0x1E, 0x03, 0x00, 0x00, };
		// mov	dword ptr [esi+31Eh], eax
		static const byte k_unit_grenade_count_dword[] = {		0x89, 0x86, 0x1E, 0x03, 0x00, 0x00, Enums::_x86_opcode_nop };
		BOOST_STATIC_ASSERT( sizeof(k_unit_grenade_count_word) == sizeof(k_unit_grenade_count_dword) );

		size_t asm_code_size = sizeof(k_unit_grenade_count_word);
		const void* asm_code = enabled ? k_unit_grenade_count_dword : k_unit_grenade_count_word;
		void* code_addr = GET_FUNC_VPTR(BIPED_PROCESS_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD);

		DebugOnly( if(enabled) ASSERT( memcmp(code_addr, k_unit_grenade_count_word, asm_code_size)==0, "GrenadeCounts asm mismtach!" ) );
		Memory::WriteMemory(code_addr, asm_code, asm_code_size);
	}
}


static void InitializeForYeloGameState_MessageDeltaGrenadeCounts(bool enabled)
{
#ifndef YELO_NO_NETWORK
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

	if(g_stock_grenade_counts.maximum_size_calculator == NULL)
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

			const grenade_counts* baseline_data_gc = CAST_PTR(const grenade_counts*, baseline_data); // should always be NULL...
			const grenade_counts* source_data_gc = CAST_PTR(const grenade_counts*, source_data);

			bits_written =  g_stock_grenade_counts.encoder(field_properties, baseline_data_gc, source_data_gc,   output_stream);

			baseline_data_gc = baseline_data_gc != NULL ? baseline_data_gc+1 : NULL;
			source_data_gc += 1;

			bits_written += g_stock_grenade_counts.encoder(field_properties, baseline_data_gc, source_data_gc+1, output_stream);

			return bits_written;
		}

		static int32 PLATFORM_API decoder(field_properties_definition* field_properties, void* baseline_data, void* destination_data, Memory::s_bitstream* input_stream)
		{
			int32 bits_read = 0;

			grenade_counts* baseline_data_gc = CAST_PTR(grenade_counts*, baseline_data); // should always be NULL...
			grenade_counts* destination_data_gc = CAST_PTR(grenade_counts*, destination_data);

			// stock decoder doesn't use baseline_data when decoding
			bits_read =  g_stock_grenade_counts.decoder(field_properties, baseline_data_gc, destination_data_gc, input_stream);

			baseline_data_gc = baseline_data_gc != NULL ? baseline_data_gc+1 : NULL;
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
			->maximum_size = g_fixed_width_weapon_index.maximum_size_calculator(NULL); // release builds don't use the prop parameter in this field type
	}
	else
	{
		g_stock_grenade_counts
			.CopyTo(grenade_counts_type, grenade_counts_props)
			->maximum_size = Enums::k_bits_in_grenade_counts_type;
		g_stock_fixed_width_grenade_index
			.CopyTo(fixed_width_grenade_index_type, fixed_width_grenade_index_props)
			->maximum_size = g_stock_fixed_width_grenade_index.maximum_size_calculator(NULL); // release builds don't use the prop parameter in this field type
	}
#endif
}