/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

CMDLINE_DEFINE_SWITCH(full_dump)
CMDLINE_DEFINE_SWITCH(disable_exception_handling)

#if (PLATFORM_TYPE == PLATFORM_SAPIEN) || (!PLATFORM_IS_EDITOR)
	CMDLINE_DEFINE_SWITCH(no_os_gamestate)
#endif

#if !PLATFORM_IS_EDITOR
	CMDLINE_DEFINE_ARG(path, cstring)

	CMDLINE_DEFINE_ARG(freeze_dump, real)
	CMDLINE_DEFINE_ARG(freeze_dump_delay, real)

	#if PLATFORM_IS_DEDI
	#else
		CMDLINE_DEFINE_ARG(mp_version, cstring)
		CMDLINE_DEFINE_SWITCH(no_os_gfx)
	#endif
#endif

// These must come last, so including file doesn't have to undef anything
#undef CMDLINE_DEFINE_ARG
#undef CMDLINE_DEFINE_SWITCH