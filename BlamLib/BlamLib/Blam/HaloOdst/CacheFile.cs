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
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.HaloOdst
{
	#region Index
	/// <summary>
	/// Halo Odst implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public sealed class CacheIndex : Halo3.CacheIndexBase
	{
		protected override Cache.CacheItemGroupTagGen3[] CreateEngineItemGroupTags(int count)
		{
			return new CacheItemGroupTag[count];
		}

		protected override Cache.CacheItemGroupTagGen3 CreateEngineItemGroupTag()
		{
			return new CacheItemGroupTag();
		}
	};
	#endregion

	#region ItemGroupTag
	public sealed class CacheItemGroupTag : Cache.CacheItemGroupTagGen3
	{
		public override void Read(BlamLib.IO.EndianReader s)
		{
			base.ReadGroupTags(Program.HaloOdst.Manager, s);

			Name.Read(s);
		}
	};
	#endregion

	#region File
	/// <summary>
	/// Halo Odst implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public sealed class CacheFile : Halo3.CacheFileBase
	{
		protected override bool SharableReferenceXbox(string path)
		{
			if (SharableReference(path, Program.HaloOdst.XboxMainmenu)) ShareCacheStreams(this, Program.HaloOdst.XboxMainmenu);
			else if (SharableReference(path, Program.HaloOdst.XboxShared)) ShareCacheStreams(this, Program.HaloOdst.XboxShared);
			else if (SharableReference(path, Program.HaloOdst.XboxCampaign)) ShareCacheStreams(this, Program.HaloOdst.XboxCampaign);
			else return false;

			return true;
		}

		public CacheFile(string map_name) : base(map_name)
		{
			engineVersion = BlamVersion.HaloOdst;

			cacheIndex = new CacheIndex();
		}

		protected override void DetermineEngineVersion()
		{
			switch (cacheHeader.Build.Substring(20))
			{
				case "atlas_relea":
					engineVersion = BlamVersion.HaloOdst_Xbox;
					break;

				default:
					throw new Debug.ExceptionLog("Unreachable!");
			}
		}
	};
	#endregion
}