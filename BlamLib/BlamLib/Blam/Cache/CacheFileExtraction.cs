/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Blam
{
	/// <summary>State information for tag extraction from a cache</summary>
	internal sealed class CacheExtraction
	{
		/// <summary>
		/// The tag datum which is currently being processed (ie, extracted and reconstructed)
		/// </summary>
		public DatumIndex CurrentTag = DatumIndex.Null;
		/// <summary>
		/// These are the tag datums which we are to process, along with a flag indicating if they have been processed yet or not.
		/// </summary>
		/// <remarks>Items are not removed once 'dequeued'</remarks>
		Dictionary<DatumIndex, bool> Datums;

		/// <summary>Initialize the extraction state</summary>
		/// <param name="tag_count">Provide the tag count to provide better operation performance</param>
		public CacheExtraction(int tag_count)
		{
			Datums = new Dictionary<DatumIndex, bool>(tag_count, DatumIndex.kEqualityComparer); // tag_count should be our only needed capacity
		}

		#region Queue
		/// <summary>Add a tag to the extraction queue</summary>
		/// <param name="index"></param>
		public void Queue(DatumIndex index)
		{
			if (index == DatumIndex.Null)
				throw new ArgumentNullException("Can't queue a null tag index!");

			if (Datums.ContainsKey(index)) return;
			Datums.Add(index, false);
		}

		/// <summary>Remove a tag to the extraction queue</summary>
		/// <param name="index"></param>
		public void Dequeue(DatumIndex index)		{ Datums[index] = true; }

		/// <summary>Remove a list of tags from the extraction queue</summary>
		/// <param name="tags"></param>
		public void Dequeue(List<DatumIndex> tags)	{ foreach (DatumIndex di in tags) Datums[di] = true; }
		#endregion

		#region Util
		/// <summary>Get a list of tags that are dependents of the last tag read from the cache</summary>
		/// <returns></returns>
		public IEnumerable<DatumIndex> CurrentDependents()
		{
			var ret = new List<DatumIndex>(Datums.Keys.Count);
			foreach (DatumIndex di in Datums.Keys)
				if (!Datums[di]) ret.Add(di);

			ret.TrimExcess();
			return ret;
		}

		/// <summary>Has this tag been processed by the extractor yet?</summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		public bool Processed(DatumIndex tag)
		{
			if (!Datums.ContainsKey(tag)) return false;

			return Datums[tag];
		}
		#endregion

		CacheExtractionInfo info;
		public CacheExtractionInfo Info { get { return info; } }
		public void ResetCurrentInfo(CacheExtractionInfo info)
		{
			this.info = info;
		}
	};

	/// <summary>Arguments for the public interface for running extraction on a set of tags</summary>
	public class CacheExtractionArguments
	{
		/// <summary>Base directory the extraction is going to</summary>
		public readonly string OutputDirectory;

		/// <summary>Are with extracting with the tag's dependent's too?</summary>
		public readonly bool WithDependents;
		/// <summary>Should the extractor output a tag_database after it finishes?</summary>
		/// <remarks>Only valid if <see cref="WithDependents"/> is used</remarks>
		public readonly bool OutputDatabase;
		/// <summary>Should we overwrite existing files on disc?</summary>
		public readonly bool OverwriteExisting;

		/// <summary>Groups which won't be extracted no matter what</summary>
		internal readonly TagInterface.TagGroupCollection DontExtractGroups;

		public CacheExtractionArguments(string out_dir, bool output_db, bool with_depns, 
			bool overwrite_existing, TagInterface.TagGroupCollection dont_extract_groups)
		{
			OutputDirectory = out_dir;
			if(WithDependents = with_depns) // yes, '='
				OutputDatabase = output_db;
			OverwriteExisting = overwrite_existing;
			DontExtractGroups = dont_extract_groups;
		}

		internal void CreateOutputDirectory()
		{
			if (!System.IO.Directory.Exists(OutputDirectory))
				System.IO.Directory.CreateDirectory(OutputDirectory);
		}
	};
	/// <summary>Provides extended information and data collection for extraction from a cache</summary>
	public sealed class CacheExtractionInfo
	{
		/// <summary>Database used to record all extracted tags</summary>
		internal Managers.CacheTagDatabase Database;
		/// <summary>Database used to record tags which have problems during the extraction process</summary>
		internal Managers.ErrorTagDatabase DatabaseErrors;

		#region Definition
		readonly CacheIndex.Item root_definition;

		CacheIndex.Item definition;
		/// <summary>Tag instance being processed</summary>
		public CacheIndex.Item Definition	{ get { return definition; } }

		public void Reset(CacheIndex.Item new_definition)
		{
			definition = new_definition;
			Database.SetRoot(definition);
		}
		#endregion

		/// <summary>
		/// How many levels deep (in terms of a a tag hierarchy) we've gone in the extraction process
		/// </summary>
		internal int ExtractionDepth = 0;

		public readonly CacheExtractionArguments Arguments;

		public CacheExtractionInfo(CacheFile cf, DatumIndex tag_datum, CacheExtractionArguments args)
		{
			var bd = Program.GetManager(cf.EngineVersion);
			Database = bd.CreateCacheTagDatabase(cf.CacheId);
			DatabaseErrors = bd.CreateErrorTagDatabase();

			root_definition = cf.Index.Tags[tag_datum.Index];
			Reset(root_definition);

			Arguments = args;
			Arguments.CreateOutputDirectory();
		}
	};


	partial class CacheFile
	{
		internal CacheExtraction ExtractionState = null;
		internal void PrepareForExtraction(CacheExtractionInfo initial_info)
		{
			ExtractionState = new CacheExtraction(Index.TagCount);
		}
	};
}