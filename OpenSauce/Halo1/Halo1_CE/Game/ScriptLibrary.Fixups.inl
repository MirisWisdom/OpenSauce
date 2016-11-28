/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
// This actually isn't entirely needed. However, the yelo scripting definitions include the definition 
// index and later versions of the blam engine don't use the name to perform look-ups on script definitions, 
// they use the actual definition index. So...I'm just looking to be a little more consistent and who knows 
// maybe some of these may serve a purpose in being implemented? Yeah because we all know how much better 
// lightmaps are when you run radiosity in-game %-)

//////////////////////////////////////////////////////////////////////////
// hs definitions
HS_FUNCTION_WITH_PARAMS(debug_pvs, void, "", "", 1,
	HS_TYPE(bool)
);
HS_FUNCTION_WITH_PARAMS(radiosity_start, void, "", "", 1,
	HS_TYPE(bool)
);
HS_FUNCTION_WITH_PARAMS(radiosity_save, void, "", "", 1,
	HS_TYPE(bool)
);
HS_FUNCTION_WITH_PARAMS(radiosity_debug_point, void, "", "", 1,
	HS_TYPE(bool)
);

HS_FUNCTION_WITH_PARAMS(debug_teleport_player, void, "", "", 2,
	HS_TYPE(short),
	HS_TYPE(short)
);

HS_FUNCTION_WITH_PARAMS(hud_team_icon_set_pos, void, "", "", 2,
	HS_TYPE(long),
	HS_TYPE(long)
);
HS_FUNCTION_WITH_PARAMS(hud_team_icon_set_scale, void, "", "", 2,
	HS_TYPE(real),
	HS_TYPE(real)
);
HS_FUNCTION_WITH_PARAMS(hud_team_background_set_pos, void, "", "", 2,
	HS_TYPE(long),
	HS_TYPE(long)
);
HS_FUNCTION_WITH_PARAMS(hud_team_background_set_scale, void, "", "", 2,
	HS_TYPE(real),
	HS_TYPE(real)
);

HS_FUNCTION_WITH_PARAMS(reload_shader_transparent_chicago, void, "", "", 1,
	HS_TYPE(bool)
);
HS_FUNCTION_WITH_PARAMS(rasterizer_reload_effects, void, "", "", 1,
	HS_TYPE(bool)
);

HS_FUNCTION_WITH_PARAMS(oid_watch, void, "", "", 1,
	HS_TYPE(string)
);
HS_FUNCTION_WITH_PARAMS(oid_dump, void, "", "", 1,
	HS_TYPE(string)
);
HS_FUNCTION_WITH_PARAMS(oid_status, void, "", "", 1,
	HS_TYPE(string)
);

#if PLATFORM_IS_DEDI // These don't appear in the dedi build, add them
	HS_FUNCTION_WITH_PARAMS(sound_enable_eax, void, "", "", 1,
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(sound_eax_enabled, void, "", "", 1,
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(sound_set_env, void, "", "", 1,
		HS_TYPE(short)
	);
	HS_FUNCTION_WITH_PARAMS(sound_enable_hardware, void, "", "", 2,
		HS_TYPE(bool),
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(sound_set_supplementary_buffers, void, "", "", 2,
		HS_TYPE(short),
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(sound_get_supplementary_buffers, void, "", "", 2,
		HS_TYPE(short),
		HS_TYPE(bool)
	);
	HS_FUNCTION_WITH_PARAMS(sound_set_rolloff, void, "", "", 1,
		HS_TYPE(real)
	);
	HS_FUNCTION_WITH_PARAMS(sound_set_factor, void, "", "", 1,
		HS_TYPE(real)
	);
#endif

HS_GLOBAL(radiosity_quality, short, nullptr);
HS_GLOBAL(radiosity_step_count, short, nullptr);
HS_GLOBAL(radiosity_lines, bool, nullptr);
HS_GLOBAL(radiosity_normals, bool, nullptr);
//////////////////////////////////////////////////////////////////////////

struct s_fixup_globals {
	struct fixup {
		int32 index;
		union {
			void* address;

			hs_function_definition* function;
			hs_global_definition* global;
		};
		bool is_global;
		PAD24;
	};

#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
	fixup fixups[];
#pragma warning( pop )
}_fixup_globals = {
	{ // fixups
		{0x11C, &GET_HS_FUNCTION(debug_pvs)},
		{0x11D, &GET_HS_FUNCTION(radiosity_start)},
		{0x11E, &GET_HS_FUNCTION(radiosity_save)},
		{0x11F, &GET_HS_FUNCTION(radiosity_debug_point)},

		{0x170, &GET_HS_FUNCTION(debug_teleport_player)},

		{0x17D, &GET_HS_FUNCTION(hud_team_icon_set_pos)},
		{0x17E, &GET_HS_FUNCTION(hud_team_icon_set_scale)},
		{0x17F, &GET_HS_FUNCTION(hud_team_background_set_pos)},
		{0x180, &GET_HS_FUNCTION(hud_team_background_set_scale)},

		{0x1A8, &GET_HS_FUNCTION(reload_shader_transparent_chicago)},
		{0x1A9, &GET_HS_FUNCTION(rasterizer_reload_effects)},

#if PLATFORM_IS_DEDI // These don't appear in the dedi build, add them
		{0x1C0, &GET_HS_FUNCTION(sound_enable_eax)},
		{0x1C1, &GET_HS_FUNCTION(sound_eax_enabled)},
		{0x1C2, &GET_HS_FUNCTION(sound_set_env)},
		{0x1C3, &GET_HS_FUNCTION(sound_enable_hardware)},
		{0x1C4, &GET_HS_FUNCTION(sound_set_supplementary_buffers)},
		{0x1C5, &GET_HS_FUNCTION(sound_get_supplementary_buffers)},
		{0x1C6, &GET_HS_FUNCTION(sound_set_rolloff)},
		{0x1C7, &GET_HS_FUNCTION(sound_set_factor)},
#endif

		{0x21C, &GET_HS_FUNCTION(oid_watch)},
		{0x21D, &GET_HS_FUNCTION(oid_dump)},
		{0x21E, &GET_HS_FUNCTION(oid_status)},

		{0x112, &GET_HS_GLOBAL(radiosity_quality), true},
		{0x113, &GET_HS_GLOBAL(radiosity_step_count), true},
		{0x114, &GET_HS_GLOBAL(radiosity_lines), true},
		{0x115, &GET_HS_GLOBAL(radiosity_normals), true},

		{NONE}
	},
};

static void NullifyScriptLibraryFixups()
{
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(debug_pvs) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(radiosity_start) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(radiosity_save) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(radiosity_debug_point) );

	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(debug_teleport_player) );

	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(hud_team_icon_set_pos) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(hud_team_icon_set_scale) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(hud_team_background_set_pos) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(hud_team_background_set_scale) );

	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(reload_shader_transparent_chicago) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(rasterizer_reload_effects) );

	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(oid_watch) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(oid_dump) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(oid_status) );

#if PLATFORM_IS_DEDI
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_enable_eax) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_eax_enabled) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_set_env) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_enable_hardware) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_set_supplementary_buffers) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_get_supplementary_buffers) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_set_rolloff) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sound_set_factor) );
#endif
}

static void InitializeLibraryFixups()
{
	NullifyScriptLibraryFixups();

	//////////////////////////////////////////////////////////////////////////
	// Add back functions/globals which don't appear in the release builds (and the like).
	// The expressions don't do anything...unless you find a reason to implement them...
	s_fixup_globals::fixup* fixups = _fixup_globals.fixups;
	while(fixups->index != NONE)
	{
		if(!fixups->is_global)
		{
			_upgrade_globals.functions.table[fixups->index] = fixups->function;
			_upgrade_globals.functions.count++;
		}
		else
		{
			_upgrade_globals.globals.table[fixups->index] = fixups->global;
			_upgrade_globals.globals.count++;
		}
		
		fixups++;
	}
	//////////////////////////////////////////////////////////////////////////
}