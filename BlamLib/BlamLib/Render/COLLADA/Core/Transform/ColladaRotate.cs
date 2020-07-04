/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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