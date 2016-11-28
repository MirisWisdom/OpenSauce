/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using OpenSauceIDE.Settings;

namespace OpenSauceIDE.LightmapImporter.Settings
{
	/// <summary>	The lightmap importer settings handler. </summary>
	static public class SettingsHandler
	{
		/// <summary>	Lightmap importer settings. </summary>
		public class LightmapImporterSettings
			: SettingsObjectBase
			, ILightmapImporterSettings
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
			public LightmapImporterSettings(PropertyNode parentNode)
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
		}

		/// <summary>	The lightmap importer settings. </summary>
		public static LightmapImporterSettings LightmapImporter;

		/// <summary>	Gets the lightmap importer settings from the global settings manager. </summary>
		public static void GetSettings()
		{
			LightmapImporter = (LightmapImporterSettings)SettingsManager.GetSettings("OpenSauceIDE.LightmapImporter", typeof(LightmapImporterSettings));
		}
	};
}
