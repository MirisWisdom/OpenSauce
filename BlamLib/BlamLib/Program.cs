/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib
{
    public static partial class Program
    {
		/// <summary>Special object constants</summary>
		public static class Constants
		{
			public const int SentinalInt32 = unchecked((int)0xDEADC0DE); // or we could use 0x1337BEEF :D
			public const uint SentinalUInt32 = 0xDEADC0DE;

			public const string
				OpenStartTag = "<",
				CloseStartTag = ">",
				OpenEndTag = "</",
				CloseEndTag = ">",
				AmpersandStr = "&",
				LessThanStr = "<",
				GreaterThanStr = ">",
				ApostropheStr = "'",

				XmlAmpersand = "&amp;",
				XmlLessThan = "&lt;",
				XmlGreaterThan = "&gt;",
				XmlApostrophe = "&apos;",
				XmlDoubleQuote = "&quot;";

			public static readonly string[][] XmlTranslation = new string[][] {
				new string[] {
					AmpersandStr, XmlAmpersand
				}, new string[] {
					LessThanStr, XmlLessThan
				}, new string[] {
					GreaterThanStr, XmlGreaterThan
				}, new string[] {
					ApostropheStr, XmlApostrophe
				}, new string[] {
					"\"", XmlDoubleQuote
				}
			};
		};

		#region Core
		const string kProjectsPath = @"C:\Mount\B\Kornner\Projects\";
		public const string SourcePath = kProjectsPath + @"BlamLib\";

		/// <summary>Name of executing assembly</summary>
		public static readonly string Name = System.Windows.Forms.Application.ProductName;
		/// <summary>Version string of the executing assembly</summary>
		public static readonly string Version;

		static readonly string DocumentsFolderPath;
		static void InitializeDocumentsFolderPath(out string documents_folder_path)
		{
			if(Name.Contains("Visual Studio"))
				documents_folder_path = kProjectsPath + @"test_results\BlamLib\";
			else
			{
				string base_path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);

				documents_folder_path = string.Format(@"{0}{1}\",
					System.IO.Path.Combine(base_path, @"Kornner Studios\"), Name);
			}

			if (!System.IO.Directory.Exists(documents_folder_path))
				System.IO.Directory.CreateDirectory(documents_folder_path);
		}

		/// <summary>Build a path to the defined BlamLib documents folder</summary>
		/// <param name="path">Path or file name</param>
		/// <returns></returns>
		public static string BuildDocumentPath(string path)
		{
			return System.IO.Path.Combine(DocumentsFolderPath, path);
		}
		#endregion

		#region Settings
		public const string NewLine = "\r\n";
		/// <summary>File path of the debug file</summary>
		public static readonly string DebugFile;
		/// <summary>Path of folder where tracing files will be stored</summary>
		public static readonly string TracePath;
		/// <summary>Path of the folder where our game data will be stored</summary>
		public static readonly string GamesPath;
		#endregion

		/// <summary>Get the manager object for an engine</summary>
		/// <param name="engine"></param>
		/// <returns></returns>
		public static Managers.BlamDefinition GetManager(BlamVersion engine)
		{
			if ((engine & BlamVersion.Halo1) != 0)			return Halo1.Manager;
#if !NO_HALO2
			else if ((engine & BlamVersion.Halo2) != 0)		return Halo2.Manager;
#endif
#if !NO_HALO3
			else if ((engine & BlamVersion.Halo3) != 0)		return Halo3.Manager;
#endif
#if !NO_HALO_ODST
			else if ((engine & BlamVersion.HaloOdst) != 0)	return HaloOdst.Manager;
#endif
#if !NO_HALO_REACH
			else if ((engine & BlamVersion.HaloReach) != 0)	return HaloReach.Manager;
#endif
#if !NO_HALO4
			else if ((engine & BlamVersion.Halo4) != 0)		return Halo4.Manager;
#endif
			else if ((engine & BlamVersion.Stubbs) != 0)	return Stubbs.Manager;
			else throw new Debug.Exceptions.UnreachableException(engine);
		}

		#region CacheBuilder
		/// <summary>
		/// Get a cache builder that is running in memory
		/// </summary>
		/// <param name="builder_id">Handle for the cache builder object</param>
		/// <returns></returns>
		public static Blam.CacheFile GetCacheBuilder(Blam.DatumIndex builder_id)
		{
			BlamVersion engine = Managers.BlamDefinition.CacheBuilderDatumToEngine(builder_id);

			return GetManager(engine).GetCacheFile(builder_id);
		}

		/// <summary>
		/// Dispose a cache builder from memory
		/// </summary>
		/// <param name="builder_id">Handle for the cache builder object</param>
		public static void CloseCacheBuilder(Blam.DatumIndex builder_id)
		{
			BlamVersion engine = Managers.BlamDefinition.CacheBuilderDatumToEngine(builder_id);

			GetManager(engine).CloseCacheBuilder(builder_id);
		}
		#endregion

		#region CacheFile
		/// <summary>
		/// Get a cache file that is loaded in memory
		/// </summary>
		/// <param name="cache_id">Handle for the cache file object</param>
		/// <returns></returns>
		public static Blam.CacheFile GetCacheFile(Blam.DatumIndex cache_id)
		{
			BlamVersion engine = Managers.BlamDefinition.CacheDatumToEngine(cache_id);

			return GetManager(engine).GetCacheFile(cache_id);
		}

		/// <summary>
		/// Dispose a cache file from memory
		/// </summary>
		/// <param name="cache_id">Handle for the cache file object</param>
		public static void CloseCacheFile(Blam.DatumIndex cache_id)
		{
			BlamVersion engine = Managers.BlamDefinition.CacheDatumToEngine(cache_id);

			GetManager(engine).CloseCacheFile(cache_id);
		}
		#endregion

		#region TagIndex
		/// <summary>
		/// Get a tag index that is loaded in memory
		/// </summary>
		/// <param name="index_id">Handle for the tag index object</param>
		/// <returns></returns>
		public static Managers.ITagIndex GetTagIndex(Blam.DatumIndex index_id)
		{
			BlamVersion engine = Managers.BlamDefinition.TagIndexDatumToEngine(index_id);

			return GetManager(engine).GetTagIndex(index_id);
		}

		/// <summary>
		/// Dispose a tag index from memory
		/// </summary>
		/// <param name="index_id">Handle for the tag index object</param>
		public static void CloseTagIndex(Blam.DatumIndex index_id)
		{
			BlamVersion engine = Managers.BlamDefinition.CacheDatumToEngine(index_id);

			GetManager(engine).CloseTagIndex(index_id);
		}
		#endregion


		#region Initialize\Dispose
		static Program()
		{
			var assembly = System.Reflection.Assembly.GetEntryAssembly();
			if (assembly == null)
				assembly = System.Reflection.Assembly.GetCallingAssembly();
			Version = assembly.GetName().Version.ToString(4);

			InitializeDocumentsFolderPath(out DocumentsFolderPath);
			DebugFile = BuildDocumentPath("debug.log");
			TracePath = BuildDocumentPath(@"Logs\");
			GamesPath = BuildDocumentPath(@"Games\");

			Initialize();
		}
		static void Application_ApplicationExit(object sender, EventArgs e) { Close(); }

		static bool isInitialized = false;
		public static void Initialize()
		{
			if (!isInitialized)
			{
				//if(System.Diagnostics.Debugger.IsAttached)
				/*{	// HACK: to make the LoaderLock message appear as soon as possible.
					// This is only needed during VS debugging. Won't get it during non-debug runtime.
					// SlimDX doesn't have these issues.
					var t = typeof(Microsoft.DirectX.UnsafeNativeMethods);
					t = typeof(Microsoft.DirectX.Direct3D.Device);
					t = null;
				}*/

				Debug.Exceptions.Initialize();

				System.Windows.Forms.Application.ApplicationExit += 
					new EventHandler(Application_ApplicationExit);

				Halo1.Initialize();
#if !NO_HALO2
				Halo2.Initialize();
#endif
#if !NO_HALO3
				Halo3.Initialize();
#endif
#if !NO_HALO_ODST
				HaloOdst.Initialize();
#endif
#if !NO_HALO_REACH
				HaloReach.Initialize();
#endif
#if !NO_HALO4
				Halo4.Initialize();
#endif
				Stubbs.Initialize();

				TagInterface.DefinitionStatePool.PostProcess();

				isInitialized = true;
			}
		}

		/// <summary>Close the resources used by this library</summary>
		public static void Close()
		{
			if (isInitialized)
			{
				Stubbs.Close();
#if !NO_HALO4
				Halo4.Close();
#endif
#if !NO_HALO_REACH
				HaloReach.Close();
#endif
#if !NO_HALO_ODST
				HaloOdst.Close();
#endif
#if !NO_HALO3
				Halo3.Close();
#endif
#if !NO_HALO2
				Halo2.Close();
#endif
				Halo1.Close();
				Debug.Exceptions.Dispose();
				Debug.Trace.Close();
				Debug.LogFile.CloseLog();

				isInitialized = false;
			}
		}
		#endregion
	};
}