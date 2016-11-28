/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	static partial class TestLibrary
	{
		static Stopwatch gTestsStopwatch;

		[AssemblyInitialize]
		public static void AssemblyInitialize(TestContext context)
		{
			gTestsStopwatch = Stopwatch.StartNew();

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

			gTestsStopwatch.Stop();
			System.Diagnostics.Debug.Print("\nTIME TAKEN: {0}\n",
				gTestsStopwatch.Elapsed);
		}

		public static void TestMethodThreaded(WaitCallback method, params ThreadedTaskArgsBase[] args)
		{
			Assert.IsTrue(args.Length > 0, "Why are there zero tasks?");

			// Can't use WaitAll in STA
			// See the following for a work around: http://blogs.msdn.com/b/ploeh/archive/2007/10/21/runningmstestinanmta.aspx
			Assert.IsFalse(Thread.CurrentThread.GetApartmentState() == ApartmentState.STA);

			var waiters = ThreadedTaskArgsBase.InitializeArgWaiters(args);

			foreach (var arg in args)
				ThreadPool.QueueUserWorkItem(method, arg);

			WaitHandle.WaitAll(waiters);
		}

		public static void TestMethodSerial(WaitCallback method, params ThreadedTaskArgsBase[] args)
		{
			Assert.IsTrue(args.Length > 0, "Why are there zero tasks?");

			foreach (var arg in args)
				method(arg);
		}
	};

	class ThreadedTaskArgsBase
	{
		AutoResetEvent m_waiter;

		public void SignalFinished()	{ if(m_waiter != null) m_waiter.Set(); }

		public static WaitHandle[] InitializeArgWaiters(params ThreadedTaskArgsBase[] args)
		{
			AutoResetEvent[] waiters = new AutoResetEvent[args.Length];

			for (int x = 0; x < waiters.Length; x++)
				args[x].m_waiter = waiters[x] = new AutoResetEvent(false);

			return waiters;
		}
	}

	class CacheFileOutputInfoArgs : ThreadedTaskArgsBase
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

		public string TestResultsPath
		{ get {
			switch (Game.ToBuild())
			{
				case BlamBuild.Halo1: return Halo1.kTestResultsPath;
				case BlamBuild.Halo2: return Halo2.kTestResultsPath;
				case BlamBuild.Halo3: return Halo3.kTestResultsPath;
				case BlamBuild.HaloOdst: return HaloOdst.kTestResultsPath;
				case BlamBuild.HaloReach: return HaloReach.kTestResultsPath;
				case BlamBuild.Halo4: return Halo4.kTestResultsPath;
				case BlamBuild.Stubbs: return Stubbs.kTestResultsPath;

				default: throw new Debug.Exceptions.UnreachableException(Game.ToString());
			}
		} }

		bool ValidateReadyStatus()
		{
			if (!System.IO.File.Exists(mapPath))
			{
				TestContext.WriteLine("Map not found: {0}", mapPath);
				return false;
			}

			return true;
		}

		static List<CacheFileOutputInfoArgs> TestMethodBuildArgs(TestContext tc, 
			BlamVersion game, string dir, params string[] map_names)
		{
			var args = new List<CacheFileOutputInfoArgs>(map_names.Length);
			for (int x = 0; x < map_names.Length; x++)
			{
				var arg = new CacheFileOutputInfoArgs(tc, game, dir, map_names[x]);

				if (arg.ValidateReadyStatus())
					args.Add(arg);
			}

			return args;
		}
		public static void TestThreadedMethod(TestContext tc, WaitCallback method,
			BlamVersion game, string dir, params string[] map_names)
		{
			var args = TestMethodBuildArgs(tc, game, dir, map_names);

			TestLibrary.TestMethodThreaded(method, args.ToArray());
		}

		public static void TestMethodSerial(TestContext tc, WaitCallback method,
			BlamVersion game, string dir, params string[] map_names)
		{
			var args = TestMethodBuildArgs(tc, game, dir, map_names);

			TestLibrary.TestMethodSerial(method, args.ToArray());
		}
	};


	[TestClass]
	public abstract class BaseTestClass
	{
		#region Stop watches
		protected Stopwatch m_testStopwatch = new Stopwatch();
		protected void StartStopwatch()
		{
			m_testStopwatch.Reset();
			m_testStopwatch.Start();
		}
		protected System.TimeSpan StopStopwatch()
		{
			m_testStopwatch.Stop();
			return m_testStopwatch.Elapsed;
		}

		protected Stopwatch m_subTestStopwatch = new Stopwatch();
		protected void StartSubStopwatch()
		{
			m_subTestStopwatch.Reset();
			m_subTestStopwatch.Start();
		}
		protected System.TimeSpan StopSubStopwatch()
		{
			m_subTestStopwatch.Stop();
			return m_subTestStopwatch.Elapsed;
		}
		#endregion

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

		public TagIndexHandler(BlamVersion game, string root, string tagsDir)
		{
            indexHandle = Program.GetManager(game).OpenTagIndex(game, root, tagsDir);
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

	/// <summary>Utility class for COLLADA nonsense</summary>
	class ModelTestDefinition
	{
		public string TypeString;
		public string Name;
		public TagInterface.TagGroup Group;

		public ModelTestDefinition(string type, string name, TagInterface.TagGroup group)
		{
			TypeString = type;
			Name = name;
			Group = group;
		}

		public Blam.DatumIndex TagIndex = Blam.DatumIndex.Null;
		public void Open(Managers.TagIndex tag_index)
		{
			TagIndex = tag_index.Open(Name, Group, IO.ITagStreamFlags.LoadDependents);
			Assert.IsTrue(BlamLib.Managers.TagIndex.IsValid(TagIndex));
		}
		public void Close(Managers.TagIndex tag_index)
		{
			Assert.IsTrue(tag_index.Unload(TagIndex));
			TagIndex = Blam.DatumIndex.Null;
		}
	};
}