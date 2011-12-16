/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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