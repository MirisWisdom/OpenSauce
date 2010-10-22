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

/*
6 value
10 color
13 anims

animated parameters - 
0000FF00 - value type (bitmap, color etc)
000000FF - index
*/
namespace BlamLib.Blam.Halo2.Tags
{
	internal interface IShaderParameter
	{
		TI.TagReference ParameterBitmap();
		TI.Real ParameterValue();
		TI.RealColor ParameterColor();
	};

	#region global_shader_parameter_block
	partial class global_shader_parameter_block
	{
		public TI.TagReference ParameterBitmap() { return Bitmap; }

		public TI.Real ParameterValue() { return ConstValue; }

		public TI.RealColor ParameterColor() { return ConstColor; }
	};
	#endregion


	#region shader
	partial class shader_group
	{
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			PredictedResources.DeleteAll();

			return true;
		}
	};
	#endregion
}