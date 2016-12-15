/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;

namespace BlamLib.Managers
{
	partial class CacheTagIndex
	{
		#region Util
		/// <summary>
		/// For deciding if <paramref name="tag_datum"/> exists on disk already and thus shouldn't be extracted
		/// </summary>
		/// <param name="cei"></param>
		/// <param name="tag_datum">Engine's index of the tag instance</param>
		/// <returns>True if the tag exists on disk, false if it doesn't</returns>
		protected virtual bool ExtractionTagExistsOnDisk(Blam.CacheExtractionInfo cei, Blam.DatumIndex tag_datum)
		{
			if(!cei.Arguments.OverwriteExisting)
			{
				string base_dir = cei.Arguments.OutputDirectory;
				string tag_name = cacheFile.GetTagIndexName(tag_datum, true);

				return File.Exists(Path.Combine(base_dir, tag_name));
			}

			return false;
		}
		/// <summary>For deciding if <paramref name="tag_datum"/> shouldn't be extracted to disc or not</summary>
		/// <param name="cei"></param>
		/// <param name="tag_datum">Engine's index of the tag instance</param>
		/// <returns>True if the tag is not suppose to be extract, false if it is</returns>
		protected virtual bool ExtractionDontExtract(Blam.CacheExtractionInfo cei, Blam.DatumIndex tag_datum)
		{
			bool dont_extract = false;

			var ignore_groups = cei.Arguments.DontExtractGroups;
			if (ignore_groups != null)
			{
				var item = cacheFile.Index.Tags[tag_datum.Index];

				dont_extract = ignore_groups.Contains(item.GroupTag);
			}

			// tag isn't of a group tag which we're ignoring so then check if 
			// it is ignored for already existing
			if(!dont_extract)
				dont_extract = ExtractionTagExistsOnDisk(cei, tag_datum);

			return dont_extract;
		}

		/// <summary>For changing tag references to different references</summary>
		/// <param name="tr"></param>
		/// <returns>true if <paramref name="tr"/> was changed</returns>
		/// <remarks>
		/// This is needed for implementing re-mappings, particularly for Halo 2 where 
		/// we haven't coded post-process definition decoding so we'd want to set the 
		/// reference to "rasterizer\invalid".
		/// 
		/// I've thought about having to update cacheFile's "References" state...but 
		/// I'm undecided on this action...
		/// </remarks>
		internal virtual bool ExtractionTagReferenceChange(TagInterface.TagReference tr)
		{
			return false;
		}
		#endregion

		#region System Interface
		/// <summary>Initializes any resources needed for extraction</summary>
		public virtual void ExtractionInitialize() { }
		/// <summary>Disposes any resources loaded in <see cref="ExtractionInitialize"/></summary>
		/// <remarks>Always called when a <see cref="Blam.CacheFile"/> is disposed</remarks>
		public virtual void ExtractionDispose() { }

		/// <summary>Initialize the extraction process</summary>
		/// <param name="tag_datum"></param>
		/// <param name="args"></param>
		/// <returns></returns>
		public Blam.CacheExtractionInfo ExtractionBegin(Blam.DatumIndex tag_datum, Blam.CacheExtractionArguments args)
		{
			if (tag_datum == Blam.DatumIndex.Null)
				throw new ArgumentNullException("tag_datum", "Null datum index!");

			var cei = new Blam.CacheExtractionInfo(cacheFile, tag_datum, args);
			cacheFile.PrepareForExtraction(cei);

			return cei;
		}
		/// <summary>Dispose of the last call to <see cref="ExtractionBegin"/></summary>
		public void ExtractionEnd() { }
		#endregion

		#region Extract Extras
		void ExtractWriteTagDatabase(Blam.CacheExtractionInfo cei, TagManager root_tag, 
			TagInterface.TagGroup database_group,
			TagInterface.Definition db_definition, bool is_error_db)
		{
			if ((db_definition as ITagDatabase).IsEmpty)
				return;

			// name the database after the root tag we're extracting
			string tag_name = root_tag.Name;
			if (is_error_db)
				tag_name = string.Format("{0}.errors", tag_name);

			Blam.CacheIndex.Item tdb_item;
			// Just in-case someone tries to extract the same root tag twice
			if (!cacheFile.TryAndFind(tag_name, database_group, out tdb_item))
			{
				tdb_item = cacheFile.AddFeignTagInstance(tag_name, database_group);

				if (tdb_item == null)
				{
					extractionTrace.WriteLine("Couldn't create a tag_database for {0}! Couldn't create tag entry for database",
						root_tag.Name);
					return;
				}
			}

			try
			{
				var tdb_index = this.Open(tdb_item.Datum);

				var tdb = this[tdb_index];
				tdb.Manage(db_definition);

				// Even though the tag isn't actually in the cache, the tag 
				// manager needs to operate this way with CacheTagIndex elements
				tdb.OpenForExtract(cei.Arguments.OutputDirectory, null);
				tdb.Extract();

				tdb.Close();
				Unload(tdb_index);
			}
			catch(Exception ex)
			{
				extractionTrace.WriteLine("Error while trying to write tag_database!");
				extractionTrace.WriteLine(ex);
			}
		}
		void ExtractWriteTagDatabases(Blam.CacheExtractionInfo cei, TagManager root_tag)
		{
			var tdb_group = Program.GetManager(Engine).TagDatabaseGroup;

			ExtractWriteTagDatabase(cei, root_tag, tdb_group, 
				cei.Database.Definition, false);
			ExtractWriteTagDatabase(cei, root_tag, tdb_group,
				cei.DatabaseErrors.Definition, true);
			
		}

		// Initializes the error DB if it hasn't already been setup
		void ExtractWithDependentsPreprocessErrorDatabase(ErrorTagDatabase etdb, TagManager tag, ref bool root_is_initialized)
		{
			if (!root_is_initialized)
			{
				etdb.SetRoot(this.References[tag.ReferenceName], tag.GroupTag.ID);
				root_is_initialized = true;
			}
		}
		// Setup the error DB and add a skipped tag entry
		void ExtractWithDependentsErrorSkipped(ErrorTagDatabase etdb, TagManager tag, ref bool root_is_initialized, Blam.DatumIndex error_tag)
		{
			ExtractWithDependentsPreprocessErrorDatabase(etdb, tag, ref root_is_initialized);

			var error_item = cacheFile.Index.Tags[error_tag.Index];
			etdb.AddDependent(References[error_item.ReferenceName], error_item.GroupTag.ID,
				ErrorTagDatabase.ErrorFlags.Skipped);
		}
		// Setup the error DB and add a failed tag entry
		void ExtractWithDependentsErrorFailed(ErrorTagDatabase etdb, TagManager tag, ref bool root_is_initialized, Blam.DatumIndex error_tag)
		{
			ExtractWithDependentsPreprocessErrorDatabase(etdb, tag, ref root_is_initialized);

			var error_item = cacheFile.Index.Tags[error_tag.Index];
			etdb.AddDependent(References[error_item.ReferenceName], error_item.GroupTag.ID,
				ErrorTagDatabase.ErrorFlags.Failure);
		}
		bool ExtractWithDependents(Blam.CacheExtractionInfo cei, TagManager tag)
		{
			ErrorTagDatabase etdb = cei.DatabaseErrors;
			bool error_root_is_initialized = false;

			var depns = cacheFile.ExtractionState.CurrentDependents();
			var depns_to_extract = new List<Blam.DatumIndex>();

			foreach (Blam.DatumIndex datum in depns)
			{
				if (!ExtractionDontExtract(cei, datum))
				{
					cei.Database.AddDependent(cacheFile.Index.Tags[datum.Index]);
					depns_to_extract.Add(datum);
				}
				else
				{
					ExtractWithDependentsErrorSkipped(etdb, tag, ref error_root_is_initialized, datum);
					cacheFile.ExtractionState.Dequeue(datum);
				}
			}

			// extract the valid dependents
			foreach (Blam.DatumIndex datum in depns_to_extract)
			{
				cei.Reset(cacheFile.Index.Tags[datum.Index]);
				if (!Extract(cei, datum, true))
					ExtractWithDependentsErrorFailed(etdb, tag, ref error_root_is_initialized, datum);
			}

			return true;
		}
		#endregion

		#region Extract
		bool Extract(Blam.CacheExtractionInfo cei, Blam.DatumIndex tag_datum, bool remove_from_state)
		{
			if (remove_from_state)
			{
				// don't process it if it's already been done
				if (cacheFile.ExtractionState.Processed(tag_datum)) return true;
				cacheFile.ExtractionState.Dequeue(tag_datum);
			}
			cacheFile.ExtractionState.CurrentTag = tag_datum;

			Blam.CacheIndex.Item ci = cacheFile.Index.Tags[tag_datum.Index];

			bool error = false;
			Blam.DatumIndex handle = Open(tag_datum);
			if (handle != Blam.DatumIndex.Null)
			{
				// we're about to extract, update our depth level
				cei.ExtractionDepth++;
				TagManager tm = Array[handle];

				#region read the tag from the cache
				try { tm.OpenForExtract(cei.Arguments.OutputDirectory, null); }
				catch (Exception ex) { extractionTrace.WriteLine("Failed to read tag, aborting extraction...'{0}.{1}'{2}{3}", tm.Name, tm.GroupTag.Name, Program.NewLine, ex); error = true; }
				#endregion

				#region if no errors
				if(!error)
				{
					try { tm.Extract(); }
					catch (Exception ex) { extractionTrace.WriteLine("Failed to write tag, aborting extraction...'{0}.{1}'{2}{3}", tm.Name, tm.GroupTag.Name, Program.NewLine, ex); error = true; }

					tm.Close();
				}

				// something bad happened, so make sure we don't keep any bad tags
				if (error) System.IO.File.Delete(Path.Combine(cei.Arguments.OutputDirectory, tm.Name));

				// process dependents if we want to
				if (!error && cei.Arguments.WithDependents)
					error = !ExtractWithDependents(cei, tm);
				#endregion

				// we're done with the extraction process, update depth level
				cei.ExtractionDepth--;

				// create and write database
				if (cei.Arguments.OutputDatabase && cei.ExtractionDepth == 0)
					ExtractWriteTagDatabases(cei, tm);

				Unload(handle);
			}
			else error = true;

			return !error;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cei"></param>
		/// <returns>True if any tags were extracted and if they extracted successfully. False if otherwise</returns>
		public bool Extract(Blam.CacheExtractionInfo cei)
		{
			if (!ExtractionDontExtract(cei, cei.Definition.Datum))
				return Extract(cei, cei.Definition.Datum, false);

			return false;
		}

		/// <summary>Extract an existing tag to a file</summary>
		/// <param name="tag_datum">index of tag to extract</param>
		/// <param name="base_directory">Root directory to extract the tag to</param>
		/// <returns>false if it can't save the tag</returns>
		public bool Extract(Blam.DatumIndex tag_datum, string base_directory) { return Extract(tag_datum, base_directory, null); }

		/// <summary>Extract an existing tag to a file</summary>
		/// <param name="tag_datum">index of tag to extract</param>
		/// <param name="base_directory">Root directory to extract the tag to</param>
		/// <param name="name_override">Optional, if not null, this is the name of the file we store the tag in</param>
		/// <returns>false if it can't save the tag</returns>
		public bool Extract(Blam.DatumIndex tag_datum, string base_directory, string name_override)
		{
			Blam.DatumIndex handle = Open(tag_datum);
			if (handle != Blam.DatumIndex.Null)
			{
				TagManager tm = Array[handle];
				try {
					tm.OpenForExtract(base_directory, name_override);
					tm.Extract();
				}
				catch (Exception ex) { extractionTrace.WriteLine("Couldn't extract tag: '{0}'{1}{2}", tm.Name, Program.NewLine, ex); return false; }
			}
			else return false;

			return true;
		}
		#endregion
	};
}