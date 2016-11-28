/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaTechniqueCommon : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaAccessor> _accessor;
		ColladaObjectElementList<Fx.ColladaInstanceMaterial> _instanceMaterial;
		ColladaObjectElement<ColladaAmbient> _ambient;
		ColladaObjectElement<ColladaDirectional> _directional;
		ColladaObjectElement<ColladaPoint> _point;
		ColladaObjectElement<ColladaSpot> _spot;
		ColladaObjectElement<ColladaOrthographic> _orthographic;
		ColladaObjectElement<ColladaPerspective> _perspective;
		#endregion

		#region Children
		#region source
		/// <summary>
		/// For use with ColladaSource only
		/// </summary>
		[XmlElement("accessor")]
		public ColladaAccessor Accessor
		{ get { return _accessor.Value; } set { _accessor.Value = value; } }
		#endregion

		#region bind_material
		/// <summary>
		/// For use with ColladaBindMaterial only
		/// </summary>
		[XmlElement("instance_material")]
		public List<Fx.ColladaInstanceMaterial> InstanceMaterial
		{ get { return _instanceMaterial.Value; } set { _instanceMaterial.Value = value; } }
		#endregion

		#region light
		/// <summary>
		/// For use with ColladaLight only
		/// </summary>
		[XmlElement("ambient")]
		public ColladaAmbient Ambient
		{ get { return _ambient.Value; } set { _ambient.Value = value; } }

		/// <summary>
		/// For use with ColladaLight only
		/// </summary>
		[XmlElement("directional")]
		public ColladaDirectional Directional
		{ get { return _directional.Value; } set { _directional.Value = value; } }

		/// <summary>
		/// For use with ColladaLight only
		/// </summary>
		[XmlElement("point")]
		public ColladaPoint Point
		{ get { return _point.Value; } set { _point.Value = value; } }

		/// <summary>
		/// For use with ColladaLight only
		/// </summary>
		[XmlElement("spot")]
		public ColladaSpot Spot
		{ get { return _spot.Value; } set { _spot.Value = value; } }
		#endregion

		#region optics
		/// <summary>
		/// For use with ColladaOptics only
		/// </summary>
		[XmlElement("orthographic")]
		public ColladaOrthographic Orthographic
		{ get { return _orthographic.Value; } set { _orthographic.Value = value; } }

		/// <summary>
		/// For use with ColladaOptics only
		/// </summary>
		[XmlElement("perspective")]
		public ColladaPerspective Perspective
		{ get { return _perspective.Value; } set { _perspective.Value = value; } }
		#endregion
		#endregion

		public ColladaTechniqueCommon() : base(Enums.ColladaElementType.Core_TechniqueCommon)
		{
			Fields.Add(_accessor = new ColladaObjectElement<ColladaAccessor>());
			Fields.Add(_instanceMaterial = new ColladaObjectElementList<Fx.ColladaInstanceMaterial>());
			Fields.Add(_ambient = new ColladaObjectElement<ColladaAmbient>());
			Fields.Add(_directional = new ColladaObjectElement<ColladaDirectional>());
			Fields.Add(_point = new ColladaObjectElement<ColladaPoint>());
			Fields.Add(_spot = new ColladaObjectElement<ColladaSpot>());
			Fields.Add(_orthographic = new ColladaObjectElement<ColladaOrthographic>());
			Fields.Add(_perspective = new ColladaObjectElement<ColladaPerspective>());

			List<ColladaObject> required_for_light = new List<ColladaObject>();
			required_for_light.Add(_ambient);
			required_for_light.Add(_directional);
			required_for_light.Add(_point);
			required_for_light.Add(_spot);

			List<ColladaObject> mutually_exclusive_for_light = new List<ColladaObject>();
			mutually_exclusive_for_light.Add(_ambient);
			mutually_exclusive_for_light.Add(_directional);
			mutually_exclusive_for_light.Add(_point);
			mutually_exclusive_for_light.Add(_spot);

			List<ColladaObject> required_for_optics = new List<ColladaObject>();
			required_for_optics.Add(_orthographic);
			required_for_optics.Add(_perspective);

			List<ColladaObject> mutually_exclusive_for_optics = new List<ColladaObject>();
			mutually_exclusive_for_optics.Add(_orthographic);
			mutually_exclusive_for_optics.Add(_perspective);
			
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_BindMaterial, _instanceMaterial));
			ValidationTests.Add(new ColladaListMinCount<Fx.ColladaInstanceMaterial>(Enums.ColladaElementType.Fx_BindMaterial, _instanceMaterial, 1));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.Core_Light, required_for_light));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.Core_Light, mutually_exclusive_for_light));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.Core_Optics, required_for_optics));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.Core_Optics, mutually_exclusive_for_optics));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Core_Source, _accessor));
		}
	}
}