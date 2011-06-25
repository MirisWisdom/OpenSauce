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
	public partial class ColladaTranslate : ColladaSIDValueArray<float>
	{
		public ColladaTranslate() : base(Enums.ColladaElementType.Core_Translate) { }
		public ColladaTranslate(float x, float y, float z)
			: this()
		{
			SetTranslate(x, y, z);
		}
		public ColladaTranslate(float x, float y, float z, float scale)
			: this()
		{
			SetTranslate(x, y, z, scale);
		}
		public ColladaTranslate(BlamLib.TagInterface.RealPoint3D translation)
			: this()
		{
			SetTranslate(translation);
		}
		public ColladaTranslate(BlamLib.TagInterface.RealPoint3D translation, float scale)
			: this()
		{
			SetTranslate(translation, scale);
		}

		public void SetTranslate(float x, float y, float z)
		{
			Clear();
			Add(x, y, z);
		}
		public void SetTranslate(float x, float y, float z, float scale)
		{
			Clear();
			Add(x * scale, y * scale, z * scale);
		}
		public void SetTranslate(BlamLib.TagInterface.RealPoint3D translation)
		{
			Clear();
			Add(translation.X, translation.Y, translation.Z);
		}
		public void SetTranslate(BlamLib.TagInterface.RealPoint3D translation, float scale)
		{
			Clear();
			Add(translation.X * scale, translation.Y * scale, translation.Z * scale);
		}
	}
}