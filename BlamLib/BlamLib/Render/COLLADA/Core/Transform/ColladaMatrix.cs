/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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