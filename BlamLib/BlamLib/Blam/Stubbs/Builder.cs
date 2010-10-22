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

namespace BlamLib.Blam.Stubbs
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
		/// Helper method to get a Stubbs BuilderItem object
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