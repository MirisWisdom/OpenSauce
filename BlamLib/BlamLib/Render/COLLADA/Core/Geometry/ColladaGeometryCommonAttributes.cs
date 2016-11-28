/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaGeometryCommonAttributes : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<uint> _count;
		ColladaObjectAttribute<string> _material;
		#endregion

		#region Attributes
		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("count")]
		public uint Count
		{ get { return _count.Value; } set { _count.Value = value; } }

		[XmlAttribute("material"), DefaultValue("")]
		public string Material
		{ get { return _material.Value; } set { _material.Value = value; } }
		#endregion

		public ColladaGeometryCommonAttributes(Enums.ColladaElementType type) : base(type)
		{
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_count = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_material = new ColladaObjectAttribute<string>(""));
		}
	}
}