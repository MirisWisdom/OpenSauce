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
	public partial class ColladaTechniqueHint : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _platform;
		ColladaObjectAttribute<string> _ref;
		ColladaObjectAttribute<string> _profile;
		#endregion

		#region Attributes
		[XmlAttribute("platform"), DefaultValue("")]
		public string Platform
		{ get { return _platform.Value; } set { _platform.Value = value; } }

		[XmlAttribute("ref")]
		public string Ref
		{ get { return _ref.Value; } set { _ref.Value = value; } }

		[XmlAttribute("profile"), DefaultValue("")]
		public string Profile
		{ get { return _profile.Value; } set { _profile.Value = value; } }
		#endregion

		public ColladaTechniqueHint() : base(Enums.ColladaElementType.Fx_TechniqueHint)
		{
			Fields.Add(_platform = new ColladaObjectAttribute<string>(""));
			Fields.Add(_ref = new ColladaObjectAttribute<string>(""));
			Fields.Add(_profile = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _ref));
		}
	}
}