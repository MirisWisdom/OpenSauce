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
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaBindVertexInput : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _semantic;
		ColladaObjectAttribute<string> _inputSemantic;
		ColladaObjectAttribute<uint> _inputSet;
		#endregion

		#region Attributes
		[XmlAttribute("semantic")]
		public string Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }

		[XmlAttribute("input_semantic")]
		public string InputSemantic
		{ get { return _inputSemantic.Value; } set { _inputSemantic.Value = value; } }

		[XmlAttribute("input_set"), DefaultValue(0)]
		public uint InputSet
		{ get { return _inputSet.Value; } set { _inputSet.Value = value; } }
		#endregion

		public ColladaBindVertexInput() : base(Enums.ColladaElementType.Fx_BindVertexInput)
		{
			Fields.Add(_semantic = new ColladaObjectAttribute<string>(""));
			Fields.Add(_inputSemantic = new ColladaObjectAttribute<string>(""));
			Fields.Add(_inputSet = new ColladaObjectAttribute<uint>(0));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _semantic));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _semantic));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _inputSemantic));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _inputSemantic));
		}
	}
}