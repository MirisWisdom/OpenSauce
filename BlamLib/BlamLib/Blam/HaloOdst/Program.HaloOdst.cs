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
﻿
namespace BlamLib
{
	partial class Program
	{
#if !NO_HALO_ODST
		/// <summary>
		/// Halo ODST global wide settings
		/// </summary>
		public static class HaloOdst
		{
			#region Manager
			static Blam.HaloOdst.GameDefinition manager = new Blam.HaloOdst.GameDefinition();
			/// <summary>
			/// Halo ODST's specific manager instance
			/// </summary>
			public static Blam.HaloOdst.GameDefinition Manager { get { return manager; } }
			#endregion

			/// <summary>
			/// Initialize the resources used by the Halo ODST systems
			/// </summary>
			public static void Initialize()
			{
				manager.Read(Managers.GameManager.GetRelativePath(BlamLib.Managers.GameManager.Namespace.HaloOdst), "HaloOdst.xml");
			}

			/// <summary>
			/// Close the resources used by the Halo ODST systems
			/// </summary>
			public static void Close()
			{
				CloseXbox();
				manager.Close();
			}

			#region Xbox
			/// <summary>
			/// Path to the file 'mainmenu.map'
			/// </summary>
			static string XboxMainmenuPath = string.Empty;

			public static BlamLib.Blam.HaloOdst.CacheFile XboxMainmenu;

			/// <summary>
			/// Path to the file 'shared.map'
			/// </summary>
			static string XboxSharedPath = string.Empty;

			public static BlamLib.Blam.HaloOdst.CacheFile XboxShared;

			/// <summary>
			/// Path to the file 'campaign.map'
			/// </summary>
			static string XboxCampaignPath = string.Empty;

			public static BlamLib.Blam.HaloOdst.CacheFile XboxCampaign;

			/// <summary>
			/// Loads the Halo 3 files needed to fully run this library
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
					di = manager.OpenResourceCacheFile(BlamVersion.HaloOdst_Xbox, XboxMainmenuPath = paths[0]);
					XboxMainmenu = manager.GetCacheFile(di) as BlamLib.Blam.HaloOdst.CacheFile;
					XboxMainmenu.ReadResourceCache();
				}

				if (paths.Length >= 2 && System.IO.File.Exists(paths[1]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloOdst_Xbox, XboxSharedPath = paths[1]);
					XboxShared = manager.GetCacheFile(di) as BlamLib.Blam.HaloOdst.CacheFile;
					XboxShared.ReadResourceCache();
				}

				if (paths.Length >= 3 && System.IO.File.Exists(paths[2]))
				{
					di = manager.OpenResourceCacheFile(BlamVersion.HaloOdst_Xbox, XboxCampaignPath = paths[2]);
					XboxCampaign = manager.GetCacheFile(di) as BlamLib.Blam.HaloOdst.CacheFile;
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
			/// Returns a CacheFile object based on a ResourcePtr
			/// </summary>
			/// <param name="ver">HaloOdst engine version</param>
			/// <param name="cache_name"></param>
			/// <returns>The CacheFile <paramref name="ptr"/> references</returns>
			public static Blam.HaloOdst.CacheFile FromLocation(BlamVersion ver, string cache_name)
			{
				if (cache_name == null || cache_name == string.Empty) return null;

				Blam.CacheType t = BlamLib.Blam.CacheType.None;
				if (cache_name.Contains("mainmenu"))		t = BlamLib.Blam.CacheType.MainMenu;
				else if (cache_name.Contains("shared"))		t = BlamLib.Blam.CacheType.Shared;
				else if (cache_name.Contains("campaign"))	t = BlamLib.Blam.CacheType.SharedCampaign;
				else										throw new Debug.Exceptions.UnreachableException(cache_name);

				if (ver == BlamVersion.HaloOdst_Xbox)
				{
					switch (t)
					{
						case BlamLib.Blam.CacheType.MainMenu:		return XboxMainmenu;
						case BlamLib.Blam.CacheType.Shared:			return XboxShared;
						case BlamLib.Blam.CacheType.SharedCampaign:	return XboxCampaign;
					}
				}

				return null;
			}

			/// <summary>
			/// Returns a CacheFile object based on a ResourcePtr
			/// </summary>
			/// <remarks>
			/// If the resource is internal then <paramref name="is_internal"/> gets set to true
			/// and null is returned. If null and <paramref name="is_internal"/> is not set,
			/// the CacheFile is either not loaded or the location was invalid.
			/// </remarks>
			/// <param name="ver">HaloOdst engine version</param>
			/// <param name="cache_name"></param>
			/// <param name="is_internal">bool reference to set if the ptr is internal</param>
			/// <returns>The CacheFile <paramref name="ptr"/> references</returns>
			public static Blam.HaloOdst.CacheFile FromLocation(BlamVersion ver, string cache_name, out bool is_internal)
			{
				is_internal = false;

				Blam.HaloOdst.CacheFile c = FromLocation(ver, cache_name);

				is_internal = c == null;
				return c;
			}
		};
#endif
	};
}