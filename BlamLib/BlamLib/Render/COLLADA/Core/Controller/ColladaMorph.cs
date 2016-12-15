/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaMorph : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sourceAttrib;
		ColladaObjectAttribute<Enums.ColladaMorphMethodType> _method;

		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElement<ColladaTargets> _targets;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("source"), ColladaURI]
		public string SourceAttrib
		{ get { return _sourceAttrib.Value; } set { _sourceAttrib.Value = value; } }

		[XmlAttribute("method"), DefaultValue(Enums.ColladaMorphMethodType.NORMALIZED)]
		public Enums.ColladaMorphMethodType Method
		{ get { return _method.Value; } set { _method.Value = value; } }
		#endregion

		#region Children
		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("targets")]
		public ColladaTargets Targets
		{ get { return _targets.Value; } set { _targets.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaMorph() : base(Enums.ColladaElementType.Core_Morph)
		{
			Fields.Add(_sourceAttrib = new ColladaObjectAttribute<string>(""));
			Fields.Add(_method = new ColladaObjectAttribute<Enums.ColladaMorphMethodType>(Enums.ColladaMorphMethodType.NORMALIZED));
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_targets = new ColladaObjectElement<ColladaTargets>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _sourceAttrib));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _sourceAttrib));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaListMinCount<ColladaSource>(Enums.ColladaElementType.All, _source, 2));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _targets));
		}
	}
}