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