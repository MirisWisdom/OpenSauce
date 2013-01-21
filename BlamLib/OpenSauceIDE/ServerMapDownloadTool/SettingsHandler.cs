using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	static public class SettingsHandler
	{
		[Serializable(), Settings.SettingsManager.SettingsVersion(Version=3)]
		public class ServerMapDownloadToolSettings
		{
			[Serializable()]
			public class MapCompressorSettings
			{
				public string LastMapsFolder { get; set; }
				public BindingList<StringWrapperClass> RecentPartsFolders { get; set; }
				public BindingList<StringWrapperClass> RecentDefinitionsFolders { get; set; }

				public MapCompressorSettings()
				{
					LastMapsFolder = "";
					RecentPartsFolders = new BindingList<StringWrapperClass>();
					RecentDefinitionsFolders = new BindingList<StringWrapperClass>();
				}
			};

			public MapCompressorSettings MapCompressor { get; set; }

			public ServerMapDownloadToolSettings()
			{
				MapCompressor = new MapCompressorSettings();
			}
		};

		public static ServerMapDownloadToolSettings MapDownloadToolSettings = new ServerMapDownloadToolSettings();

		public static void GetSettings()
		{
			// get the settings class from the settings manager
			MapDownloadToolSettings = Settings.SettingsManager.GetSettings(typeof(ServerMapDownloadToolSettings)) as ServerMapDownloadToolSettings;

			// if there is no settings object, create a new one
			if (MapDownloadToolSettings == null)
			{
				MapDownloadToolSettings = new ServerMapDownloadToolSettings();

				Settings.SettingsManager.AddSettings(MapDownloadToolSettings);
			}
		}
	};
}
