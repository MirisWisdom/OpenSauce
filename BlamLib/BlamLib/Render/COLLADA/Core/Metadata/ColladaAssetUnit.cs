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
	public partial class ColladaAssetUnit : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<double> _meter;
		ColladaObjectAttribute<string> _name;
		#endregion

		#region Attributes
		[XmlAttribute("meter"), DefaultValue(1.0)]
		public double Meter
		{ get { return _meter.Value; } set { _meter.Value = value; } }

		[XmlAttribute("name"), DefaultValue("meter")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		public ColladaAssetUnit() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_meter = new ColladaObjectAttribute<double>(1.0));
			Fields.Add(_name = new ColladaObjectAttribute<string>("meter"));
		}
	}
}