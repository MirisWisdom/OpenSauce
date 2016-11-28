/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Core
{
	[Serializable]
	public partial class ColladaTranslate : ColladaSIDValueArray<float>
	{
		public ColladaTranslate() : base(Enums.ColladaElementType.Core_Translate) { }
		public ColladaTranslate(float x, float y, float z) : this()
		{
			SetTranslate(x, y, z);
		}
		public ColladaTranslate(float x, float y, float z, float scale) : this()
		{
			SetTranslate(x, y, z, scale);
		}

		public ColladaTranslate(LowLevel.Math.real_point3d translation) :
			this(translation.X, translation.Y, translation.Z)
		{
		}
		public ColladaTranslate(LowLevel.Math.real_point3d translation, float scale)
			: this(translation.X, translation.Y, translation.Z, scale)
		{
		}

		public ColladaTranslate(BlamLib.TagInterface.RealPoint3D translation) : 
			this(translation.X, translation.Y, translation.Z)
		{
		}
		public ColladaTranslate(BlamLib.TagInterface.RealPoint3D translation, float scale)
			: this(translation.X, translation.Y, translation.Z, scale)
		{
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

		public void SetTranslate(LowLevel.Math.real_point3d translation)
		{
			SetTranslate(translation.X, translation.Y, translation.Z);
		}
		public void SetTranslate(LowLevel.Math.real_point3d translation, float scale)
		{
			SetTranslate(translation.X, translation.Y, translation.Z, scale);
		}

		public void SetTranslate(BlamLib.TagInterface.RealPoint3D translation)
		{
			SetTranslate(translation.X, translation.Y, translation.Z);
		}
		public void SetTranslate(BlamLib.TagInterface.RealPoint3D translation, float scale)
		{
			SetTranslate(translation.X, translation.Y, translation.Z, scale);
		}
	};
}