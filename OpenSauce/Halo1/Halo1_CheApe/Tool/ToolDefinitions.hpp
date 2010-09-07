/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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