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
	public partial class ColladaScale : ColladaSIDValueArray<float>
	{
		public ColladaScale() : base(Enums.ColladaElementType.Core_Scale) { }
		public ColladaScale(float scale_x, float scale_y, float scale_z)
			: this()
		{
			SetScale(scale_x, scale_y, scale_z);
		}
		public ColladaScale(BlamLib.TagInterface.RealVector3D scale)
			: this()
		{
			SetScale(scale);
		}

		public void SetScale(float scale_x, float scale_y, float scale_z)
		{
			Clear();
			Add(scale_x, scale_y, scale_z);
		}
		public void SetScale(BlamLib.TagInterface.RealVector3D scale)
		{
			Clear();
			Add(scale.I, scale.J, scale.K);
		}
	}
}