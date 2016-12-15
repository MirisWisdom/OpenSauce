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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaInstanceMaterial : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sid;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _target;
		ColladaObjectAttribute<string> _symbol;
		ColladaObjectElementList<ColladaBind> _bind;
		ColladaObjectElementList<ColladaBindVertexInput> _bindVertexInput;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("target"), ColladaURI]
		public string Target
		{ get { return _target.Value; } set { _target.Value = value; } }

		[XmlAttribute("symbol")]
		public string Symbol
		{ get { return _symbol.Value; } set { _symbol.Value = value; } }
		#endregion

		#region Children
		[XmlElement("bind")]
		public List<ColladaBind> Bind
		{ get { return _bind.Value; } set { _bind.Value = value; } }

		[XmlElement("bind_vertex_input")]
		public List<ColladaBindVertexInput> BindVertexInput
		{ get { return _bindVertexInput.Value; } set { _bindVertexInput.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaInstanceMaterial() : base(Enums.ColladaElementType.Fx_InstanceMaterial)
		{
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_target = new ColladaObjectAttribute<string>(""));
			Fields.Add(_symbol = new ColladaObjectAttribute<string>(""));
			Fields.Add(_bind = new ColladaObjectElementList<ColladaBind>());
			Fields.Add(_bindVertexInput = new ColladaObjectElementList<ColladaBindVertexInput>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _target));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _target));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _symbol));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _symbol));
		}
	}
}