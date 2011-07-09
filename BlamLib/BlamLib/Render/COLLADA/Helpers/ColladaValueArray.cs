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
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA
{
	[Serializable]
	[XmlType(AnonymousType = true)]
	public partial class ColladaValueArray<T> : ColladaElement
	{
		public ColladaValueArray() : base(Enums.ColladaElementType.Undefined) { }
		public ColladaValueArray(Enums.ColladaElementType type) : base(type) { }

		[XmlIgnore]
		public List<T> Values;

		[XmlText]
		public string Text
		{
			get { return ColladaUtilities.ListToString<T>(Values); }
			set { Values = ColladaUtilities.StringToList<T>(value); }
		}

		public void Add(params T[] values)
		{
			if (Values == null)
				Values = new List<T>();

			foreach (T value in values)
				Values.Add(value);
		}
		public void Add(List<T> values)
		{
			if (Values == null)
				Values = new List<T>();

			Values.AddRange(values);
		}
		public void Clear()
		{
			if (Values != null)
				Values.Clear();
		}
	};

	public static class ColladaValueArrayExtensions
	{
		public static void Add(this ColladaValueArray<float> array, LowLevel.Math.real_point2d v)
		{
			array.Add(v.X, v.Y);
		}
		public static void Add(this ColladaValueArray<float> array, LowLevel.Math.real_point3d v)
		{
			array.Add(v.X, v.Y, v.Z);
		}

		public static void Add(this ColladaValueArray<float> array, LowLevel.Math.real_vector2d v)
		{
			array.Add(v.I, v.J);
		}
		public static void Add(this ColladaValueArray<float> array, LowLevel.Math.real_vector3d v)
		{
			array.Add(v.I, v.J, v.K);
		}
	};
}