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