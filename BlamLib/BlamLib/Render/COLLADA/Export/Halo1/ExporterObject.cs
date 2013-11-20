using BlamLib.Managers;
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
	public interface Halo1ObjectDataProvider
	{
		void GetObjectSets();
	}

    public class ColladaObjectExporter : ColladaExporterHalo1
	{		
		#region Constructor
        ///-------------------------------------------------------------------------------------------------
        /// <summary>   Halo1 Object exporter class. </summary>
        /// <param name="settings">         Arguments class containing configuration variables. </param>
        /// <param name="tagIndex">         The tag index that contains the tag being exported. </param>
        ///-------------------------------------------------------------------------------------------------
        public ColladaObjectExporter(IColladaSettings settings, TagIndexBase tagIndex)
			: base(settings, tagIndex)
        {
        }
		#endregion

        /*
         * Each permutation and LOD separate
         * Instance geometry/controllers
         * Add Shaders
         * Add lights
         */

		#region Library Creation
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates the library_visual_scenes element in the collada file. The node list is added
		/// 	under a node named "frame" since that is required when creating new BSPs.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
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

			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	}
}
