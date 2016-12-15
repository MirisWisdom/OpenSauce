/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaBind : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _semantic;
		ColladaObjectAttribute<string> _target;
		#endregion

		#region Attributes
		[XmlAttribute("semantic"), DefaultValue("")]
		public string Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }

		[XmlAttribute("target"), ColladaURI]
		public string Target
		{ get { return _target.Value; } set { _target.Value = value; } }
		#endregion

		public ColladaBind() : base(Enums.ColladaElementType.Fx_Bind)
		{
			Fields.Add(_semantic = new ColladaObjectAttribute<string>(""));
			Fields.Add(_target = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _target));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _target));
		}
	}
}