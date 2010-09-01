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


static void PLATFORM_API scenario_compile_scripts(void** arguments)
{
	struct s_arguments {
		cstring scenario_name;
	}* args = CAST_PTR(s_arguments*, arguments);

	// load scenario tag

	// call...ugh why do I even try? :|

	// save scenario tag

	// might have to call some of the system dispose functions

	printf_s("FUCK YOU! Oh and %s has to be the worse scenario ever. kill you're saelf, hth.\n", args->scenario_name);
	printf_s("If you can see this, it means this feature hasn't been implemented.\n");
}