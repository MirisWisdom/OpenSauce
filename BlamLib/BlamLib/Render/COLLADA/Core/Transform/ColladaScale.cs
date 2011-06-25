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
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	public partial class ColladaScale : ColladaSIDValueArray<float>
	{
		public ColladaScale() : base(Enums.ColladaElementType.Core_Scale) { }
		public ColladaScale(float scale_x, float scale_y, float scale_z)
			: this()
		{
			SetScale(scale_x, scale_y, scale_z);
		}
		public ColladaScale(BlamLib.TagInterface.RealVector3D scale)
			: this()
		{
			SetScale(scale);
		}

		public void SetScale(float scale_x, float scale_y, float scale_z)
		{
			Clear();
			Add(scale_x, scale_y, scale_z);
		}
		public void SetScale(BlamLib.TagInterface.RealVector3D scale)
		{
			Clear();
			Add(scale.I, scale.J, scale.K);
		}
	}
}