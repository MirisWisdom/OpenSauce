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
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.HaloReach.Tags
{
	#region hs_scripts_block
	[TI.Definition(3, 24)]
	public class hs_scripts_block : Scripting.hs_scripts_block
	{
		public TI.Block<Blam.Halo3.Tags.hs_scripts_block.hs_scripts_parameters_block> Parameters;

		#region Ctor
		public hs_scripts_block() : base(5)
		{
			Add(Name = new TI.StringId());
			Add(ScriptType = new TI.Enum());
			Add(ReturnType = new TI.Enum());
			Add(RootExpressionIndex = new TI.LongInteger());
			Add(Parameters = new TI.Block<Blam.Halo3.Tags.hs_scripts_block.hs_scripts_parameters_block>(this, 0));
		}
		#endregion
	}
	#endregion
}