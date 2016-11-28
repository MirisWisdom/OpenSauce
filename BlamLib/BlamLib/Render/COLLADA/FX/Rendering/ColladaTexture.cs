/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaTexture : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _texture;
		ColladaObjectAttribute<string> _texCoord;
		ColladaObjectElement<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("texture")]
		public string Texture
		{ get { return _texture.Value; } set { _texture.Value = value; } }

		[XmlAttribute("texcoord")]
		public string TexCoord
		{ get { return _texCoord.Value; } set { _texCoord.Value = value; } }
		#endregion

		#region Children
		[XmlElement("extra")]
		public Core.ColladaExtra Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaTexture() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_texture = new ColladaObjectAttribute<string>(""));
			Fields.Add(_texCoord = new ColladaObjectAttribute<string>(""));
			Fields.Add(_extra = new ColladaObjectElement<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texture));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texture));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texCoord));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texCoord));
		}
	}
}