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
		static class Constants
		{
			#region Open Sauce
			public static PlatformData OpenSauceDllString = new PlatformData(
				// "OpenSauceClient.dll\0"
				BlamLib.Util.ByteStringToArray("4F70656E5361756365436C69656E742E646C6C00"),
				// "OpenSauceDedi.dll\0"
				BlamLib.Util.ByteStringToArray("4F70656E5361756365446564692E646C6C00")
				);

			/// <summary>Initializer code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.OpenSauceFunction1"/>
			/// retn
			/// push	<see cref="AddressOf.OpenSauceDllString"/>
			/// call	ds:[<see cref="AddressOf.OpenSauceLoadLibrary"/>]
			/// jmp		__SEH_prolog
			/// </remarks>
			public static PlatformData OpenSauceFunction1 = new PlatformData(
				// retn
				// push	0x5FE7C0		// Push our dll string
				// call	ds:[0x5DF0A4]	// call LoadLibrary
				// jmp	0x5CF5E0		// goto __SEH_prolog
				BlamLib.Util.ByteStringToArray("C3" + "68C0E75F00" + "FF15A4F05D00" + "E9EF551800"),

				// retn
				// push	0x55A100		// Push our dll string
				// call	ds:[0x54D0A0]	// call LoadLibrary
				// jmp	0x53F5F0		// goto __SEH_prolog
				BlamLib.Util.ByteStringToArray("C3" + "6800A15500" + "FF15A0D05400" + "E9CF590F00")
				);

			/// <summary>Initializer's hook code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.OpenSauceFunction2"/>
			/// call	<see cref="AddressOf.OpenSauceFunction1"/>
			/// </remarks>
			public static PlatformData OpenSauceFunction2 = new PlatformData(
				// call	0x449FE1		// +1 Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E8BECDE7FF"),

				// call	0x449C11		// +1 Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E8A4BBF0FF")
				);
			#endregion
		};
	};
};