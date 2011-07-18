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
	public partial class ColladaMorph : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sourceAttrib;
		ColladaObjectAttribute<Enums.ColladaMorphMethodType> _method;

		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElement<ColladaTargets> _targets;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("source"), ColladaURI]
		public string SourceAttrib
		{ get { return _sourceAttrib.Value; } set { _sourceAttrib.Value = value; } }

		[XmlAttribute("method"), DefaultValue(Enums.ColladaMorphMethodType.NORMALIZED)]
		public Enums.ColladaMorphMethodType Method
		{ get { return _method.Value; } set { _method.Value = value; } }
		#endregion

		#region Children
		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("targets")]
		public ColladaTargets Targets
		{ get { return _targets.Value; } set { _targets.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaMorph() : base(Enums.ColladaElementType.Core_Morph)
		{
			Fields.Add(_sourceAttrib = new ColladaObjectAttribute<string>(""));
			Fields.Add(_method = new ColladaObjectAttribute<Enums.ColladaMorphMethodType>(Enums.ColladaMorphMethodType.NORMALIZED));
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_targets = new ColladaObjectElement<ColladaTargets>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _sourceAttrib));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _sourceAttrib));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaListMinCount<ColladaSource>(Enums.ColladaElementType.All, _source, 2));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _targets));
		}
	}
}