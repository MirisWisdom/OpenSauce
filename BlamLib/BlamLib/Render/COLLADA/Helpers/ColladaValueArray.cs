/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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