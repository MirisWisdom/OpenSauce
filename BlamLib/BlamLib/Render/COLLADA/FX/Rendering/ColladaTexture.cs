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
	public partial class ColladaTexture : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _texture;
		ColladaObjectAttribute<string> _texCoord;
		ColladaObjectElement<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("texture")]
		public string Texture
		{ get { return _texture.Value; } set { _texture.Value = value; } }

		[XmlAttribute("texcoord")]
		public string TexCoord
		{ get { return _texCoord.Value; } set { _texCoord.Value = value; } }
		#endregion

		#region Children
		[XmlElement("extra")]
		public Core.ColladaExtra Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaTexture() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_texture = new ColladaObjectAttribute<string>(""));
			Fields.Add(_texCoord = new ColladaObjectAttribute<string>(""));
			Fields.Add(_extra = new ColladaObjectElement<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texture));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texture));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texCoord));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _texCoord));
		}
	}
}