using BlamLib.Managers;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.Aether.AutoUI.Attributes;
using OpenSauceIDE.Aether.Extraction;
using OpenSauceIDE.Aether.SceneObject;
using OpenSauceIDE.Aether.Settings;
using System;
using System.IO;

namespace OpenSauceIDE.Aether.Project.Halo1
{
	[SceneObject(SceneObjectClassEnum.Object, "LIGHT"), AutoUINameFormatted("Light: {0}", "ObjectName")]
	public class SceneObjectLight : SceneObjectBase, IObjectExtractable
	{
		#region Fields
		private LightData mLightData = null;
		private TagIndexBase mTagIndex = null;
		private BlamLib.Managers.TagManager mTagManager = null;
		#endregion Fields

		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="lightData"> 	The lights data. </param>
		/// <param name="tagIndex">  	Tag index the tag is associated with. </param>
		/// <param name="tagManager">	Manager for the tag. </param>
		public SceneObjectLight(LightData lightData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			mLightData = lightData;
			mTagIndex = tagIndex;
			mTagManager = tagManager;

			EnableExtraction = true;
		}
		#endregion Constructor

		#region Extraction
		#region Fields
		private bool mEnableExtraction;
		#endregion Fields

		#region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether extraction is enabled. </summary>
		///
		/// <value>	true if extraction is enabled, false if not. </value>
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
		#endregion Properties

		/// <summary>	Extracts this object. </summary>
		public bool Extract()
		{
			var extractor = new ColladaLightExporter(Aether.Instance.CurrentProject.Settings, mTagIndex, mTagManager);
			
			extractor.ErrorOccured +=
				(sender, args) =>
				{
					Aether.Instance.Output.WriteLine(Output.OutputManager.OutputTypeEnum.Error, args.ErrorMessage);
				};

			extractor.AddDataProvider(mLightData);
			if (extractor.BuildColladaInstance())
			{
				extractor.SaveDAE(Path.Combine(Aether.Instance.CurrentProject.Settings.DataPath, mLightData.GetRelativeURL()) + ".dae");
				return true;
			}
			return false;
		}
		#endregion Extraction

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the object. </summary>
		///
		/// <value>	The name of the object. </value>
		public override string ObjectName
		{
			get { return Path.GetFileNameWithoutExtension(mLightData.TagPath); }
		}
	}
}
