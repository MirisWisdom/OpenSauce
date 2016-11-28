/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
                // push	0x55A0C0		// Push our dll string
                // call	ds:[0x54D078]	// call LoadLibrary
                // jmp	0x53F5F0		// goto __SEH_prolog
				BlamLib.Util.ByteStringToArray("C3" + "68C0A05500" + "FF1578D05400" + "E9BF590F00")
                
				);

			/// <summary>Initializer's hook code for each game tool</summary>
			/// <remarks>
			/// offset	<see cref="AddressOf.OpenSauceFunction2"/>
			/// call	<see cref="AddressOf.OpenSauceFunction1"/>
			/// </remarks>
			public static PlatformData OpenSauceFunction2 = new PlatformData(
				// call	0x449FE1		// +1 Call our setup code listed above
				BlamLib.Util.ByteStringToArray("E8BECDE7FF"),

				// call	0x449C21		// +1 Call our setup code listed above
                BlamLib.Util.ByteStringToArray("E8B4BBF0FF")
				);
			#endregion
		};
	};
};