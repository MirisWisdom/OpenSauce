/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaCommonColorOrTextureType : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<Enums.ColladaFXOpaqueEnum> _opaque;
		ColladaObjectElement<Core.ColladaColor> _color;
		ColladaObjectElement<ColladaParam> _param;
		ColladaObjectElement<ColladaTexture> _texture;
		#endregion

		#region Attributes
		[XmlAttribute("opaque"), DefaultValue(Enums.ColladaFXOpaqueEnum.A_ONE)]
		public Enums.ColladaFXOpaqueEnum Opaque
		{ get { return _opaque.Value; } set { _opaque.Value = value; } }
		#endregion

		#region Children
		[XmlElement("color")]
		public Core.ColladaColor Color
		{ get { return _color.Value; } set { _color.Value = value; } }

		[XmlElement("param")]
		public ColladaParam Param
		{ get { return _param.Value; } set { _param.Value = value; } }

		[XmlElement("texture")]
		public ColladaTexture Texture
		{ get { return _texture.Value; } set { _texture.Value = value; } }
		#endregion

		public ColladaCommonColorOrTextureType() : base(Enums.ColladaElementType.Fx_CommonColorOrTextureType)
		{
			Fields.Add(_opaque = new ColladaObjectAttribute<Enums.ColladaFXOpaqueEnum>(Enums.ColladaFXOpaqueEnum.A_ONE));
			Fields.Add(_color = new ColladaObjectElement<Core.ColladaColor>());
			Fields.Add(_param = new ColladaObjectElement<ColladaParam>());
			Fields.Add(_texture = new ColladaObjectElement<ColladaTexture>());

			List<ColladaObject> field_list = new List<ColladaObject>();
			field_list.Add(_color);
			field_list.Add(_param);
			field_list.Add(_texture);

			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, field_list));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, field_list));
		}
	}
}