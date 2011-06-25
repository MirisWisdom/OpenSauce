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
	public partial class ColladaLookat : ColladaSIDValueArray<float>
	{
		public ColladaLookat() : base(Enums.ColladaElementType.Core_Lookat) { }
		public ColladaLookat(float eye_x, float eye_y, float eye_z,
			float interest_x, float interest_y, float interest_z,
			float up_x, float up_y, float up_z)
			: this()
		{
			SetLookAt(eye_x, eye_y, eye_z,
				interest_x, interest_y, interest_z,
				up_x, up_y, up_z);
		}
		public ColladaLookat(BlamLib.TagInterface.RealPoint3D eye_position,
			BlamLib.TagInterface.RealPoint3D interest_position,
			BlamLib.TagInterface.RealVector3D up_vector)
			: this()
		{
			SetLookAt(eye_position,
				interest_position,
				up_vector);
		}

		public void SetLookAt(
			float eye_x, float eye_y, float eye_z,
			float interest_x, float interest_y, float interest_z,
			float up_x, float up_y, float up_z)
		{
			Clear();
			Add(eye_x, eye_y, eye_z, interest_x, interest_y, interest_z, up_x, up_y, up_z);
		}
		public void SetLookAt(
			BlamLib.TagInterface.RealPoint3D eye_position,
			BlamLib.TagInterface.RealPoint3D interest_position,
			BlamLib.TagInterface.RealVector3D up_vector)
		{
			Clear();
			Add(eye_position.X, eye_position.Y, eye_position.Z,
				interest_position.X, interest_position.Y, interest_position.Z,
				up_vector.I, up_vector.J, up_vector.K);
		}
	}
}