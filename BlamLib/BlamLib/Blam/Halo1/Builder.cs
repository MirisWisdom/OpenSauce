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