using BlamLib.Managers;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.Aether.AutoUI.Attributes;
using OpenSauceIDE.Aether.SceneObject;
using OpenSauceIDE.Aether.Settings;
using System;
using System.IO;

namespace OpenSauceIDE.Aether.Project.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Scene object for Halo1 scenario structure bsp. </summary>
	///-------------------------------------------------------------------------------------------------
	[SceneObject(SceneObjectClassEnum.Object, "STRUCTURE_BSP"), AutoUINameFormatted("Structure BSP: {0}", "ObjectName")]
	public class SceneObjectStructureBSP : SceneObjectBase, ISceneObjectExtractable
	{
		#region Fields
		private StructureBSPData mBSPData = null;
		private TagIndexBase mTagIndex = null;
		private BlamLib.Managers.TagManager mTagManager = null;
		#endregion Fields

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="parent">	 	The parent. </param>
		/// <param name="bspData">   	Information describing the bsp. </param>
		/// <param name="tagIndex">  	Tag index containing the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		///-------------------------------------------------------------------------------------------------
		public SceneObjectStructureBSP(SceneObjectBase parent
			, StructureBSPData bspData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			mBSPData = bspData;
			mTagIndex = tagIndex;
			mTagManager = tagManager;

			EnableExtraction = true;
			RenderMesh = true;
			Portals = true;
			FogPlanes = true;
		}
		#endregion Constructor

		#region Extraction
		#region Fields
		private bool mEnableExtraction;
		#endregion Fields

		#region Properties
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets or sets a value indicating whether extraction. </summary>
		/// <value>	true if extraction is enabled, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		[AutoUI, AutoUIName("Enabled")]
		public bool EnableExtraction
		{
			get
			{
				return mEnableExtraction;
			}
			set
			{
				mEnableExtraction = value;
				OnPropertyChanged("EnableExtraction");
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the render mesh should be extracted.
		/// </summary>
		/// <value>	true if the render mesh should be extracted, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		[AutoUI, AutoUIName("Render Mesh")]
		public bool RenderMesh
		{
			get
			{
				return mBSPData.IncludeRenderMesh;
			}
			set
			{
				mBSPData.IncludeRenderMesh = value;
				OnPropertyChanged("RenderMesh");
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets or sets a value indicating whether the portals should be extracted. </summary>
		/// <value>	true if portals should be extracted, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		[AutoUI, AutoUIName("Portals")]
		public bool Portals
		{
			get
			{
				return mBSPData.IncludePortals;
			}
			set
			{
				mBSPData.IncludePortals = value;
				OnPropertyChanged("Portals");
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the fog planes should be extracted.
		/// </summary>
		/// <value>	true if fog planes should be extracted, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		[AutoUI, AutoUIName("Fog Planes")]
		public bool FogPlanes
		{
			get
			{
				return mBSPData.IncludeFogPlanes;
			}
			set
			{
				mBSPData.IncludeFogPlanes = value;
				OnPropertyChanged("FogPlanes");
			}
		}
		#endregion Properties

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Extracts this object. </summary>
		///-------------------------------------------------------------------------------------------------
		public void Extract()
		{
			if (!EnableExtraction)
			{
				return;
			}

			string outputBSPPath = Path.Combine(Aether.Instance.CurrentProject.Settings.DataPath, mTagManager.Name);

			StructureBSPShaderData bspShaderData = new StructureBSPShaderData();
			bspShaderData.CollectData(mTagIndex, mTagManager);

			var extractor = new ColladaBSPExporter(Aether.Instance.CurrentProject.Settings,
				mTagIndex,
				mTagManager);

			extractor.AddDataProvider(mBSPData);
			extractor.AddDataProvider(bspShaderData);
			if (extractor.BuildColladaInstance())
			{
				extractor.SaveDAE(outputBSPPath + ".dae");
			}
			// TODO: error here
		}
		#endregion Extraction

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the name of the object. </summary>
		/// <value>	The name of the object. </value>
		///-------------------------------------------------------------------------------------------------
		public override string ObjectName
		{
			get { return Path.GetFileNameWithoutExtension(mBSPData.TagPath); }
		}
	}
}
