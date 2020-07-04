/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaScene : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaInstanceVisualScene> _instanceVisualScene;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("instance_visual_scene")]
		public ColladaInstanceVisualScene InstanceVisualScene
		{ get { return _instanceVisualScene.Value; } set { _instanceVisualScene.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaScene() : base(Enums.ColladaElementType.Core_Scene)
		{
			Fields.Add(_instanceVisualScene = new ColladaObjectElement<ColladaInstanceVisualScene>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());
		}
	}
}