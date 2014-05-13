using BlamLib.Render.COLLADA;
using OpenSauceIDE.Aether.AutoUI.Attributes;
using System;
using System.Collections.Generic;
using System.IO;

namespace OpenSauceIDE.Aether.Settings
{
	/// <summary>	Defines the global operating parameters for a single instance of Aether. </summary>
	public class AetherSettings : OpenSauceIDE.Settings.SettingsObjectBase, IColladaSettings
	{
		/// <summary>	Enum for defining a bitmap type. </summary>
		public enum BitmapExportFormatEnum
		{
			[AutoUI, AutoUIName("Direct3D Surface (*.dds)")]
			DDS = 0,
			[AutoUI, AutoUIName("Targa (*.tga)")]
			TGA = 1,
			[AutoUI, AutoUIName("TIFF (*.tif)")]
			TIF = 2,
			[AutoUI, AutoUIName("PNG (*.png)")]
			PNG = 3,
			[AutoUI, AutoUIName("Bitmap (*.bmp)")]
			BMP = 4,
			[AutoUI, AutoUIName("Jpeg (*.jpg)")]
			JPG = 5
		};

		/// <summary>	Enum for defining what bitmaps to extract. </summary>
		public enum BitmapExportFilterEnum
		{
			[AutoUI, AutoUIName("None")]
			None,
			[AutoUI, AutoUIName("All")]
			All,
			[AutoUI, AutoUIName("Diffuse Only")]
			DiffuseOnly
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether overwriting files is enabled. </summary>
		///
		/// <value>	true if overwrite is enabled, false if not. </value>
		[AutoUI, AutoUIName("Overwrite Enabled")]
		public bool OverwriteEnabled
		{
			get { return GetValue<bool>("OverwriteEnabled"); }
			set { SetValue<bool>("OverwriteEnabled", value); }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the bitmap export format. </summary>
		///
		/// <value>	The bitmap export format. </value>
		[AutoUI, AutoUIName("Bitmap Export Format")]
		public BitmapExportFormatEnum BitmapExportFormat
		{
			get { return GetValue<BitmapExportFormatEnum>("BitmapExportFormat"); }
			set { SetValue<BitmapExportFormatEnum>("BitmapExportFormat", value); }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the bitmap export filter. </summary>
		///
		/// <value>	The bitmap export filter. </value>
		[AutoUI, AutoUIName("Bitmap Export Filter")]
		public BitmapExportFilterEnum BitmapExportFilter
		{
			get { return GetValue<BitmapExportFilterEnum>("BitmapExportFilter"); }
			set { SetValue<BitmapExportFilterEnum>("BitmapExportFilter", value); }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the lightmaps should be exported. </summary>
		///
		/// <value>	true if export lightmaps, false if not. </value>
		[AutoUI, AutoUIName("Export Lightmaps")]
		public bool ExportLightmaps
		{
			get { return GetValue<bool>("ExportLightmaps"); }
			set { SetValue<bool>("ExportLightmaps", value); }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the bitmap export extension. </summary>
		///
		/// <value>	The bitmap export extension. </value>
		public string BitmapExportExtension
		{
			get
			{
				switch (BitmapExportFormat)
				{
					case BitmapExportFormatEnum.DDS: return "dds";
					case BitmapExportFormatEnum.TGA: return "tga";
					case BitmapExportFormatEnum.PNG: return "png";
					case BitmapExportFormatEnum.BMP: return "bmp";
					case BitmapExportFormatEnum.JPG: return "jpg";
					case BitmapExportFormatEnum.TIF:
					default:
						return "tif";
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the pathname of the installation directory. </summary>
		///
		/// <value>	The pathname of the installation directory. </value>
		[AutoUI, AutoUIName("Installation Directory"), AutoUIControlType(typeof(AutoUI.Controls.AutoUIPathString))]
		public string InstallationDirectory
		{
			get
			{
				return GetValue<string>("InstallationDirectory");
			}
			set
			{
				SetValue<string>("InstallationDirectory", value);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the full pathname of the tags path. </summary>
		///
		/// <value>	The full pathname of the tags path. </value>
		public string TagsPath
		{
			get { return Path.Combine(InstallationDirectory, "tags"); }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the full pathname of the data path. </summary>
		///
		/// <value>	The full pathname of the data path. </value>
		public string DataPath
		{
			get { return Path.Combine(InstallationDirectory, "data"); }
		}

		#region IColladaSettings Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a value indicating whether COLLADA esports can overwrite files. </summary>
		///
		/// <value>	true if overwrite is enabled, false if not. </value>
		public bool Overwrite
		{
			get
			{
				return OverwriteEnabled;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the pathname of the root directory COLLADA exports will use. </summary>
		///
		/// <value>	The pathname of the root directory. </value>
		public string RootDirectory
		{
			get
			{
				return DataPath;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the bitmap extension COLLADA exports will use. </summary>
		///
		/// <value>	The bitmap extension. </value>
		public string BitmapExtension
		{
			get
			{
				return BitmapExportExtension;
			}
		}
		#endregion

		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="parentNode">	The parent property node. </param>
		public AetherSettings(OpenSauceIDE.Settings.PropertyNode parentNode)
			: base(parentNode)
		{
			if (!parentNode.Has("OverwriteEnabled"))
			{
				parentNode.Put("OverwriteEnabled", false);
			}
			if (!parentNode.Has("BitmapExportFormat"))
			{
				parentNode.Put("BitmapExportFormat", BitmapExportFormatEnum.TIF);
			}
			if (!parentNode.Has("BitmapExportFilter"))
			{
				parentNode.Put("BitmapExportFilter", BitmapExportFilterEnum.DiffuseOnly);
			}
			if (!parentNode.Has("ExportLightmaps"))
			{
				parentNode.Put("ExportLightmaps", false);
			}
			if (!parentNode.Has("InstallationDirectory"))
			{
				parentNode.Put("InstallationDirectory", "");
			}
		}
		#endregion Constructor
	}

	/// <summary>	Handler for getting the Aether settings for each engine type. </summary>
	public class AetherSettingsHandler
	{
		#region Singleton
		private static AetherSettingsHandler mInstance = new AetherSettingsHandler();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the handler instance. </summary>
		///
		/// <value>	The handler instance. </value>
		public static AetherSettingsHandler Instance
		{
			get
			{
				return mInstance;
			}
		}

		/// <summary>	Constructor that prevents a default instance of this class from being created. </summary>
		private AetherSettingsHandler()
		{ }
		#endregion Singleton

		#region Fields
		private static readonly Dictionary<BlamLib.BlamVersion, string> kPermittedVersions = new Dictionary<BlamLib.BlamVersion, string> {
			{ BlamLib.BlamVersion.Halo1_CE, "Halo1_CE"},
			//{ BlamLib.BlamVersion.Halo1_Mac, "Halo1_Mac"},
			//{ BlamLib.BlamVersion.Halo1_PC, "Halo1_PC"},
			//{ BlamLib.BlamVersion.Halo1_PCX, "Halo1_PCX"},
			//{ BlamLib.BlamVersion.Halo1_Xbox, "Halo1_Xbox"},
			//{ BlamLib.BlamVersion.Halo1_XboxX, "Halo1_XboxX"},
			//{ BlamLib.BlamVersion.Halo2_Alpha, "Halo2_Alpha"},
			//{ BlamLib.BlamVersion.Halo2_Epsilon, "Halo2_Epsilon"},
			//{ BlamLib.BlamVersion.Halo2_PC, "Halo2_PC"},
			//{ BlamLib.BlamVersion.Halo2_Xbox, "Halo2_Xbox"},
			//{ BlamLib.BlamVersion.Halo3_Beta, "Halo3_Beta"},
			//{ BlamLib.BlamVersion.Halo3_Epsilon, "Halo3_Epsilon"},
			//{ BlamLib.BlamVersion.Halo3_Xbox, "Halo3_Xbox"},
			//{ BlamLib.BlamVersion.Halo4_Xbox, "Halo4_Xbox"},
			//{ BlamLib.BlamVersion.HaloOdst_Xbox, "HaloOdst_Xbox"},
			//{ BlamLib.BlamVersion.HaloReach_Beta, "HaloReach_Beta"},
			//{ BlamLib.BlamVersion.HaloReach_Xbox, "HaloReach_Xbox"},
			//{ BlamLib.BlamVersion.Stubbs_Mac, "Stubbs_Mac"},
			//{ BlamLib.BlamVersion.Stubbs_PC, "Stubbs_PC"},
			//{ BlamLib.BlamVersion.Stubbs_Xbox, "Stubbs_Xbox"},
		};

		private Dictionary<BlamLib.BlamVersion, AetherSettings> mSettings = new Dictionary<BlamLib.BlamVersion, AetherSettings>();
		#endregion Fields

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the settings object for the specified engine type. </summary>
		///
		/// <exception cref="Exception">	Thrown if an unsupported engine is supplied. </exception>
		///
		/// <param name="blamVersion">	The blam engine version. </param>
		///
		/// <returns>	The settings object. </returns>
		public AetherSettings GetSettings(BlamLib.BlamVersion blamVersion)
		{
			if (!kPermittedVersions.ContainsKey(blamVersion))
			{
				throw new Exception("Requested Aether settings for an unsupported version");
			}

			if (!mSettings.ContainsKey(blamVersion))
			{
				AetherSettings aetherSettings = (AetherSettings)OpenSauceIDE.Settings.SettingsManager.GetSettings("OpenSauceIDE.Aether." + kPermittedVersions[blamVersion], typeof(AetherSettings));

				mSettings.Add(blamVersion, aetherSettings);
			}

			return mSettings[blamVersion];
		}
	}
}
