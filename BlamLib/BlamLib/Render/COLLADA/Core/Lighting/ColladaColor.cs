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
using BlamLib.TagInterface;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaColor : ColladaValueArray<float>
	{
		public ColladaColor() : base(Enums.ColladaElementType.Core_Color) { }
		public ColladaColor(float red, float green, float blue)
			: this()
		{
			SetColor(red, green, blue);
		}
		public ColladaColor(float red, float green, float blue, float alpha)
			: this()
		{
			SetColor(red, green, blue, alpha);
		}
		public ColladaColor(RealColor color, bool use_alpha)
			: this()
		{
			SetColor(color, use_alpha);
		}
		public ColladaColor(RealColor color, float alpha_override)
			: this()
		{
			SetColor(color, alpha_override);
		}

		public void SetColor(float red, float green, float blue)
		{
			Clear();
			Add(red, green, blue);
		}
		public void SetColor(float red, float green, float blue, float alpha)
		{
			Clear();
			Add(red, green, blue, alpha);
		}
		public void SetColor(RealColor color, bool use_alpha)
		{
			Clear();
			if (use_alpha)
				Add(color.R, color.G, color.B, color.A);
			else
				Add(color.R, color.G, color.B);
		}
		public void SetColor(RealColor color, float alpha_override)
		{
			Clear();
			Add(color.R, color.G, color.B, alpha_override);
		}
	}
}