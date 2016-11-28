/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo3
	{
		struct SndData
		{
			ushort Flags;
			byte SoundClass;
			byte SampleRate;
			byte Encoding;
			public byte CodecIndex;
			short PlaybackIndex;
			short Unk08, Unk0A;

			public SndData(Blam.Halo3.Tags.cache_file_sound_group def)
			{
				Flags = (ushort)def.Flags.Value;
				SoundClass = (byte)def.SoundClass.Value;
				SampleRate = (byte)def.SampleRate.Value;
				Encoding = (byte)def.Encoding.Value;
				CodecIndex = (byte)def.CodecIndex.Value;
				PlaybackIndex = def.PlaybackIndex.Value;
				Unk08 = def.Unknown08.Value;
				Unk0A = def.Unknown0A.Value;

// 				int h1, h2, h3;
// 				h1 = Flags;
// 				h1 <<= 8;
// 				h1 |= SoundClass;
// 				h1 <<= 8;
// 				h1 |= SampleRate;
// 
// 				h2 = Encoding;
// 				h2 <<= 8;
// 				h2 |= CodecIndex;
// 				h2 <<= 8;
// 				h2 |= (ushort)PlaybackIndex;
// 
// 				h3 = Unk08;
// 				h3 <<= 16;
// 				h3 |= (ushort)Unk0A;
// 
// 				kHashCode = h1 ^ h2 ^ h3;
				kHashCode = CodecIndex;
			}

			readonly int kHashCode;
			public override int GetHashCode()	{ return kHashCode; }

			public void Output(StreamWriter s)
			{
				s.WriteLine("\t" + 
					"\t{0}\t{1}\t{2}" + // Flags,SoundClass,SampleRate
					"\t{3}\t{4}\t{5}" + // Encoding,CodecIndex,PlaybackIndex
					"\t{6}\t{7}", // 08,0A
					Flags.ToString("X4"), SoundClass.ToString("X2"), SampleRate.ToString("X2"),
					Encoding.ToString("X2"), CodecIndex.ToString("X2"), PlaybackIndex.ToString("X4"),
					Unk08.ToString("X4"), Unk0A.ToString("X4")
				);
			}
		};

		class MapSndComparer
		{
			public string map_name;
			public Dictionary<string, SndData> dic;

			public MapSndComparer(string name, int tag_count)
			{
				map_name = name;
				dic = new Dictionary<string, SndData>(tag_count/3);
			}

			public void Add(string name, Blam.Halo3.Tags.cache_file_sound_group def)
			{
				var dat = new SndData(def);
				dic.Add(name, dat);
			}
		};

		class SndComparer
		{
			List<MapSndComparer> maps;
			Dictionary<string, Dictionary<string, SndData>> dic;

			public SndComparer(int map_count)
			{
				maps = new List<MapSndComparer>(map_count);
				dic = new Dictionary<string, Dictionary<string, SndData>>();
				counts = new Dictionary<string, int>(20 * 1000);
			}

			Dictionary<string, int> counts;
			public void PreProcess(List<MapSndComparer> maps, int map_count)
			{
				this.maps = maps;

				// add all the tag names
				foreach(var map in maps)
					foreach (string s in map.dic.Keys)
						if (!counts.ContainsKey(s))
							counts.Add(s, 1);
						else
							counts[s]++;

				// cull any tag name which doesn't appear more than once
				foreach (var kv in counts)
					if (kv.Value > 1)
						dic.Add(kv.Key, new Dictionary<string, SndData>(map_count));
				counts = null;
			}

			public void ScanForDiffs()
			{
				foreach (var map in maps)
					foreach(var kv in dic) // for each map, then for each tag name
					{
						if (!map.dic.ContainsKey(kv.Key)) // is the tag name part of our post processed list?
							continue;

						dic[kv.Key].Add(map.map_name, map.dic[kv.Key]); // it is, so add it's data from this map
					}
				maps = null;

				List<string> null_this_shit = new List<string>(dic.Count / 3);
				foreach(var kv in dic)
				{
					bool has_diffs = false;
					int hash_code = 0;
					foreach(var kv2 in kv.Value) // not the best comparison method, but should fit our needs
					{
						if (has_diffs) break; // we already found a diff, don't bother continuing

						int hc = kv2.Value.GetHashCode();

						if (hash_code == 0) // first kv2
							hash_code = hc;
						else
						{
							has_diffs = (hash_code ^ hc) != 0;
							hash_code = hc; // next loop will use the current kv2's hash code
						}
					}

					// element had no diffs, dispose
					if (!has_diffs) null_this_shit.Add(kv.Key);//dic[kv.Key] = null;
				}

				foreach (string s in null_this_shit)
					dic.Remove(s);
			}

			public void OutputDiffs(string path)
			{
				using(var s = new StreamWriter(path))
				{
					foreach(var kv in dic)
					{
						s.WriteLine(kv.Key);
						foreach(var kv2 in kv.Value)
						{
							s.WriteLine("\t{0}\t{1}", kv2.Value.CodecIndex.ToString("X2"), kv2.Key);
							//kv2.Value.Output(s);
						}
					}
				}
			}
		};

		static MapSndComparer SndComparerMethod(CacheFileOutputInfoArgs args)
		{
			string dir = Path.GetDirectoryName(args.MapPath);

			MapSndComparer cmp;
			using (var handler = new CacheHandler<Blam.Halo3.CacheFileBase>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				cmp = new MapSndComparer(args.MapPath, cache.IndexHalo3.TagCount);
				foreach (var tag in cache.IndexHalo3)
				{
					if (tag.GroupTag != Blam.Halo3.TagGroups.snd_) continue;

					var snd_index = cache.TagIndexManager.Open(tag.Datum);
					var snd_man = cache.TagIndexManager[snd_index];

					cmp.Add(cache.References[snd_man.ReferenceName], snd_man.TagDefinition as Blam.Halo3.Tags.cache_file_sound_group);
					cache.TagIndexManager.Unload(snd_index);
				}

			}

			return cmp;
		}
		internal static void TestSndComparer(TestContext tc, BlamVersion game, string dir, string[] map_names)
		{
			int map_count = map_names.Length;
			List<MapSndComparer> maps = new List<MapSndComparer>(map_count);

			var thread_code = new System.Threading.WaitCallback(delegate(object param)
			{
				var args = param as CacheFileOutputInfoArgs;

				MapSndComparer cmp = SndComparerMethod(args);
				lock (maps)
					maps.Add(cmp);

				args.SignalFinished();
			});

			CacheFileOutputInfoArgs.TestThreadedMethod(tc, thread_code,
				game, dir, map_names);

			SndComparer snd_cmp = new SndComparer(map_count);
			snd_cmp.PreProcess(maps, map_count);
			snd_cmp.ScanForDiffs();
			snd_cmp.OutputDiffs(BuildResultPath(kTestResultsPath, game, null, "sound_cmp_results", "txt"));
		}

		[TestMethod]
		public void Halo3TestSndComparerXbox()
		{
			TestSndComparer(TestContext, BlamVersion.Halo3_Xbox, kDirectoryXbox, kMapNames_Retail);
		}
		[TestMethod]
		public void Halo3TestSndComparerXboxEpsilon()
		{
			TestSndComparer(TestContext, BlamVersion.Halo3_Epsilon, kDirectoryXbox, kMapNames_Epsilon);
		}
	};
}