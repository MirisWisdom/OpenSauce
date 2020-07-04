/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Halo1
{
	public class BuilderItem : Blam.Cache.BuilderItem
	{
		/// <summary>Only for satisfying <see cref="BlamLib.Managers.DataArray{T}"/>'s template constraints</summary>
		public BuilderItem() {}

		internal BuilderItem(Blam.Cache.BuilderTagIndexBase owner, Managers.TagManager source) : base(owner, source)
		{
		}

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
		/// Helper method to get a Halo1 BuilderItem object
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
		public override BlamLib.Blam.Cache.BuilderItem AddTag() { return AddTagInternal();  }
	};

	public class BuilderTagIndex : Blam.Cache.BuilderTagIndexBase
	{
		Managers.DataArray<BuilderItem> Array;

		public BuilderTagIndex(BlamVersion version, Managers.ITagIndex source_index) : base(version, source_index)
		{
			int max_tag_count = 1024;

			var g = Program.GetManager(version).FindGame(version);
			if (g != null) max_tag_count = g.Tags.MaxCount;

			Array = new Managers.DataArray<BuilderItem>(max_tag_count, "builder tag instances");
			DataArraySet(Array);
		}

		protected override Blam.Cache.BuilderItem BuildFromSource(Managers.TagManager source_tag)
		{
			BuilderItem bi = new BuilderItem(this, source_tag);
			bi.Datum = Array.Add(bi);
			return bi;
		}
	};
}