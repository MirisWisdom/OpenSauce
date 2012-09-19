/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

CMDLINE_DEFINE_ARG(path, char*);

#if PLATFORM_IS_DEDI
	CMDLINE_DEFINE_ARG(http_root, char*);
	CMDLINE_DEFINE_ARG(http_threads, int32);
#else
	CMDLINE_DEFINE_ARG(mp_version, char*);
	CMDLINE_DEFINE_SWITCH(no_os_gfx);
#endif
