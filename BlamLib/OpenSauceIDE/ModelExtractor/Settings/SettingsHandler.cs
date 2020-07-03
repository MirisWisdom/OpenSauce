/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Bitmaps;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.Settings;

namespace OpenSauceIDE.ModelExtractor.Settings
{
	/// <summary>	The model extractor settings handler. </summary>
	static public class SettingsHandler
	{
		/// <summary>	A root model extractor settings. </summary>
		public class ModelExtractorSettings
			: SettingsObjectBase
		{
			protected new int mClassVersion = 0x00000001;

			/// <summary>	The extractor path settings. </summary>
			public class ExtractorPathSettings
				: SettingsObjectBase
				, IExtractorPathSettings
			{
				protected new int mClassVersion = 0x00000001;

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets or sets the pathname of the tags folder. </summary>
				///
				/// <value>	The pathname of the tags folder. </value>
				public string TagsFolder
				{
					get { return GetValue<string>("TagsFolder"); }
					set { SetValue<string>("TagsFolder", value); }
				}

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets or sets the pathname of the data folder. </summary>
				///
				/// <value>	The pathname of the data folder. </value>
				public string DataFolder
				{
					get { return GetValue<string>("DataFolder"); }
					set { SetValue<string>("DataFolder", value); }
				}

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Constructor. </summary>
				///
				/// <param name="parentNode">	The parent node. </param>
				public ExtractorPathSettings(PropertyNode parentNode)
					: base(parentNode)
				{
					if (!parentNode.Has("TagsFolder"))
					{
						TagsFolder = "";
					}
					if (!parentNode.Has("DataFolder"))
					{
						DataFolder = "";
					}
				}
			};

			/// <summary>	The extractor collada settings. </summary>
			public class ExtractorColladaSettings
				: SettingsObjectBase
				, IExtractorColladaSettings
			{
				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets or sets a value indicating whether models should be overwritten. </summary>
				///
				/// <value>	true if overwrite, false if not. </value>
				public bool Overwrite
				{
					get { return GetValue<bool>("Overwrite"); }
					set { SetValue<bool>("Overwrite", value); }
				}

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets or sets the collada bitmap format. </summary>
				///
				/// <value>	The bitmap format. </value>
				public AssetFormat BitmapFormat
				{
					get { return GetValue<AssetFormat>("BitmapFormat"); }
					set { SetValue<AssetFormat>("BitmapFormat", value); }
				}

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Constructor. </summary>
				///
				/// <param name="parentNode">	The parent node. </param>
				public ExtractorColladaSettings(PropertyNode parentNode)
					: base(parentNode)
				{
					if (!parentNode.Has("Overwrite"))
					{
						Overwrite = false;
					}
					if (!parentNode.Has("BitmapFormat"))
					{
						BitmapFormat = AssetFormat.tga;
					}
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets or sets the extractor path settings. </summary>
			///
			/// <value>	The extractor path settings. </value>
			public ExtractorPathSettings Extractor { get; set; }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets or sets the collada settings. </summary>
			///
			/// <value>	The collada settings. </value>
			public ExtractorColladaSettings Collada { get; set; }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="parentNode">	The parent node. </param>
			public ModelExtractorSettings(PropertyNode parentNode)
				: base(parentNode)
			{
				Extractor = (ExtractorPathSettings)SettingsManager.GetSettings("OpenSauceIDE.ModelExtractor.Extractor", typeof(ExtractorPathSettings));
				Collada = (ExtractorColladaSettings)SettingsManager.GetSettings("OpenSauceIDE.ModelExtractor.Collada", typeof(ExtractorColladaSettings));
			}
		};

		/// <summary>	The model extractor settings root. </summary>
		public static ModelExtractorSettings ModelExtractor;

		/// <summary>	Gets the model extractor settings from the global settings manager. </summary>
		public static void GetSettings()
		{
			ModelExtractor = (ModelExtractorSettings)SettingsManager.GetSettings("OpenSauceIDE.ModelExtractor", typeof(ModelExtractorSettings));
		}
	};
}
