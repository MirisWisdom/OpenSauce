﻿/*
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
﻿using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo3
	{
		#region CacheLoadResource (Specific)
		static void CacheLoadResourceSpecificXbox(string map)
		{
			using (var handler = new CacheHandler<Blam.Halo3.CacheFile>(BlamVersion.Halo3_Xbox, map))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				//var play_index = icsi.TagIndexManager.Open(new Blam.DatumIndex(1, unchecked((short)0xE176)));
				var play_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.play);
				var playman = cache.TagIndexManager[play_index];
				var playdef = playman.TagDefinition as Blam.Halo3.Tags.cache_file_resource_layout_table_group;

				//var zone_index = icsi.TagIndexManager.Open(new Blam.DatumIndex(3, unchecked((short)0xE178)));
				var zone_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.zone);
				var zoneman = cache.TagIndexManager[zone_index];
				var zonedef = zoneman.TagDefinition as Blam.Halo3.Tags.cache_file_resource_gestalt_group;

				if (map == kMapPathChillout)
				{
					var sbsp_index = cache.TagIndexManager.Open(@"levels\dlc\chillout\chillout", BlamLib.Blam.Halo3.TagGroups.sbsp);
					var sbspman = cache.TagIndexManager[sbsp_index];
					var sbspdef = sbspman.TagDefinition as Blam.Halo3.Tags.scenario_structure_bsp_group;

					//Blam.DatumIndex rsrc_index = /*new Blam.DatumIndex(0x1136, unchecked((short)0xF299));*/ sbspdef.BspTagResourcesIndex.Value;
					//TagInterface.Definition rsrcdef = zonedef.LoadResources(rsrc_index, cache, playdef.ResourceLayoutTable);
					Blam.DatumIndex rsrc_index = sbspdef.BspRenderResourcesIndex.Value;
					TagInterface.Definition rsrcdef = zonedef.LoadResources(rsrc_index, cache, playdef.ResourceLayoutTable);
					cache.TagIndexManager.Unload(sbsp_index);
				}

				if (map == kMapPathSandbox)
				{
					var mode_index = cache.TagIndexManager.Open(@"objects\levels\dlc\shared\golf_club\fp_golf_club\fp_golf_club", Blam.Halo3.TagGroups.mode);
					var modeman = cache.TagIndexManager[mode_index];
					var modedef = modeman.TagDefinition as Blam.Halo3.Tags.render_model_group;
					Blam.DatumIndex rsrc_index = modedef.ResourceId.Value;
					TagInterface.Definition rsrcdef = zonedef.LoadResources(rsrc_index, cache, playdef.ResourceLayoutTable, true);

					(rsrcdef as Blam.Halo3.Tags.render_geometry_api_resource_definition).Debug(modeman,
						Path.Combine(kTestResultsPath, "debug_render_geometry.txt"), true);

					cache.TagIndexManager.Unload(mode_index);
				}

				playdef = null;
				cache.TagIndexManager.Unload(play_index);
				zonedef = null;
				cache.TagIndexManager.Unload(zone_index);
			}
		}
		[TestMethod]
		public void Halo3TestCacheLoadResourceSpecificXbox()
		{
			CacheLoadResourceSpecificXbox(kMapPathChillout);
			CacheLoadResourceSpecificXbox(kMapPathSandbox);
		}
		#endregion

		#region CacheLoadResource
		internal static void CacheLoadResourceMethod(object param)
		{
			bool output_play = false;
			bool output_zone = false;
			bool output_sound_info = true;
			bool output_index = false;


			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.Halo3.CacheFileBase>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				#region output_play
				if (output_play)
				{
					var play_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.play);
					var playman = cache.TagIndexManager[play_index];
					var playdef = playman.TagDefinition as Blam.Halo3.Tags.cache_file_resource_layout_table_group;
					using (var sw = new StreamWriter(Path.Combine(kTestResultsPath, cache.Header.Name) + "_output_play.txt"))
					{
						Blam.Halo3.Tags.cache_file_resource_layout_table.Output(sw, playdef.ResourceLayoutTable);
					}

// 					Blam.Halo3.Tags.cache_file_resource_layout_table.OutputResources(kTestResultsPath, cache,
// 						playdef.ResourceLayoutTable);
					cache.TagIndexManager.Unload(play_index);
				}
				#endregion

				#region output_zone
				if (output_zone)
				{
					var zone_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.zone);
					var zoneman = cache.TagIndexManager[zone_index];
					using (var sw = new StreamWriter(Path.Combine(kTestResultsPath, cache.Header.Name) + "_output_zone.txt"))
					{
						var zone = zoneman.TagDefinition as Blam.Halo3.Tags.cache_file_resource_gestalt_group;
						Blam.Halo3.Tags.cache_file_resource_gestalt_group.Output(cache, sw, zone);

// 						using (var fs = new FileStream(
// 							Path.Combine(kTestResultsPath, cache.Header.Name) + "_output_zone_data.bin", 
// 							FileMode.Create, FileAccess.Write, FileShare.Read))
//  						{
//  							Blam.Halo3.Tags.cache_file_resource_gestalt_group.OutputData(fs, zone);
//  						}
					}
					cache.TagIndexManager.Unload(zone_index);
				}
				#endregion

				#region output sound info
				if (output_sound_info)
				{
					using (var sw = new StreamWriter(Path.Combine(kTestResultsPath, cache.Header.Name) + "_output_sounds.txt"))
					{
						var dic = new Dictionary<int, List<string>>();
						dic.Add(0, new List<string>());
						dic.Add(1, new List<string>());
						dic.Add(2, new List<string>());
						dic.Add(3, new List<string>());
						dic.Add(4, new List<string>());
						dic.Add(5, new List<string>());

						sw.WriteLine("sound definitions");
						foreach (var tag in cache.IndexHalo3)
						{
							if (tag.GroupTag != BlamLib.Blam.Halo3.TagGroups.snd_) continue;

							var snd_index = cache.TagIndexManager.Open(tag.Datum);
							var snd_man = cache.TagIndexManager[snd_index];
// 							sw.WriteLine("\t{0}\t{1}", tag.Datum.ToString(), cache.References[tag.ReferenceName]);
// 							Blam.Halo3.Tags.cache_file_sound_group.Output(sw,
// 								snd_man.TagDefinition as Blam.Halo3.Tags.cache_file_sound_group);
// 							sw.WriteLine();
							Blam.Halo3.Tags.cache_file_sound_group.Output(dic,
								snd_man.TagDefinition as Blam.Halo3.Tags.cache_file_sound_group,
								string.Format("{0}\t{1}", tag.Datum.Handle.ToString("X8"), cache.References[tag.ReferenceName]));
							cache.TagIndexManager.Unload(snd_index);
						}

						foreach (KeyValuePair<int, List<string>> kv in dic)
						{
							sw.WriteLine("\tCompression: {0}\t{1}", kv.Key.ToString(), kv.Value.Count.ToString());
							foreach (string s in kv.Value)
								sw.WriteLine(s);
						}
						sw.WriteLine();

						sw.WriteLine("sound gestalt");
					}
				}
				#endregion

				if (output_index)
				{
//					Blam.CacheFile.OutputStringIds(cache, 
//						BuildResultPath(kTestResultsPath, args.Game, cache.Header.Name, "output_string_id", "txt"), true);
					Blam.CacheFile.OutputTags(cache,
						BuildResultPath(kTestResultsPath, args.Game, cache.Header.Name, "output_index", "txt"));
				}
			}
		}
		[TestMethod]
		public void Halo3TestCacheLoadResourceXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheLoadResourceMethod,
				BlamVersion.Halo3_Xbox, kDirectoryXbox, kMapNames_Retail);
		}
		#endregion
	};
}