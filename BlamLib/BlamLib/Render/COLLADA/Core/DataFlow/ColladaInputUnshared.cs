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
	public partial class ColladaInputUnshared : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<Enums.ColladaInputSharedSemantic> _semantic;
		ColladaObjectAttribute<string> _source;
		#endregion

		#region Attributes
		[XmlAttribute("semantic")]
		public Enums.ColladaInputSharedSemantic Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }

		[XmlAttribute("source"), ColladaURI]
		public string Source
		{ get { return _source.Value; } set { _source.Value = value; } }
		#endregion

		public ColladaInputUnshared() : base(Enums.ColladaElementType.Core_InputUnshared)
		{
			Fields.Add(_semantic = new ColladaObjectAttribute<Enums.ColladaInputSharedSemantic>(Enums.ColladaInputSharedSemantic.BINORMAL));
			Fields.Add(_source = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _source));
		}
	}
}