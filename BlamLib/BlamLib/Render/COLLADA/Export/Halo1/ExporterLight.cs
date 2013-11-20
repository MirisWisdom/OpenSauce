/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlamLib.Render.COLLADA.Halo1
{
	public interface IHalo1LightDataProvider : IColladaDataProvider
	{
		LightData.LightDetails GetLight();
	}

	public class ColladaLightExporter : ColladaExporter
	{
		#region Class Members
		protected static readonly LowLevel.Math.real_vector3d RotationVectorY = new LowLevel.Math.real_vector3d(0, 1, 0);
		protected static readonly LowLevel.Math.real_vector3d RotationVectorP = new LowLevel.Math.real_vector3d(1, 0, 0);
		protected static readonly LowLevel.Math.real_vector3d RotationVectorR = new LowLevel.Math.real_vector3d(0, 0, 1);

		IHalo1LightDataProvider mLightDataProvider = null;
		protected List<Core.ColladaLight> listLight = new List<Core.ColladaLight>();
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 Scenario exporter class
		/// </summary>
		/// <param name="bsp_info">An object implementing IHalo1BSPInterface to define what meshes are to be included in the collada file</param>
		/// <param name="tag_index">The tag index that contains the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaLightExporter(IColladaSettings settings
			, Managers.TagIndexBase tagIndex
			, Managers.TagManager tagManager)
			: base(settings)
		{ }
		#endregion

		protected COLLADA.Core.ColladaSpot CreateSpotLight(TagInterface.RealColor color,
			float constantAttenuation,
			float linearAttenuation,
			float quadratictAttenuation,
			float falloffAngle,
			float falloffExponent)
		{
			Core.ColladaLight light = new Core.ColladaLight();

			var spotLight = new Core.ColladaSpot();
			spotLight.Color = new Core.ColladaColor(color, false);
			spotLight.ConstantAttenuation = new ColladaSIDValue<float>(constantAttenuation);
			spotLight.LinearAttenuation = new ColladaSIDValue<float>(linearAttenuation);
			spotLight.QuadraticAttenuation = new ColladaSIDValue<float>(quadratictAttenuation);
			spotLight.FalloffAngle = new ColladaSIDValue<float>(falloffAngle);
			spotLight.FalloffExponent = new ColladaSIDValue<float>(falloffExponent);

			return spotLight;
		}

		private void CreateLightList()
		{
			var light = new Core.ColladaLight();

			var lightDetails = mLightDataProvider.GetLight();

			// initialise the lights attributes
			light.Name = lightDetails.Name;
			light.ID = lightDetails.Name;

			light.TechniqueCommon = new Core.ColladaTechniqueCommon();
			light.TechniqueCommon.Spot = CreateSpotLight(lightDetails.Color,
				lightDetails.ConstantAttenuation,
				lightDetails.LinearAttenuation,
				lightDetails.QuadraticAttenuation,
				lightDetails.FalloffAngle,
				lightDetails.FalloffExponent);

			listLight.Add(light);
		}

		protected void CreateNodeInstanceLight(string name, int index)
		{
			// create the node and set its attributes
			Core.ColladaNode node = new Core.ColladaNode();
			node.Name = name;
			node.ID = name;
			node.Type = Enums.ColladaNodeType.NODE;

			Core.ColladaInstanceLight instanceLight = new Core.ColladaInstanceLight();
			instanceLight.URL = ColladaUtilities.BuildUri(listLight[index].ID);

			node.Add(instanceLight);

			// add the conpleted instance to the node list
			listNode.Add(node);
		}

		private void CreateNodeList()
		{
			// create light instances for all of the lights
			for (int i = 0; i < listLight.Count; i++)
			{
				CreateNodeInstanceLight(listLight[i].Name, i);
			}
		}

		#region Library Creation
		void AddLibraryLights()
		{
			COLLADAFile.LibraryLights = new Core.ColladaLibraryLights();
			COLLADAFile.LibraryLights.Light = listLight;
		}
		/// <summary>
		/// Creates the library_visual_scenes element in the collada file. The node list is added under a node named "frame" since that is
		/// required when creating new BSPs.
		/// </summary>
		void AddLibraryVisualScenes()
		{
			// add the main scene node
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = "main";

			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node = listNode;
		}
		#endregion

		protected override bool BuildColladaInstanceImpl()
		{
			COLLADAFile = new ColladaFile();

			COLLADAFile.Version = "1.4.1";
			AddAsset(
				System.Environment.UserName,
				"OpenSauceIDE:ColladaBuilder",
				"meter", 0.0254, Enums.ColladaUpAxisEnum.Z_UP);

			mLightDataProvider = GetDataProvider<IHalo1LightDataProvider>();

			CreateLightList();
			CreateNodeList();

			AddLibraryLights();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	}
}
