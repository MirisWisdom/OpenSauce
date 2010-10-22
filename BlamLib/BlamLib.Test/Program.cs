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
﻿using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	static class TestLibrary
	{
		static System.DateTime startup_time;

		public const string kTestResultsPath = @"C:\Mount\B\Kornner\Projects\test_results\BlamLib\";

		[AssemblyInitialize]
		public static void AssemblyInitialize(TestContext context)
		{
			startup_time = System.DateTime.Now;

			BlamLib.Program.Initialize();

			// If this isn't true, then whoever is using this code didn't update 
			// [kTestResultsPath] to reflect their project tree's test results dir.
			// 
			// We don't use the TextContext's TestDir properties because there have 
			// been issues with VS using the "In" dir, and deleting whatever gets 
			// outputted into there. But hey, maybe I'm just doing something wrong!
			Assert.IsTrue(System.IO.Directory.Exists(kTestResultsPath));
		}
		[AssemblyCleanup]
		public static void AssemblyDispose()
		{
			BlamLib.Program.Close();

			System.Diagnostics.Debug.Print("\nTIME TAKEN: {0}\n",
				(System.DateTime.Now - startup_time));
		}

		public static void TestMethod(ParameterizedThreadStart method, params object[] inputs)
		{
			var threads = new Thread[4];

			for (int x = 0; x < inputs.Length; x += threads.Length)
			{
				for (int t_idx = 0;
					(x + t_idx) < inputs.Length && t_idx < threads.Length;
					t_idx++)
				{
					threads[t_idx] = new System.Threading.Thread(method);
					threads[t_idx].Name = string.Format("TestMethod::Thread[{0}]", t_idx);
					threads[t_idx].Start(inputs[x + t_idx]);
				}

				foreach (var t in threads)
					if (t != null && t.ThreadState != ThreadState.Unstarted) t.Join();
			}
		}
	};

	class CacheFileOutputInfoArgs
	{
		public readonly TestContext TestContext;
		public readonly BlamVersion Game;
		public readonly string Directory;
		public readonly string MapName;
		readonly string mapPath;

		public CacheFileOutputInfoArgs(TestContext tc, BlamVersion g, string d, string m)
		{
			TestContext = tc;
			Game = g;
			Directory = d;
			MapName = m;
			mapPath = System.IO.Path.Combine(Directory, MapName);
		}

		public string MapPath { get { return mapPath; } }

		bool ValidateReadyStatus()
		{
			if (!System.IO.File.Exists(mapPath))
			{
				TestContext.WriteLine("Map not found: {0}", mapPath);
				return false;
			}

			return true;
		}

		public static void TestThreadedMethod(TestContext tc, ParameterizedThreadStart method,
			BlamVersion game, string dir, params string[] map_names)
		{
			var args = new System.Collections.Generic.List<CacheFileOutputInfoArgs>(map_names.Length);
			for (int x = 0; x < map_names.Length; x++)
			{
				var arg = new CacheFileOutputInfoArgs(tc, game, dir, map_names[x]);

				if(arg.ValidateReadyStatus())
					args.Add(arg);
			}

			TestLibrary.TestMethod(method, args.ToArray());
		}
	};


	[TestClass]
	public abstract class BaseTestClass
	{
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		public TestContext TestContext { get; set; }

		protected static string BuildResultPath(string test_results_root, BlamVersion engine, 
			string map_name, string file_name, string file_ext)
		{
			string format = !string.IsNullOrEmpty(map_name) ?
				"{0}_{1}" : "{1}";
			format += !string.IsNullOrEmpty(file_name) ?
				"_{2}.{3}" : ".{3}";

			return System.IO.Path.Combine(test_results_root, string.Format(
				format,
				map_name,
				engine.ToString(),
				file_name, file_ext));
		}
	};

	/// <summary>
	/// Utility class just to make things and code easier with cache files
	/// </summary>
	/// <typeparam name="T"></typeparam>
	class CacheHandler<T> : System.IDisposable 
		where T : Blam.CacheFile
	{
		Blam.DatumIndex cacheHandle;
		T cacheInterface;

		public T CacheInterface { get {
			Assert.AreNotEqual(cacheHandle, Blam.DatumIndex.Null);
			Assert.IsNotNull(cacheInterface);

			return cacheInterface;
		} }

		public CacheHandler(BlamVersion game, string path)
		{
			cacheHandle = Program.GetManager(game).OpenCacheFile(game, path);
			cacheInterface = Program.GetCacheFile(cacheHandle) as T;
		}

		public void Read()
		{
			Assert.AreNotEqual(cacheHandle, Blam.DatumIndex.Null);

			cacheInterface.Read();
		}

		public void Dispose()
		{
			if(cacheHandle != Blam.DatumIndex.Null)
			{
				cacheInterface = null;
				BlamLib.Program.CloseCacheFile(cacheHandle);
				cacheHandle = Blam.DatumIndex.Null;
			}
		}

		public static implicit operator T(CacheHandler<T> handler)
		{
			return handler.CacheInterface;
		}
	};

	/// <summary>
	/// Utility class just to make things and code easier with tag indexes
	/// </summary>
	/// <typeparam name="T"></typeparam>
	class TagIndexHandler<T> : System.IDisposable
		where T : Managers.TagIndexBase
	{
		Blam.DatumIndex indexHandle;
		T indexInterface;

		public T IndexInterface { get {
			Assert.AreNotEqual(indexHandle, Blam.DatumIndex.Null);
			Assert.IsNotNull(indexInterface);

			return indexInterface;
		} }

		public TagIndexHandler(BlamVersion game, string path)
		{
			indexHandle = Program.GetManager(game).OpenTagIndex(game, path);
			indexInterface = Program.GetTagIndex(indexHandle) as T;
		}

		public void Dispose()
		{
			if (indexHandle != Blam.DatumIndex.Null)
			{
				indexInterface = null;
				BlamLib.Program.CloseTagIndex(indexHandle);
				indexHandle = Blam.DatumIndex.Null;
			}
		}

		public static implicit operator T(TagIndexHandler<T> handler)
		{
			return handler.IndexInterface;
		}
	};
}