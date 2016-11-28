/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaInitFrom : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<uint> _mip;
		ColladaObjectAttribute<uint> _slice;
		ColladaObjectAttribute<Enums.ColladaFXSurfaceFaceEnum> _face;
		ColladaObjectValue<string> _text;
		#endregion

		#region Attributes
		[XmlAttribute("mip"), DefaultValue(0)]
		public uint Mip
		{ get { return _mip.Value; } set { _mip.Value = value; } }

		[XmlAttribute("slice"), DefaultValue(0)]
		public uint Slice
		{ get { return _slice.Value; } set { _slice.Value = value; } }

		[XmlAttribute("face"), DefaultValue(Enums.ColladaFXSurfaceFaceEnum.POSITIVE_X)]
		public Enums.ColladaFXSurfaceFaceEnum Face
		{ get { return _face.Value; } set { _face.Value = value; } }
		#endregion

		[XmlTextAttribute(), ColladaURI]
		public string Text
		{ get { return _text.Value; } set { _text.Value = value; } }
		
		public ColladaInitFrom() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_mip = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_slice = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_face = new ColladaObjectAttribute<Enums.ColladaFXSurfaceFaceEnum>(Enums.ColladaFXSurfaceFaceEnum.POSITIVE_X));
			Fields.Add(_text = new ColladaObjectValue<string>(""));
		}
	}
}