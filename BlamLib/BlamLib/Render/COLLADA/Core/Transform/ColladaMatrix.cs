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
	public partial class ColladaMatrix : ColladaSIDValueArray<float>
	{
		public ColladaMatrix() : base(Enums.ColladaElementType.Core_Matrix) { }
		public ColladaMatrix(float m1x1, float m2x1, float m3x1, float m4x1,
			float m1x2, float m2x2, float m3x2, float m4x2,
			float m1x3, float m2x3, float m3x3, float m4x3,
			float m1x4, float m2x4, float m3x4, float m4x4)
			: this()
		{
			SetMatrix(m1x1, m2x1, m3x1, m4x1,
				m1x2, m2x2, m3x2, m4x2,
				m1x3, m2x3, m3x3, m4x3,
				m1x4, m2x4, m3x4, m4x4);
		}

		public void SetMatrix(
			float m1x1, float m2x1, float m3x1, float m4x1,
			float m1x2, float m2x2, float m3x2, float m4x2,
			float m1x3, float m2x3, float m3x3, float m4x3,
			float m1x4, float m2x4, float m3x4, float m4x4)
		{
			Clear();
			Add(m1x1, m2x1, m3x1, m4x1,
				m1x2, m2x2, m3x2, m4x2,
				m1x3, m2x3, m3x3, m4x3,
				m1x4, m2x4, m3x4, m4x4);
		}
	}
}