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
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaInstanceEffect : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sid;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _url;
		ColladaObjectElementList<ColladaTechniqueHint> _techniqueHint;
		ColladaObjectElementList<ColladaSetparam> _setparam;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("url")]
		public string URL
		{ get { return _url.Value; } set { _url.Value = value; } }
		#endregion

		#region Children
		[XmlElement("technique_hint")]
		public List<ColladaTechniqueHint> TechniqueHint
		{ get { return _techniqueHint.Value; } set { _techniqueHint.Value = value; } }

		[XmlElement("setparam")]
		public List<ColladaSetparam> Setparam
		{ get { return _setparam.Value; } set { _setparam.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaInstanceEffect() : base(Enums.ColladaElementType.Fx_InstanceEffect)
		{
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_url = new ColladaObjectAttribute<string>(""));
			Fields.Add(_techniqueHint = new ColladaObjectElementList<ColladaTechniqueHint>());
			Fields.Add(_setparam = new ColladaObjectElementList<ColladaSetparam>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _sid));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _sid));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _url));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _url));
		}
	}
}