/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;

namespace BlamLib
{
    public static partial class Program
    {
		/// <summary>
		/// Special object constants
		/// </summary>
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

		/// <summary>
		/// Name of this assembly
		/// </summary>
		public static readonly string Name = System.Windows.Forms.Application.ProductName;
		/// <summary>
		/// Version string of this assembly
		/// </summary>
		public static readonly string Version = System.Windows.Forms.Application.ProductVersion;
		/// <summary>
		/// Root namespace for the codebase
		/// </summary>
		public const string Namespace = "BlamLib";
		/// <summary>
		/// Startup path of this assembly
		/// </summary>
		public static readonly string StartupPath = //System.Windows.Forms.Application.StartupPath + "\\";
			kProjectsPath + @"test_results\BlamLib\";
		#endregion

		#region Settings
		public const string NewLine = "\r\n";
		/// <summary>
		/// File path of the debug file
		/// </summary>
		public static readonly string DebugFile = Program.StartupPath + "debug.log";
		/// <summary>
		/// Path of folder where tracing files will be stored
		/// </summary>
		public static readonly string TracePath = Program.StartupPath + "Logs\\";
		/// <summary>
		/// Path of the folder where our game data will be stored
		/// </summary>
		public static readonly string GamesPath = Program.StartupPath + "Games\\";
		/// <summary>
		/// Should beta protection code be ran?
		/// </summary>
		public const bool BetaProtection = false;
		/// <summary>
		/// Should the bug report be made\sent?
		/// </summary>
		public static readonly bool BugReportingEnabled = false;
		/// <summary>
		/// Should we download updates automatically?
		/// </summary>
		public static readonly bool AutoUpdateEnabled = false;
		#endregion

		/// <summary>
		/// Get the manager object for an engine
		/// </summary>
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
//			StartupPath =	Path.Combine(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), 
//								"Kornner Studios"), 
//									"BlamLib") + @"\";

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
				Stubbs.Initialize();

				TagInterface.DefinitionStatePool.PostProcess();

				isInitialized = true;
			}
		}

		/// <summary>
		/// Close the resources used by this library
		/// </summary>
		public static void Close()
		{
			if (isInitialized)
			{
				Stubbs.Close();
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