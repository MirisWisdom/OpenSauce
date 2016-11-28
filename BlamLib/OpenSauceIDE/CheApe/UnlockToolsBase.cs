/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;

namespace OpenSauceIDE.CheApeInterface
{
	abstract class UnlockToolsBase : UnlockExeBase
	{
		protected enum Platform
		{
			Guerilla,
			Tool,
			Sapien,
		};

		protected struct PlatformAddress
		{
			public uint Guerilla, Tool, Sapien;

			public PlatformAddress(uint g, uint t, uint s) { Guerilla = g; Tool = t; Sapien = s; }

			public uint this[Platform p] {
				get {
					switch(p)
					{
						case Platform.Guerilla: return Guerilla;
						case Platform.Tool:		return Tool;
						case Platform.Sapien:	return Sapien;
						default: throw new Exception();
					}
				}
			}
		};

		protected class PlatformData
		{
			public byte[] Guerilla, Tool, Sapien;

			public PlatformData(byte[] g, byte[] t, byte[] s) { Guerilla = g; Tool = t; Sapien = s; }

			public byte[] this[Platform p]
			{
				get
				{
					switch (p)
					{
						case Platform.Guerilla:	return Guerilla;
						case Platform.Tool:		return Tool;
						case Platform.Sapien:	return Sapien;
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