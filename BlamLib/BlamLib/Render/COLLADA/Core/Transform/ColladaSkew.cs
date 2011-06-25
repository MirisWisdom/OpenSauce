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
	public partial class ColladaSkew : ColladaSIDValueArray<float>
	{
		public ColladaSkew() : base(Enums.ColladaElementType.Core_Skew) { }
		public ColladaSkew(float angle,
			float trans_column_x, float trans_column_y, float trans_column_z,
			float rot_column_x, float rot_column_y, float rot_column_z)
			: this() 
		{
			SetSkew(angle, trans_column_x, trans_column_y, trans_column_z, rot_column_x, rot_column_y, rot_column_z);
		}
		public ColladaSkew(float angle,
			BlamLib.TagInterface.RealVector3D trans_column,
			BlamLib.TagInterface.RealVector3D rot_column)
			: this()
		{
			SetSkew(angle, trans_column, rot_column);
		}

		public void SetSkew(float angle,
			float trans_column_x, float trans_column_y, float trans_column_z,
			float rot_column_x, float rot_column_y, float rot_column_z)
		{
			Clear();
			Add(angle,
				trans_column_x, trans_column_y, trans_column_z,
				rot_column_x, rot_column_y, rot_column_z);
		}
		public void SetSkew(float angle,
			BlamLib.TagInterface.RealVector3D trans_column,
			BlamLib.TagInterface.RealVector3D rot_column)
		{
			Clear();
			Add(angle,
				trans_column.I, trans_column.J, trans_column.K,
				rot_column.I, rot_column.J, rot_column.K);
		}
	}
}