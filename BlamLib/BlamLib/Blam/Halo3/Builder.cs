/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Halo3
{
	public class BuilderItem : Blam.Cache.BuilderItem
	{
		public override void PreProcess(BlamLib.Blam.Cache.BuilderBase owner)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public override bool Build(BlamLib.Blam.Cache.BuilderBase owner)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public override void PostProcess(BlamLib.Blam.Cache.BuilderBase owner)
		{
			throw new Exception("The method or operation is not implemented.");
		}
	};

	public class Builder : Blam.Cache.BuilderBase
	{
		public Builder() : base(BlamVersion.Unknown) { }

		/// <summary>
		/// Helper method to get a Halo3 BuilderItem object
		/// </summary>
		/// <param name="index">Index of the tag to get</param>
		/// <returns>BuilderItem at <paramref name="index"/></returns>
		public BuilderItem this[int index] { get { return tags[index] as BuilderItem; } }

		private BuilderItem AddTagInternal()
		{
			return null;
		}

		/// <summary>
		/// Adds a new tag to the list
		/// </summary>
		/// <remarks>Item will only have its index value set</remarks>
		/// <returns>Reference to the new BuilderItem</returns>
		public override BlamLib.Blam.Cache.BuilderItem AddTag() { return AddTagInternal(); }
	};
}