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