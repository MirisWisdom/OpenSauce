/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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