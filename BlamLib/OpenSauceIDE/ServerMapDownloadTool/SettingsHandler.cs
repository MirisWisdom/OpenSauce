using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using OpenSauceIDE.Settings;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	static public class SettingsHandler
	{
		public class ServerMapDownloadToolSettings : OpenSauceIDE.Settings.SettingsObjectBase
		{
			protected new int mClassVersion = 0x00000001;

			public class MapCompressorSettings : SettingsObjectBase
			{
				protected new int mClassVersion = 0x00000001;

				public static readonly DictionarySizeWrapper[] DictionarySizeMap =
				{
					new DictionarySizeWrapper(65536, "64KB"),
					new DictionarySizeWrapper(1048576, "1MB"),
					new DictionarySizeWrapper(1048576 * 2, "2MB"),
					new DictionarySizeWrapper(1048576 * 3, "3MB"),
					new DictionarySizeWrapper(1048576 * 4, "4MB"),
					new DictionarySizeWrapper(1048576 * 6, "6MB"),
					new DictionarySizeWrapper(1048576 * 8, "8MB"),
					new DictionarySizeWrapper(1048576 * 12, "12MB"),
					new DictionarySizeWrapper(1048576 * 16, "16MB"),
					new DictionarySizeWrapper(1048576 * 24, "24MB"),
					new DictionarySizeWrapper(1048576 * 32, "32MB"),
					new DictionarySizeWrapper(1048576 * 48, "48MB"),
					new DictionarySizeWrapper(1048576 * 64, "64MB"),
				};

				public string MapsFolder
				{
					get { return GetValue<string>("MapsFolder"); }
					set { SetValue<string>("MapsFolder", value); }
				}

				public string PartsFolder
				{
					get { return GetValue<string>("PartsFolder"); }
					set { SetValue<string>("PartsFolder", value); }
				}

				public string DefinitionsFolder
				{
					get { return GetValue<string>("DefinitionsFolder"); }
					set { SetValue<string>("DefinitionsFolder", value); }
				}

				public bool EncryptArchive
				{
					get { return GetValue<bool>("EncryptArchive"); }
					set { SetValue<bool>("EncryptArchive", value); }
				}

				private string mServerPassword = "";
				public string ServerPassword
				{
					get { return mServerPassword; }
					set
					{
						mServerPassword = value;
						OnPropertyChanged("ServerPassword");
					}
				}

				public MapCompressorSettings(PropertyNode parentNode)
					: base(parentNode)
				{
					if (!parentNode.Has("MapsFolder"))
					{
						MapsFolder = "";
					}
					if (!parentNode.Has("PartsFolder"))
					{
						PartsFolder = "";
					}
					if (!parentNode.Has("DefinitionsFolder"))
					{
						DefinitionsFolder = "";
					}
					if (!parentNode.Has("EncryptArchive"))
					{
						EncryptArchive = false;
					}

					ServerPassword = "";
				}
			};

			public MapCompressorSettings MapCompressor { get; set; }

			public ServerMapDownloadToolSettings(PropertyNode parentNode)
				: base(parentNode)
			{
				MapCompressor = (MapCompressorSettings)SettingsManager.GetSettings("OpenSauceIDE.MapServerDownloadTool.Settings.MapCompressor", typeof(MapCompressorSettings));
			}
		};

		public static ServerMapDownloadToolSettings MapDownloadToolSettings;

		public static void GetSettings()
		{
			// get the settings class from the settings manager
			MapDownloadToolSettings = (ServerMapDownloadToolSettings)Settings.SettingsManager.GetSettings("OpenSauceIDE.MapServerDownloadTool.Settings", typeof(ServerMapDownloadToolSettings));
		}
	};
}
