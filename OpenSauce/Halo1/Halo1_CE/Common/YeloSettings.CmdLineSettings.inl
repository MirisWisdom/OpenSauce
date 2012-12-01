/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

CMDLINE_DEFINE_ARG(path, cstring)
CMDLINE_DEFINE_SWITCH(use_os_gamestate)

#if PLATFORM_IS_DEDI
#else
	CMDLINE_DEFINE_ARG(mp_version, cstring)
	CMDLINE_DEFINE_SWITCH(no_os_gfx)
#endif

// These must come last, so including file doesn't have to undef anything
#undef CMDLINE_DEFINE_ARG
#undef CMDLINE_DEFINE_SWITCH