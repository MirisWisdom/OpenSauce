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