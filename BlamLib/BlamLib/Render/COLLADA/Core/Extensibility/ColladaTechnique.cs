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
	public partial class ColladaTechnique : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _profile;
		ColladaObjectAttribute<string> _xmlns;
		#endregion

		#region Attributes
		[XmlAttribute("profile")]
		public string Profile
		{ get { return _profile.Value; } set { _profile.Value = value; } }

		[XmlAttribute("xmlns"), DefaultValue("")]
		public string Xmlns
		{ get { return _xmlns.Value; } set { _xmlns.Value = value; } }
		#endregion

		#region Children
		[XmlAnyElement]
		public List<object> Elements;
		#endregion

		public ColladaTechnique() : base(Enums.ColladaElementType.Core_Technique)
		{
			Fields.Add(_profile = new ColladaObjectAttribute<string>(""));
			Fields.Add(_xmlns = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _profile));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _profile));
		}
	}
}