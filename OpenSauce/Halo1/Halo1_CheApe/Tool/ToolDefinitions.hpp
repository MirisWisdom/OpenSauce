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
		// have to define it here because build-cache-file code uses it
		typedef void (PLATFORM_API* _import_class_proc)(void** arguments);

		struct s_import_class {
			cstring				name;
			cstring				usage;
			int32				argument_count;
			_import_class_proc	import_proc;
		};
#define IMPORT_CLASS_DEFINITION(name, arg_count, arguments, proc) \
		{name, name " " arguments, arg_count, proc}
	};
};
#endif