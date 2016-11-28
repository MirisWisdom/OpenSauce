/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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