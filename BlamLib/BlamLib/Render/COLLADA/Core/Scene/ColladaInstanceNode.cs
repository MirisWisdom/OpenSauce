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
	public partial class ColladaInstanceNode : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sid;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _url;

		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("url"), ColladaURI]
		public string URL
		{ get { return _url.Value; } set { _url.Value = value; } }
		#endregion

		#region Children
		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaInstanceNode() : base(Enums.ColladaElementType.Core_InstanceNode)
		{
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_url = new ColladaObjectAttribute<string>(""));
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _url));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _url));
		}
	}
}