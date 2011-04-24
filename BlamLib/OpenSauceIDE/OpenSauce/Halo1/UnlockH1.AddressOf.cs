/*
    BlamLib: .NET SDK for the Blam Engine

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

namespace OpenSauceIDE.GameInterface
{
	partial class UnlockH1
	{
		static class AddressOf
		{
			#region Open Sauce
			/// <summary>Address to place our DLL string at</summary>
			public static PlatformAddress OpenSauceDllString = new PlatformAddress
                (0x5FE7C0, 0x55A0C0); // where "reference function: %s\r\n" used to be
			/// <summary>LoadLibraryA address in each EXE</summary>
			public static PlatformAddress OpenSauceLoadLibrary = new PlatformAddress
                (0x5DF0A4, 0x54D078);

			/// <summary>Address to place our initializer code</summary>
			public static PlatformAddress OpenSauceFunction1 = new PlatformAddress
                (0x449FE0, 0x449C20); // where _write_to_error_file used to be

			/// <summary>Address to place our hook code at which calls the initializer</summary>
			public static PlatformAddress OpenSauceFunction2 = new PlatformAddress
				(0x5CD21E, 0x53E068);
			#endregion
		};
	};
}