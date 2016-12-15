/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;

namespace OpenSauceIDE.GameInterface
{
	abstract class UnlockGameBase : UnlockExeBase
	{
		protected enum Platform
		{
			Client,
			Dedi,
		};

		protected struct PlatformAddress
		{
			public uint Client, Dedi;

			public PlatformAddress(uint c, uint d) { Client = c; Dedi = d; }

			public uint this[Platform p] {
				get {
					switch(p)
					{
						case Platform.Client:	return Client;
						case Platform.Dedi:		return Dedi;
						default: throw new Exception();
					}
				}
			}
		};

		protected class PlatformData
		{
			public byte[] Client, Dedi;

			public PlatformData(byte[] c, byte[] d) { Client = c; Dedi = d; }

			public byte[] this[Platform p]
			{
				get
				{
					switch (p)
					{
						case Platform.Client:	return Client;
						case Platform.Dedi:		return Dedi;
						default: throw new Exception();
					}
				}
			}
		};

		protected abstract class PlatformInterfaceBase : PlatformInterfaceGenericBase
		{
			protected Platform PlatformVersion;

			protected PlatformInterfaceBase(Platform pv)
			{
				PlatformVersion = pv;
			}
		};
	};
}