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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSurface : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<Enums.ColladaFXSurfaceTypeEnum> _type;
		ColladaObjectElement<ColladaInitFrom> _initFrom;
		#endregion

		#region Attributes
		[XmlIgnore]
		public Enums.ColladaFXSurfaceTypeEnum Type
		{ get { return _type.Value; } set { _type.Value = value; } }

		[XmlAttribute("type")]
		public string TypeAsString
		{
			get
			{
				switch (Type)
				{
					default: return "UNTYPED";
					case COLLADA.Enums.ColladaFXSurfaceTypeEnum._1D: return "1D";
					case COLLADA.Enums.ColladaFXSurfaceTypeEnum._2D: return "2D";
					case COLLADA.Enums.ColladaFXSurfaceTypeEnum._3D: return "3D";
					case COLLADA.Enums.ColladaFXSurfaceTypeEnum.CUBE: return "CUBE";
					case COLLADA.Enums.ColladaFXSurfaceTypeEnum.DEPTH: return "DEPTH";
					case COLLADA.Enums.ColladaFXSurfaceTypeEnum.RECT: return "RECT";
				}
			}
			set { }
		}
		#endregion

		#region Children
		[XmlElement("init_from")]
		public ColladaInitFrom InitFrom
		{ get { return _initFrom.Value; } set { _initFrom.Value = value; } }
		#endregion

		public ColladaSurface() : base(Enums.ColladaElementType.Fx_Surface)
		{
			Fields.Add(_type = new ColladaObjectAttribute<Enums.ColladaFXSurfaceTypeEnum>(Enums.ColladaFXSurfaceTypeEnum.UNTYPED));
			Fields.Add(_initFrom = new ColladaObjectElement<ColladaInitFrom>());
		}
	}
}