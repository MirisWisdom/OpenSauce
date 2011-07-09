/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaNode : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _id;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _sid;
		ColladaObjectAttribute<Enums.ColladaNodeType> _type;
		ColladaObjectAttribute<string> _layer;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaElement> _transformationElements;
		ColladaObjectElementList<ColladaInstanceCamera> _instanceCamera;
		ColladaObjectElementList<ColladaInstanceController> _instanceController;
		ColladaObjectElementList<ColladaInstanceGeometry> _instanceGeometry;
		ColladaObjectElementList<ColladaInstanceLight> _instanceLight;
		ColladaObjectElementList<ColladaInstanceNode> _instanceNode;
		ColladaObjectElementList<ColladaNode> _node;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		[XmlIgnore]
		public new static readonly string ElementIDFormat = "node-{0}";

		#region Attributes
		[XmlAttribute("id"), DefaultValue("")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }

		[XmlAttribute("type"), DefaultValue(Enums.ColladaNodeType.NODE)]
		public Enums.ColladaNodeType Type
		{ get { return _type.Value; } set { _type.Value = value; } }

		[XmlAttribute("layer"), DefaultValue("")]
		public string Layer
		{ get { return _layer.Value; } set { _layer.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement(ElementName = "lookat", Type = typeof(ColladaLookat)),
		XmlElement(ElementName = "matrix", Type = typeof(ColladaMatrix)),
		XmlElement(ElementName = "rotate", Type = typeof(ColladaRotate)),
		XmlElement(ElementName = "scale", Type = typeof(ColladaScale)),
		XmlElement(ElementName = "skew", Type = typeof(ColladaSkew)),
		XmlElement(ElementName = "translate", Type = typeof(ColladaTranslate))]
		public List<ColladaElement> TransformationElements
		{ get { return _transformationElements.Value; } set { _transformationElements.Value = value; } }

		[XmlElement("instance_camera")]
		public List<ColladaInstanceCamera> InstanceCamera
		{ get { return _instanceCamera.Value; } set { _instanceCamera.Value = value; } }

		[XmlElement("instance_controller")]
		public List<ColladaInstanceController> InstanceController
		{ get { return _instanceController.Value; } set { _instanceController.Value = value; } }

		[XmlElement("instance_geometry")]
		public List<ColladaInstanceGeometry> InstanceGeometry
		{ get { return _instanceGeometry.Value; } set { _instanceGeometry.Value = value; } }

		[XmlElement("instance_light")]
		public List<ColladaInstanceLight> InstanceLight
		{ get { return _instanceLight.Value; } set { _instanceLight.Value = value; } }

		[XmlElement("instance_node")]
		public List<ColladaInstanceNode> InstanceNode
		{ get { return _instanceNode.Value; } set { _instanceNode.Value = value; } }
		
		[XmlElement("node")]
		public List<ColladaNode> Node
		{ get { return _node.Value; } set { _node.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaNode() : base(Enums.ColladaElementType.Core_Node)
		{
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_type = new ColladaObjectAttribute<Enums.ColladaNodeType>(Enums.ColladaNodeType.NODE));
			Fields.Add(_layer = new ColladaObjectAttribute<string>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_transformationElements = new ColladaObjectElementList<ColladaElement>());
			Fields.Add(_instanceCamera = new ColladaObjectElementList<ColladaInstanceCamera>());
			Fields.Add(_instanceController = new ColladaObjectElementList<ColladaInstanceController>());
			Fields.Add(_instanceGeometry = new ColladaObjectElementList<ColladaInstanceGeometry>());
			Fields.Add(_instanceLight = new ColladaObjectElementList<ColladaInstanceLight>());
			Fields.Add(_instanceNode = new ColladaObjectElementList<ColladaInstanceNode>());
			Fields.Add(_node = new ColladaObjectElementList<ColladaNode>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());
		}

		private void AddToElementList<T>(T element, ColladaObjectElementList<T> list)
			where T : new()
		{
			if (element == null) return;

			if (list.Value == null)
				list.Value = new List<T>();
			list.Value.Add(element);
		}
		private void AddRangeToElementList<T>(List<T> elements, ColladaObjectElementList<T> list)
			where T : new()
		{
			if (elements == null) return;

			if (list.Value == null)
				list.Value = new List<T>();
			list.Value.AddRange(elements);
		}

		public void Add(ColladaElement transform)			{ AddToElementList<ColladaElement>(transform, _transformationElements); }
		public void Add(ColladaInstanceCamera instance)		{ AddToElementList<ColladaInstanceCamera>(instance, _instanceCamera); }
		public void Add(ColladaInstanceController instance)	{ AddToElementList<ColladaInstanceController>(instance, _instanceController); }
		public void Add(ColladaInstanceGeometry instance)	{ AddToElementList<ColladaInstanceGeometry>(instance, _instanceGeometry); }
		public void Add(ColladaInstanceLight instance)		{ AddToElementList<ColladaInstanceLight>(instance, _instanceLight); }
		public void Add(ColladaInstanceNode instance)		{ AddToElementList<ColladaInstanceNode>(instance, _instanceNode); }
		public void Add(ColladaNode node)					{ AddToElementList<ColladaNode>(node, _node); }

		public void AddRange(List<ColladaElement> transforms)			{ AddRangeToElementList<ColladaElement>(transforms, _transformationElements); }
		public void AddRange(List<ColladaInstanceCamera> instances)		{ AddRangeToElementList<ColladaInstanceCamera>(instances, _instanceCamera); }
		public void AddRange(List<ColladaInstanceController> instances)	{ AddRangeToElementList<ColladaInstanceController>(instances, _instanceController); }
		public void AddRange(List<ColladaInstanceGeometry> instances)	{ AddRangeToElementList<ColladaInstanceGeometry>(instances, _instanceGeometry); }
		public void AddRange(List<ColladaInstanceLight> instances)		{ AddRangeToElementList<ColladaInstanceLight>(instances, _instanceLight); }
		public void AddRange(List<ColladaInstanceNode> instances)		{ AddRangeToElementList<ColladaInstanceNode>(instances, _instanceNode); }
		public void AddRange(List<ColladaNode> nodes)					{ AddRangeToElementList<ColladaNode>(nodes, _node); }
	};
}