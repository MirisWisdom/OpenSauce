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
	public partial class ColladaSource : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaSource>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElement<ColladaIDREFArray> _idrefArray;
		ColladaObjectElement<ColladaNameArray> _nameArray;
		ColladaObjectElement<ColladaBoolArray> _boolArray;
		ColladaObjectElement<ColladaFloatArray> _floatArray;
		ColladaObjectElement<ColladaIntArray> _intArray;
		ColladaObjectElement<ColladaTechniqueCommon> _techniqueCommon;
		ColladaObjectElementList<ColladaTechnique> _technique;
		#endregion

		#region Attributes
		[XmlAttribute("id"), ColladaID("source-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("IDREF_array")]
		public ColladaIDREFArray IDREFArray
		{ get { return _idrefArray.Value; } set { _idrefArray.Value = value; } }

		[XmlElement("Name_array")]
		public ColladaNameArray NameArray
		{ get { return _nameArray.Value; } set { _nameArray.Value = value; } }

		[XmlElement("bool_array")]
		public ColladaBoolArray BoolArray
		{ get { return _boolArray.Value; } set { _boolArray.Value = value; } }

		[XmlElement("float_array")]
		public ColladaFloatArray FloatArray
		{ get { return _floatArray.Value; } set { _floatArray.Value = value; } }

		[XmlElement("int_array")]
		public ColladaIntArray IntArray
		{ get { return _intArray.Value; } set { _intArray.Value = value; } }

		[XmlElement("technique_common")]
		public ColladaTechniqueCommon TechniqueCommon
		{ get { return _techniqueCommon.Value; } set { _techniqueCommon.Value = value; } }

		[XmlElement("technique")]
		public List<ColladaTechnique> Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }
		#endregion

		public ColladaSource() : base(Enums.ColladaElementType.Core_Source)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaSource>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_idrefArray = new ColladaObjectElement<ColladaIDREFArray>());
			Fields.Add(_nameArray = new ColladaObjectElement<ColladaNameArray>());
			Fields.Add(_boolArray = new ColladaObjectElement<ColladaBoolArray>());
			Fields.Add(_floatArray = new ColladaObjectElement<ColladaFloatArray>());
			Fields.Add(_intArray = new ColladaObjectElement<ColladaIntArray>());
			Fields.Add(_techniqueCommon = new ColladaObjectElement<ColladaTechniqueCommon>());
			Fields.Add(_technique = new ColladaObjectElementList<ColladaTechnique>());

			List<ColladaObject> mutually_exclusive = new List<ColladaObject>();
			mutually_exclusive.Add(_idrefArray);
			mutually_exclusive.Add(_nameArray);
			mutually_exclusive.Add(_boolArray);
			mutually_exclusive.Add(_floatArray);
			mutually_exclusive.Add(_intArray);

			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, mutually_exclusive));
		}
	}
}