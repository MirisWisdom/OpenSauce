/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;

namespace BlamLib.Managers
{
	/// <summary>
	/// Static set of functions and enums for interfacing with
	/// the different game's we support
	/// </summary>
	public static class GameManager
	{
		/// <summary>
		/// Handles user overriding of default file paths.
		/// </summary>
		/// <remarks>Works below the platform level, so a user can't override a main Game's folder path</remarks>
		public sealed class Overrides
		{
			#region Tags
			private string tags = null;
			/// <summary>
			/// Path that supersedes the normal tags folder
			/// </summary>
			/// <remarks>null if not overrided</remarks>
			public string Tags	{ get { return tags; } }
			#endregion

			#region Data
			private string data;
			/// <summary>
			/// Path that supersedes the normal data folder
			/// </summary>
			/// <remarks>null if not overrided</remarks>
			public string Data	{ get { return data; } }
			#endregion

			#region Maps
			private string maps;
			/// <summary>
			/// Path that supersedes the normal maps folder
			/// </summary>
			/// <remarks>null if not overrided</remarks>
			public string Maps	{ get { return maps; } }
			#endregion

			#region Plugins
			private string plugins;
			/// <summary>
			/// Path that supersedes the normal maps folder
			/// </summary>
			/// <remarks>null if not overrided</remarks>
			public string Plugins	{ get { return plugins; } }
			#endregion

			/// <summary>
			/// Define a Platform's path override
			/// </summary>
			/// <param name="path_tags">Path to supersede the default, null if no override</param>
			/// <param name="path_data">Path to supersede the default, null if no override</param>
			/// <param name="path_maps">Path to supersede the default, null if no override</param>
			/// <param name="path_plugins">Path to supersede the default, null if no override</param>
			public Overrides(string path_tags, string path_data, string path_maps, string path_plugins)
			{
				tags = path_tags;
				data = path_data;
				maps = path_maps;
				plugins = path_plugins;
			}

			/// <summary>
			/// Get the path that overrides <paramref name="pf"/>
			/// </summary>
			/// <param name="pf">The platform's folder. Must only have one folder flag set.</param>
			/// <param name="override_path">The overridden path value</param>
			/// <returns>True if a override exists</returns>
			public bool GetOverride(PlatformFolder pf, out string override_path)
			{
				override_path = null;

				switch(pf)
				{
					case PlatformFolder.Tags: override_path = tags; break;
					case PlatformFolder.Data: override_path = data; break;
					case PlatformFolder.Maps: override_path = maps; break;
					case PlatformFolder.Plugins: override_path = plugins; break;
					default: return false;
				}
				if (override_path == null) return false; // the override path wasn't defined in the first place
				return true;
			}

			/// <summary>
			/// Get the relative path to a special folder for a platform
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <returns>Relative path to the folder</returns>
			public string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder)											{ return GameManager.GetRelativePath(ns, platform, folder, this); }

			/// <summary>
			/// Create or get the path to a special folder for a platform
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <returns>Path to the folder</returns>
			public string CreatePlatformFolder(Namespace ns, Platform platform, PlatformFolder folder)										{ return GameManager.CreatePlatformFolder(ns, platform, folder, this); }

			/// <summary>
			/// Create or get the path to a special folder for a platform
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
			/// <returns>Path to the folder</returns>
			public string CreatePlatformFolder(Namespace ns, Platform platform, PlatformFolder folder, bool create)							{ return GameManager.CreatePlatformFolder(ns, platform, folder, this, create); }

			/// <summary>
			/// Get the relative path to a file
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="file">File name</param>
			/// <returns>Relative path to the file</returns>
			public string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, string file)								{ return GameManager.GetRelativePath(ns, platform, folder, file, this); }

			/// <summary>
			/// Create or get the path to a file
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="file">File name</param>
			/// <returns>Path to the file</returns>
			public string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file)							{ return GameManager.CreatePlatformFile(ns, platform, folder, file, this); }

			/// <summary>
			/// Create or get the path to a file
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="file">File name</param>
			/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
			/// <returns>Path to the file</returns>
			public string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, bool create)				{ return GameManager.CreatePlatformFile(ns, platform, folder, file, this, create); }

			/// <summary>
			/// Get the relative path to a file
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="file">File name</param>
			/// <param name="ext">File extension</param>
			/// <returns>Relative path to the file</returns>
			public string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext)					{ return GameManager.GetRelativePath(ns, platform, folder, file, ext, this); }

			/// <summary>
			/// Create or get the path to a file
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="file">File name</param>
			/// <param name="ext">File extension</param>
			/// <returns>Path to the file</returns>
			public string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext)				{ return GameManager.CreatePlatformFile(ns, platform, folder, file, ext, this); }

			/// <summary>
			/// Create or get the path to a file
			/// </summary>
			/// <param name="ns">Game namespace</param>
			/// <param name="platform">Platform folder</param>
			/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
			/// <param name="file">File name</param>
			/// <param name="ext">File extension</param>
			/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
			/// <returns>Path to the file</returns>
			public string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext, bool create)	{ return GameManager.CreatePlatformFile(ns, platform, folder, file, ext, this, create); }
		};


		/// <summary>
		/// Game namespaces
		/// </summary>
		public enum Namespace
		{
			Unknown,

			Halo1,
			Stubbs,
			Halo2,
			Halo3,
			HaloOdst,
			HaloReach,
			Halo4,
		};

		/// <summary>
		/// Game namespace platforms
		/// </summary>
		public enum Platform
		{
			Unknown,

			Xbox,
			PC,
			Mac,
		};

		#region Conversion
		/// <summary>
		/// Translate a Namespace and a Platform into a specific BlamVersion enumeration
		/// </summary>
		/// <param name="n"></param>
		/// <param name="p"></param>
		/// <returns>The BlamVersion enumeration based on the parameters</returns>
		public static BlamVersion ToBlamVersion(Namespace n, Platform p)
		{
			BlamVersion g = BlamVersion.Unknown;
			switch(p)
			{
				case Platform.Xbox:	g |= BlamVersion.Xbox; break;
				case Platform.PC:	g |= BlamVersion.PC; break;
				case Platform.Mac:	g |= BlamVersion.Mac; break;
			}

			switch(n)
			{
				case Namespace.Halo1:		g |= BlamVersion.Halo1; break;
				case Namespace.Stubbs:		g |= BlamVersion.Stubbs; break;
				case Namespace.Halo2:		g |= BlamVersion.Halo2; break;
				case Namespace.Halo3:		g |= BlamVersion.Halo3; break;
				case Namespace.HaloOdst:	g |= BlamVersion.HaloOdst; break;
				case Namespace.HaloReach:	g |= BlamVersion.HaloReach; break;
				case Namespace.Halo4:		g |= BlamVersion.Halo4; break;
			}

			return g;
		}
		/// <summary>
		/// Translate a BlamVersion enumeration into a Namespace and Platform enumerations
		/// </summary>
		/// <param name="g">Enumeration to translate</param>
		/// <param name="n"><paramref name="g"/>'s Namespace</param>
		/// <param name="p"><paramref name="g"/>'s Platform</param>
		public static void FromBlamVersion(BlamVersion g, out Namespace n, out Platform p)
		{
			n = Namespace.Unknown;
			p = Platform.Unknown;

			if ((g & BlamVersion.Xbox) != 0)	p = Platform.Xbox;
			else if ((g & BlamVersion.PC) != 0)	p = Platform.PC;
			else if ((g & BlamVersion.Mac) != 0)p = Platform.Mac;

			if ((g & BlamVersion.Halo1) != 0)			n = Namespace.Halo1;
			else if ((g & BlamVersion.Stubbs) != 0)		n = Namespace.Stubbs;
			else if ((g & BlamVersion.Halo2) != 0)		n = Namespace.Halo2;
			else if ((g & BlamVersion.Halo3) != 0)		n = Namespace.Halo3;
			else if ((g & BlamVersion.HaloOdst) != 0)	n = Namespace.HaloOdst;
			else if ((g & BlamVersion.HaloReach) != 0)	n = Namespace.HaloReach;
			else if ((g & BlamVersion.Halo4) != 0)		n = Namespace.Halo4;
		}
		#endregion

		#region PlatformFolder
		/// <summary>
		/// Platform sub folders
		/// </summary>
		/// <remarks>
		/// Use underscore character to enable sub-folders (ie, 
		/// <see cref="PlatformFolder.Definitions_GameState"/>)
		/// </remarks>
		[Flags]
		public enum PlatformFolder : byte
		{
			/// <summary>
			/// Platform's "Tags\" folder
			/// </summary>
			Tags = 2,
			/// <summary>
			/// Platform's "Data\" folder
			/// </summary>
			Data = 4,
			/// <summary>
			/// Platform's "Maps\" folder
			/// </summary>
			Maps = 8,
			/// <summary>
			/// Platform's "Definitions\" folder
			/// </summary>
			/// <remarks>Can't be overrided</remarks>
			Definitions = 16,
			/// <summary>
			/// Platform's "Definitions\TDF\" folder
			/// </summary>
			/// <remarks>Can't be overrided</remarks>
			Definitions_TDF = 32,
			/// <summary>
			/// Platform's "Definitions\Gamestate\" folder
			/// </summary>
			/// <remarks>Can't be overrided</remarks>
			Definitions_GameState = 64,
			/// <summary>
			/// Platform's "Plugins\" folder
			/// </summary>
			Plugins = 128,

			AllFolders = Tags | Data | Maps |
						 Definitions | Definitions_TDF | Definitions_GameState |
						 Plugins,
		};

		/// <summary>
		/// Checks to see if there are multiple folder flags
		/// </summary>
		/// <param name="folder">Platform's folder. Can have more than one folder flag set.</param>
		/// <returns>true if there are multiple folder flags</returns>
		private static bool CheckPlatformFolderFlags(PlatformFolder folder)
		{
			int f = (int)folder;
			bool _checked = false;
			for (int x = 0; x < sizeof(PlatformFolder) * 8; x++)
			{
				if ((f & x) != 0)
					if (_checked)	return true;
					else			_checked = true;
			}
			return false;
		}

		/// <summary>
		/// Gets a complete platform folder path based on a game's namespace, and its platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <returns></returns>
		public static string GetPlatformFolderPath(Namespace ns, Platform platform, PlatformFolder folder)
		{
			return string.Format("{0}{1}\\{2}\\{3}\\", Program.GamesPath, ns.ToString(), platform.ToString(), folder.ToString().Replace('_', '\\'));
		}
		#endregion

		#region GetRelativePath
		/// <summary>
		/// Get the relative path to game folder
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <returns>relative game path</returns>
		public static string GetRelativePath(Namespace ns) { return string.Format("{0}\\", ns.ToString()); }

		/// <summary>
		/// Get the relative path to a platform specific folder for a game
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <returns>relative game's platform's folder path</returns>
		public static string GetRelativePath(Namespace ns, Platform platform) { return string.Format("{0}{1}\\", GetRelativePath(ns), platform.ToString()); }

		/// <summary>
		/// Get the relative path to a special folder for a platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <returns>Relative path to the folder</returns>
		public static string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder) { return string.Format("{0}{1}\\", GetRelativePath(ns, platform), folder.ToString().Replace('_', '\\')); }

		/// <summary>
		/// Get the relative path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <returns>Relative path to the file</returns>
		public static string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, string file) { return string.Format("{0}{1}", CreatePlatformFolder(ns, platform, folder), file); }

		/// <summary>
		/// Get the relative path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="ext">File extension</param>
		/// <returns>Relative path to the file</returns>
		public static string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext) { return string.Format("{0}{1}.{2}", CreatePlatformFolder(ns, platform, folder), file, ext); }
		#endregion

		#region Create
		/// <summary>
		/// Creates or get the path to game folder
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <returns>game path</returns>
		public static string CreateGameNamespace(Namespace ns)	{ return CreateGameNamespace(ns, true); }

		/// <summary>
		/// Creates or get the path to game folder
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>game path</returns>
		public static string CreateGameNamespace(Namespace ns, bool create)
		{
			string dir_path = string.Format("{0}{1}\\", Program.GamesPath, ns.ToString());

			if (create && !Directory.Exists(dir_path)) Directory.CreateDirectory(dir_path);

			return dir_path;
		}

		/// <summary>
		/// Create or get the path to a platform specific folder for a game
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <returns>game's platform's folder path</returns>
		public static string CreatePlatform(Namespace ns, Platform platform) { return CreatePlatform(ns, platform, true); }

		/// <summary>
		/// Create or get the path to a platform specific folder for a game
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>game's platform's folder path</returns>
		public static string CreatePlatform(Namespace ns, Platform platform, bool create)
		{
			string dir_path = string.Format("{0}{1}\\", CreateGameNamespace(ns), platform.ToString());

			if (create && !Directory.Exists(dir_path)) Directory.CreateDirectory(dir_path);

			return dir_path;
		}

		/// <summary>
		/// Create or get the path to a special folder for a platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <returns>Path to the folder</returns>
		public static string CreatePlatformFolder(Namespace ns, Platform platform, PlatformFolder folder) { return CreatePlatformFolder(ns, platform, folder, true); }

		/// <summary>
		/// Create or get the path to a special folder for a platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>Path to the folder</returns>
		public static string CreatePlatformFolder(Namespace ns, Platform platform, PlatformFolder folder, bool create)
		{
			string dir_path = string.Format("{0}{1}\\", CreatePlatform(ns, platform), folder.ToString().Replace('_', '\\'));

			if (create && !Directory.Exists(dir_path)) Directory.CreateDirectory(dir_path);

			return dir_path;
		}

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file) { return CreatePlatformFile(ns, platform, folder, file, true); }

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, bool create)
		{
			string file_path = string.Format("{0}{1}", CreatePlatformFolder(ns, platform, folder), file);

			if (create && !File.Exists(file_path)) File.Create(file_path).Close();

			return file_path;
		}

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="ext">File extension</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext) { return CreatePlatformFile(ns, platform, folder, file, ext, true); }

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="ext">File extension</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext, bool create)
		{
			string file_path = string.Format("{0}{1}.{2}", CreatePlatformFolder(ns, platform, folder), file, ext);

			if (create && !File.Exists(file_path)) File.Create(file_path).Close();

			return file_path;
		}
		#endregion

		#region Checks
		/// <summary>
		/// Checks to see if the game namespace exists
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <returns>True if the folder exists</returns>
		public static bool CheckNamespace(Namespace ns)						{ return Directory.Exists(Program.BuildDocumentPath(ns + "\\")); }

		/// <summary>
		/// Checks to see if a platform has been defined in the game's folder
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <returns>True if the folder exists</returns>
		public static bool CheckPlatform(Namespace ns, Platform platform)	{ return Directory.Exists(Program.BuildDocumentPath(ns + "\\" + platform + "\\")); }

		/// <summary>
		/// Checks for what platform folders exist in the game namespace and game platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <returns>Flagged PlatformFolder of all existing folders</returns>
		public static PlatformFolder CheckPlatformFolders(Namespace ns, Platform platform)
		{
			return PlatformFolder.AllFolders; // TODO: do me
		}
		#endregion

		#region GetRelativePath (with Overrides)
		/// <summary>
		/// Get the relative path to a special folder for a platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <returns>Relative path to the folder</returns>
		public static string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, Overrides overrides)
		{
			string dir_path;
			if (overrides != null && overrides.GetOverride(folder, out dir_path))
				return dir_path;
			else
				return GetRelativePath(ns, platform, folder);
		}

		/// <summary>
		/// Get the relative path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <returns>Relative path to the file</returns>
		public static string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, string file, Overrides overrides)
		{
			return string.Format("{0}{1}", GetRelativePath(ns, platform, folder, overrides), file);
		}

		/// <summary>
		/// Get the relative path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="ext">File extension</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <returns>Relative path to the file</returns>
		public static string GetRelativePath(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext, Overrides overrides)
		{
			return string.Format("{0}{1}.{2}", GetRelativePath(ns, platform, folder, overrides), file, ext);
		}
		#endregion

		#region Create (with Overrides)
		/// <summary>
		/// Create or get the path to a special folder for a platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <returns>Path to the folder</returns>
		public static string CreatePlatformFolder(Namespace ns, Platform platform, PlatformFolder folder, Overrides overrides) { return CreatePlatformFolder(ns, platform, folder, overrides, true); }

		/// <summary>
		/// Create or get the path to a special folder for a platform
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>Path to the folder</returns>
		public static string CreatePlatformFolder(Namespace ns, Platform platform, PlatformFolder folder, Overrides overrides, bool create)
		{
			string dir_path;
			if (overrides != null && overrides.GetOverride(folder, out dir_path))
			{
				if (create && !Directory.Exists(dir_path)) Directory.CreateDirectory(dir_path);

				return dir_path;
			}
			else
				return CreatePlatformFolder(ns, platform, folder);
		}

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, Overrides overrides) { return CreatePlatformFile(ns, platform, folder, file, overrides, true); }

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, Overrides overrides, bool create)
		{
			string file_path = string.Format("{0}{1}", CreatePlatformFolder(ns, platform, folder, overrides), file);

			if (create && !File.Exists(file_path)) File.Create(file_path).Close();

			return file_path;
		}

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="ext">File extension</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext, Overrides overrides) { return CreatePlatformFile(ns, platform, folder, file, ext, overrides, true); }

		/// <summary>
		/// Create or get the path to a file
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="folder">Platform's folder. Must only have one folder flag set.</param>
		/// <param name="file">File name</param>
		/// <param name="ext">File extension</param>
		/// <param name="overrides">Optional set of overrides to use. Can be null.</param>
		/// <param name="create">True if we should create the path on disk if it doesn't exist already</param>
		/// <returns>Path to the file</returns>
		public static string CreatePlatformFile(Namespace ns, Platform platform, PlatformFolder folder, string file, string ext, Overrides overrides, bool create)
		{
			string file_path = string.Format("{0}{1}.{2}", CreatePlatformFolder(ns, platform, folder, overrides), file, ext);

			if (create && !File.Exists(file_path)) File.Create(file_path).Close();

			return file_path;
		}
		#endregion

		#region Checks (with Overrides)
		/// <summary>
		/// Checks for what overridden platform folders exist
		/// </summary>
		/// <param name="ns">Game namespace</param>
		/// <param name="platform">Platform folder</param>
		/// <param name="overrides"></param>
		/// <returns>Flagged PlatformFolder of all existing folders</returns>
		public static PlatformFolder CheckPlatformFolders(Namespace ns, Platform platform, Overrides overrides)
		{
			return PlatformFolder.AllFolders; // TODO: do me
		}
		#endregion
	}
}