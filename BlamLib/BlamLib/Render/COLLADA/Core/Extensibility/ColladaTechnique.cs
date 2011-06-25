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

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaTechnique : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _profile;
		ColladaObjectAttribute<string> _xmlns;
		#endregion

		#region Attributes
		[XmlAttribute("profile")]
		public string Profile
		{ get { return _profile.Value; } set { _profile.Value = value; } }

		[XmlAttribute("xmlns"), DefaultValue("")]
		public string Xmlns
		{ get { return _xmlns.Value; } set { _xmlns.Value = value; } }
		#endregion

		#region Children
		[XmlAnyElement]
		public List<object> Elements;
		#endregion

		public ColladaTechnique() : base(Enums.ColladaElementType.Core_Technique)
		{
			Fields.Add(_profile = new ColladaObjectAttribute<string>(""));
			Fields.Add(_xmlns = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _profile));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _profile));
		}

		//public override void ValidateElement(Enums.ColladaElementType parent_type)
		//{
		//    #region Validate Self
		//    if ((Profile == null) || (Profile.Length == 0))
		//        throw new ColladaValidationException(
		//            String.Format(ColladaValidationException.MissingAttribute, "profile"));
		//    #endregion
		//}
	}
}