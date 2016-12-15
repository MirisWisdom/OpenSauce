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
	public partial class ColladaGeometry : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaGeometry>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElement<ColladaMesh> _mesh;
		ColladaObjectElement<ColladaSpline> _spline;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("geometry-{0}")]
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

		[XmlElement("mesh")]
		public ColladaMesh Mesh
		{ get { return _mesh.Value; } set { _mesh.Value = value; } }

		[XmlElement("spline")]
		public ColladaSpline Spline
		{ get { return _spline.Value; } set { _spline.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaGeometry() : base(Enums.ColladaElementType.Core_Geometry)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaGeometry>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_mesh = new ColladaObjectElement<ColladaMesh>());
			Fields.Add(_spline = new ColladaObjectElement<ColladaSpline>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			List<ColladaObject> mutually_exclusive = new List<ColladaObject>();
			mutually_exclusive.Add(_mesh);
			mutually_exclusive.Add(_spline);

			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, mutually_exclusive));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, mutually_exclusive));
		}
	}
}