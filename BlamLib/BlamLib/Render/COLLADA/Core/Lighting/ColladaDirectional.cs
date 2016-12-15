/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaDirectional : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaColor> _color;
		#endregion

		#region Children
		[XmlElement("color")]
		public ColladaColor Color
		{ get { return _color.Value; } set { _color.Value = value; } }
		#endregion

		public ColladaDirectional() : base(Enums.ColladaElementType.Core_Directional)
		{
			Fields.Add(_color = new ColladaObjectElement<ColladaColor>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _color));
		}
	}
}