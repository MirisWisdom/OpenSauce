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
	public partial class ColladaFloatArray : ColladaValueArray<float>
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaFloatArray>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
		ColladaObjectAttribute<short> _digits;
		ColladaObjectAttribute<short> _magnitude;
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

		[XmlAttribute("id"), DefaultValue(""), ColladaID("floatarray-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("digits"), DefaultValue(6)]
		public short Digits
		{ get { return _digits.Value; } set { _digits.Value = value; } }

		[XmlAttribute("magnitude"), DefaultValue(38)]
		public short Magnitude
		{ get { return _magnitude.Value; } set { _magnitude.Value = value; } }
		#endregion

		public ColladaFloatArray() : base(Enums.ColladaElementType.Core_FloatArray)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaFloatArray>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_digits = new ColladaObjectAttribute<short>(6));
			Fields.Add(_magnitude = new ColladaObjectAttribute<short>(38));
		}
	}
}