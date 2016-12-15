/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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

			Name.Read(s, (s.Owner as CacheFile).StringIds.Definition.Description);
		}
	};
	#endregion

	#region File
	/// <summary>
	/// Halo Odst implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public sealed class CacheFile : Halo3.CacheFileBase
	{
		#region GetCacheFileResourceDefinitionFactory
		class CacheFileResourceDefinitionFactory : Cache.Tags.CacheFileResourceDefinitionFactory
		{
			public override BlamLib.TagInterface.Definition GenerateStructureBspTagResource()
			{ return new Halo3.Tags.scenario_structure_bsp_group.structure_bsp_tag_resources_odst(); }
		};
		public override Cache.Tags.CacheFileResourceDefinitionFactory GetCacheFileResourceDefinitionFactory()
		{
			return new CacheFileResourceDefinitionFactory();
		}
		#endregion

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