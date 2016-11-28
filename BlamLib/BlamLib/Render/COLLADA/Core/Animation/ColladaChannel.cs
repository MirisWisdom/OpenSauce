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

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaChannel : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _source;
		ColladaObjectAttribute<string> _target;
		#endregion

		#region Attributes
		[XmlAttribute("source"), ColladaURI]
		public string Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlAttribute("target"), ColladaURI]
		public string Target
		{ get { return _target.Value; } set { _target.Value = value; } }
		#endregion

		public ColladaChannel() : base(Enums.ColladaElementType.Core_Channel)
		{
			Fields.Add(_source = new ColladaObjectAttribute<string>(""));
			Fields.Add(_target = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _target));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _target));
		}
	}
}