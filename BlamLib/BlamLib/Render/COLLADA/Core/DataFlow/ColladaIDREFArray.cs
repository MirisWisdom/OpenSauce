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
	public partial class ColladaIDREFArray : ColladaValueArray<string>
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaIDREFArray>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
		#endregion

		#region Attributes
		[XmlAttribute("count")]
		public uint Count
		{
			get
			{
				if (Values == null)
					return 0;
				else
					return (uint)Values.Count;
			}
			set { }
		}

		[XmlAttribute("id"), DefaultValue(""), ColladaID("idrefarray-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		public ColladaIDREFArray() : base(Enums.ColladaElementType.Core_IDREFArray)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaIDREFArray>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
		}
	}
}