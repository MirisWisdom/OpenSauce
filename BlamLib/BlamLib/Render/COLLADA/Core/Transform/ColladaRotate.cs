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
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	public partial class ColladaRotate : ColladaSIDValueArray<float>
	{
		public ColladaRotate() : base(Enums.ColladaElementType.Core_Rotate) { }
		public ColladaRotate(float column_x, float column_y, float column_z,
			float angle)
			: this()
		{
			SetRotate(column_x, column_y, column_z, angle);
		}

		public void SetRotate(
			float column_x, float column_y, float column_z,
			float angle)
		{
			Clear();
			Add(column_x, column_y, column_z, angle);
		}
	}
}