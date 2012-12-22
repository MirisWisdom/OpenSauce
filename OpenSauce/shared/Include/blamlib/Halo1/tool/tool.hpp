/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_ID == PLATFORM_TOOL

namespace Yelo
{
	namespace Tool
	{
		typedef void (PLATFORM_API* proc_import_class)(void** arguments);

		struct s_import_class {
			cstring				name;
			cstring				usage;
			int32				argument_count;
			proc_import_class	import_proc;
		};
#define IMPORT_CLASS_DEFINITION(name, arg_count, arguments, proc) \
		{name, name " " arguments, arg_count, proc}
	};
};
#endif