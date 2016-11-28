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
	public partial class ColladaLibraryVisualScenes : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaLibraryVisualScenes>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaVisualScene> _visualScene;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("libvisualscenes-{0}")]
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

		[XmlElement("visual_scene")]
		public List<ColladaVisualScene> VisualScene
		{ get { return _visualScene.Value; } set { _visualScene.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaLibraryVisualScenes() : base(Enums.ColladaElementType.Core_LibraryVisualScenes)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaLibraryVisualScenes>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_visualScene = new ColladaObjectElementList<ColladaVisualScene>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _visualScene));
			ValidationTests.Add(new ColladaListMinCount<ColladaVisualScene>(Enums.ColladaElementType.All, _visualScene, 1));
		}
	}
}