/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿
namespace BlamLib
{
	partial class Program
	{
#if !NO_HALO_REACH
		/// <summary>
		/// Halo Reach global wide settings
		/// </summary>
		public static class HaloReach
		{
			#region Manager
			static Blam.HaloReach.GameDefinition manager = new Blam.HaloReach.GameDefinition();
			/// <summary>
			/// Halo Reach's specific manager instance
			/// </summary>
			public static Blam.HaloReach.GameDefinition Manager { get { return manager; } }
			#endregion

			/// <summary>
			/// Initialize the resources used by the Halo Reach systems
			/// </summary>
			public static void Initialize()
			{
				manager.Read(Managers.GameManager.GetRelativePath(BlamLib.Managers.GameManager.Namespace.HaloReach), "HaloReach.xml");
			}

			/// <summary>
			/// Close the resources used by the Halo Reach systems
			/// </summary>
			public static void Close()
			{
				CloseBeta();
				CloseXbox();
				manager.Close();
			}

			#region Beta
			/// <summary>
			/// Path to the file 'mainmenu.map'
			/// </summary>
			static string BetaMainmenuPath = string.Empty;

			public static BlamLib.Blam.HaloReach.CacheFile BetaMainmenu;

			/// <summary>
			/// Path to the file 'shared.map'
			/// </summary>
			static string BetaSharedPath = string.Empty;

			public static BlamLib.Blam.HaloReach.CacheFile BetaShared;

			/// <summary>
			/// Loads the Halo Reach files needed to fully run this library
			/// </summary>
			/// <remarks>
			/// Path[0] = Mainmenu
			/// Path[1] = Shared
			/// </remarks>
			/// <param name="paths"></param>
			public static void LoadBeta(params string[] paths)
			{
				Blam.DatumIndex di;

				if (paths.Length >= 1 && System.IO.File.Exists(paths[0]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloReach_Beta, BetaMainmenuPath = paths[0]);
					BetaMainmenu = manager.GetCacheFile(di) as BlamLib.Blam.HaloReach.CacheFile;
					BetaMainmenu.EngineVersion = BlamVersion.HaloReach_Beta;
					BetaMainmenu.ReadResourceCache();
				}

				if (paths.Length >= 2 && System.IO.File.Exists(paths[1]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloReach_Beta, BetaSharedPath = paths[1]);
					BetaShared = manager.GetCacheFile(di) as BlamLib.Blam.HaloReach.CacheFile;
					BetaShared.EngineVersion = BlamVersion.HaloReach_Beta;
					BetaShared.ReadResourceCache();
				}
			}

			private static void CloseBeta()
			{
				if (BetaMainmenu != null)	manager.CloseCacheFile(BetaMainmenu.CacheId);
				if (BetaShared != null)		manager.CloseCacheFile(BetaShared.CacheId);
			}
			#endregion

			#region Xbox
			/// <summary>
			/// Path to the file 'mainmenu.map'
			/// </summary>
			static string XboxMainmenuPath = string.Empty;

			public static BlamLib.Blam.HaloReach.CacheFile XboxMainmenu;

			/// <summary>
			/// Path to the file 'shared.map'
			/// </summary>
			static string XboxSharedPath = string.Empty;

			public static BlamLib.Blam.HaloReach.CacheFile XboxShared;

			/// <summary>
			/// Path to the file 'campaign.map'
			/// </summary>
			static string XboxCampaignPath = string.Empty;

			public static BlamLib.Blam.HaloReach.CacheFile XboxCampaign;

			/// <summary>
			/// Loads the Halo Reach files needed to fully run this library
			/// </summary>
			/// <remarks>
			/// Path[0] = Mainmenu
			/// Path[1] = Shared
			/// Path[2] = Single Player Shared
			/// </remarks>
			/// <param name="paths"></param>
			public static void LoadXbox(params string[] paths)
			{
				Blam.DatumIndex di;

				if (paths.Length >= 1 && System.IO.File.Exists(paths[0]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloReach_Xbox, XboxMainmenuPath = paths[0]);
					XboxMainmenu = manager.GetCacheFile(di) as BlamLib.Blam.HaloReach.CacheFile;
					XboxMainmenu.ReadResourceCache();
				}

				if (paths.Length >= 2 && System.IO.File.Exists(paths[1]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloReach_Xbox, XboxSharedPath = paths[1]);
					XboxShared = manager.GetCacheFile(di) as BlamLib.Blam.HaloReach.CacheFile;
					XboxShared.ReadResourceCache();
				}

				if (paths.Length >= 3 && System.IO.File.Exists(paths[2]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloReach_Xbox, XboxCampaignPath = paths[2]);
					XboxCampaign = manager.GetCacheFile(di) as BlamLib.Blam.HaloReach.CacheFile;
					XboxCampaign.ReadResourceCache();
				}
			}

			private static void CloseXbox()
			{
				if (XboxMainmenu != null)	manager.CloseCacheFile(XboxMainmenu.CacheId);
				if (XboxShared != null)		manager.CloseCacheFile(XboxShared.CacheId);
				if (XboxCampaign != null)	manager.CloseCacheFile(XboxCampaign.CacheId);
			}
			#endregion

			/// <summary>
			/// Returns a CacheFile object based on a cache name
			/// </summary>
			/// <param name="ver">HaloReach engine version</param>
			/// <param name="cache_name">Blam based cache name</param>
			/// <returns>The CacheFile associated with <paramref name="cache_name"/></returns>
			public static Blam.HaloReach.CacheFile FromLocation(BlamVersion ver, string cache_name)
			{
				if (string.IsNullOrEmpty(cache_name)) return null;

				Blam.CacheType t = BlamLib.Blam.CacheType.None;
				if (cache_name.Contains("mainmenu"))		t = BlamLib.Blam.CacheType.MainMenu;
				else if (cache_name.Contains("shared"))		t = BlamLib.Blam.CacheType.Shared;
				else if (cache_name.Contains("campaign"))	t = BlamLib.Blam.CacheType.SharedCampaign;
				else										throw new Debug.Exceptions.UnreachableException(cache_name);

				if (ver == BlamVersion.HaloReach_Xbox)
				{
					switch (t)
					{
						case BlamLib.Blam.CacheType.MainMenu:		return XboxMainmenu;
						case BlamLib.Blam.CacheType.Shared:			return XboxShared;
						case BlamLib.Blam.CacheType.SharedCampaign:	return XboxCampaign;
					}
				}
				else if (ver == BlamVersion.HaloReach_Beta)
				{
					switch (t)
					{
						case BlamLib.Blam.CacheType.MainMenu:		return BetaMainmenu;
						case BlamLib.Blam.CacheType.Shared:			return BetaShared;
						case BlamLib.Blam.CacheType.SharedCampaign: throw new Debug.Exceptions.UnreachableException(t);
					}
				}

				return null;
			}

			/// <summary>
			/// Returns a CacheFile object based on a cache name
			/// </summary>
			/// <remarks>
			/// If the <paramref name="cache_name"/> is null or empty then <paramref name="is_internal"/> 
			/// gets set to true and null is returned. If null and <paramref name="is_internal"/> is not set, 
			/// the CacheFile is either not loaded or the location was invalid.
			/// </remarks>
			/// <param name="ver">HaloReach engine version</param>
			/// <param name="cache_name">Blam based cache name</param>
			/// <param name="is_internal">bool reference to set if the reference is internal</param>
			/// <returns>The CacheFile associated with <paramref name="cache_name"/></returns>
			public static Blam.HaloReach.CacheFile FromLocation(BlamVersion ver, string cache_name, out bool is_internal)
			{
				is_internal = false;

				Blam.HaloReach.CacheFile c = FromLocation(ver, cache_name);

				is_internal = c == null;
				return c;
			}
		};
#endif
	};
}