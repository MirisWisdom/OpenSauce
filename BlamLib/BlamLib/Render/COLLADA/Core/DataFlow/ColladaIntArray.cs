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
	public partial class ColladaIntArray : ColladaValueArray<int>
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaIntArray>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
		ColladaObjectAttribute<int> _minInclusive;
		ColladaObjectAttribute<int> _maxInclusive;
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

		[XmlAttribute("id"), DefaultValue(""), ColladaID("intarray-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("minInclusive"), DefaultValue(-2147483648)]
		public int MinInclusive
		{ get { return _minInclusive.Value; } set { _minInclusive.Value = value; } }

		[XmlAttribute("maxInclusive"), DefaultValue(2147483647)]
		public int MaxInclusive
		{ get { return _maxInclusive.Value; } set { _maxInclusive.Value = value; } }
		#endregion

		public ColladaIntArray() : base(Enums.ColladaElementType.Core_IntArray)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaIntArray>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_minInclusive = new ColladaObjectAttribute<int>(-2147483648));
			Fields.Add(_maxInclusive = new ColladaObjectAttribute<int>(2147483647));
		}
	}
}