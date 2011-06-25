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
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaBindMaterial : ColladaElement
	{
		#region Fields
		ColladaObjectElementList<Core.ColladaParam> _param;
		ColladaObjectElement<Core.ColladaTechniqueCommon> _techniqueCommon;
		ColladaObjectElementList<Core.ColladaTechnique> _technique;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("param")]
		public List<Core.ColladaParam> Param
		{ get { return _param.Value; } set { _param.Value = value; } }

		[XmlElement("technique_common")]
		public Core.ColladaTechniqueCommon TechniqueCommon
		{ get { return _techniqueCommon.Value; } set { _techniqueCommon.Value = value; } }

		[XmlElement("technique")]
		public List<Core.ColladaTechnique> Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaBindMaterial() : base(Enums.ColladaElementType.Fx_BindMaterial)
		{
			Fields.Add(_param = new ColladaObjectElementList<Core.ColladaParam>());
			Fields.Add(_techniqueCommon = new ColladaObjectElement<Core.ColladaTechniqueCommon>());
			Fields.Add(_technique = new ColladaObjectElementList<Core.ColladaTechnique>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _techniqueCommon));
		}
	}
}