/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

DOC_TODO_DEBUG("Update these if you change Client platform version")
// These are the same build strings the stock game checks when verifying a game save
static cstring k_binary_compatible_build_numbers_stock[] = {
#if PLATFORM_VERSION == 0x1080
	"01.00.08.0616",
#elif PLATFORM_VERSION == 0x1090
	"01.00.09.0620",
#endif
	"01.00.00.0564",
	"01.00.01.0580",
	"01.00.02.0581",
};

static cstring k_build_number_yelo_current = BOOST_STRINGIZE( K_OPENSAUCE_VERSION_HCE_BUILD_NUMBER(PLATFORM_VERSION_BUILD) );
static cstring k_binary_compatible_build_numbers_yelo[] = {
	k_build_number_yelo_current
};