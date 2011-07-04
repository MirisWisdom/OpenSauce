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
	public partial class ColladaSampler : ColladaElement
	{
		[XmlIgnore]
		public new static readonly string ElementIDFormat = "sampler-{0}";

		#region Fields
		ColladaObjectAttribute<string> _id;

		ColladaObjectElementList<ColladaInputUnshared> _input;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue("")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputUnshared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }
		#endregion

		public ColladaSampler() : base(Enums.ColladaElementType.Core_Sampler)
		{
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputUnshared>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _input));
			ValidationTests.Add(new ColladaListMinCount<ColladaInputUnshared>(Enums.ColladaElementType.All, _input, 1));
			ValidationTests.Add(new ColladaListHasValue<ColladaInputUnshared, Enums.ColladaInputSharedSemantic>(Enums.ColladaElementType.All,
				_input, "Semantic", Enums.ColladaInputSharedSemantic.INTERPOLATION));
		}
	}
}