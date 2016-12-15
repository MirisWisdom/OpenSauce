/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;

using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA
{
	[Serializable]
	[XmlType(AnonymousType = true)]
	[XmlRoot(ElementName = "COLLADA", Namespace = "http://www.collada.org/2005/11/COLLADASchema", IsNullable = false)]
	public class ColladaFile : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _version;
		ColladaObjectElement<Core.ColladaAsset> _asset;
		ColladaObjectElement<Core.ColladaLibraryAnimations> _libraryAnimations;
		ColladaObjectElement<Core.ColladaLibraryAnimationClips> _libraryAnimationClips;
		ColladaObjectElement<Core.ColladaLibraryCameras> _libraryCameras;
		ColladaObjectElement<Core.ColladaLibraryControllers> _libraryControllers;
		ColladaObjectElement<Fx.ColladaLibraryEffects> _libraryEffects;
		ColladaObjectElement<Core.ColladaLibraryGeometries> _libraryGeometries;
		ColladaObjectElement<Fx.ColladaLibraryImages> _libraryImages;
		ColladaObjectElement<Core.ColladaLibraryLights> _libraryLights;
		ColladaObjectElement<Fx.ColladaLibraryMaterials> _libraryMaterials;
		ColladaObjectElement<Core.ColladaLibraryNodes> _libraryNodes;
		ColladaObjectElement<Core.ColladaLibraryVisualScenes> _libraryVisualScenes;
		ColladaObjectElement<Core.ColladaScene> _scene;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("version")]
		public string Version
		{ get { return _version.Value; } set { _version.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public Core.ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("library_animations")]
		public Core.ColladaLibraryAnimations LibraryAnimations
		{ get { return _libraryAnimations.Value; } set { _libraryAnimations.Value = value; } }

		[XmlElement("library_animation_clips")]
		public Core.ColladaLibraryAnimationClips LibraryAnimationClips
		{ get { return _libraryAnimationClips.Value; } set { _libraryAnimationClips.Value = value; } }

		[XmlElement("library_cameras")]
		public Core.ColladaLibraryCameras LibraryCameras
		{ get { return _libraryCameras.Value; } set { _libraryCameras.Value = value; } }

		[XmlElement("library_controllers")]
		public Core.ColladaLibraryControllers LibraryControllers
		{ get { return _libraryControllers.Value; } set { _libraryControllers.Value = value; } }

		[XmlElement("library_effects")]
		public Fx.ColladaLibraryEffects LibraryEffects
		{ get { return _libraryEffects.Value; } set { _libraryEffects.Value = value; } }

		[XmlElement("library_geometries")]
		public Core.ColladaLibraryGeometries LibraryGeometries
		{ get { return _libraryGeometries.Value; } set { _libraryGeometries.Value = value; } }

		[XmlElement("library_images")]
		public Fx.ColladaLibraryImages LibraryImages
		{ get { return _libraryImages.Value; } set { _libraryImages.Value = value; } }

		[XmlElement("library_lights")]
		public Core.ColladaLibraryLights LibraryLights
		{ get { return _libraryLights.Value; } set { _libraryLights.Value = value; } }

		[XmlElement("library_materials")]
		public Fx.ColladaLibraryMaterials LibraryMaterials
		{ get { return _libraryMaterials.Value; } set { _libraryMaterials.Value = value; } }

		[XmlElement("library_nodes")]
		public Core.ColladaLibraryNodes LibraryNodes
		{ get { return _libraryNodes.Value; } set { _libraryNodes.Value = value; } }

		[XmlElement("library_visual_scenes")]
		public Core.ColladaLibraryVisualScenes LibraryVisualScenes
		{ get { return _libraryVisualScenes.Value; } set { _libraryVisualScenes.Value = value; } }

		[XmlElement("scene")]
		public Core.ColladaScene Scene
		{ get { return _scene.Value; } set { _scene.Value = value; } }
		
		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaFile() : base(Enums.ColladaElementType.Core_COLLADA)
		{
			Fields.Add(_version = new ColladaObjectAttribute<string>(""));
			Fields.Add(_asset = new ColladaObjectElement<Core.ColladaAsset>());
			Fields.Add(_libraryAnimations = new ColladaObjectElement<Core.ColladaLibraryAnimations>());
			Fields.Add(_libraryAnimationClips = new ColladaObjectElement<Core.ColladaLibraryAnimationClips>());
			Fields.Add(_libraryCameras = new ColladaObjectElement<Core.ColladaLibraryCameras>());
			Fields.Add(_libraryControllers = new ColladaObjectElement<Core.ColladaLibraryControllers>());
			Fields.Add(_libraryEffects = new ColladaObjectElement<Fx.ColladaLibraryEffects>());
			Fields.Add(_libraryGeometries = new ColladaObjectElement<Core.ColladaLibraryGeometries>());
			Fields.Add(_libraryImages = new ColladaObjectElement<Fx.ColladaLibraryImages>());
			Fields.Add(_libraryLights = new ColladaObjectElement<Core.ColladaLibraryLights>());
			Fields.Add(_libraryMaterials = new ColladaObjectElement<Fx.ColladaLibraryMaterials>());
			Fields.Add(_libraryNodes = new ColladaObjectElement<Core.ColladaLibraryNodes>());
			Fields.Add(_libraryVisualScenes = new ColladaObjectElement<Core.ColladaLibraryVisualScenes>());
			Fields.Add(_scene = new ColladaObjectElement<Core.ColladaScene>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _version));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _asset));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _version));
		}

		public void Validate()
		{
			ValidateElement(BlamLib.Render.COLLADA.Enums.ColladaElementType.All);
		}
	};
}