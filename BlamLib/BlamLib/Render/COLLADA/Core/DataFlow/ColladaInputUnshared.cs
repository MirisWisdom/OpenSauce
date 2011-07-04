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
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaInputUnshared : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<Enums.ColladaInputSharedSemantic> _semantic;
		ColladaObjectAttribute<string> _source;
		#endregion

		#region Attributes
		[XmlAttribute("semantic")]
		public Enums.ColladaInputSharedSemantic Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }

		[XmlAttribute("source")]
		public string Source
		{ get { return _source.Value; } set { _source.Value = value; } }
		#endregion

		public ColladaInputUnshared() : base(Enums.ColladaElementType.Core_InputUnshared)
		{
			Fields.Add(_semantic = new ColladaObjectAttribute<Enums.ColladaInputSharedSemantic>(Enums.ColladaInputSharedSemantic.BINORMAL));
			Fields.Add(_source = new ColladaObjectAttribute<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _source));
		}
	}
}