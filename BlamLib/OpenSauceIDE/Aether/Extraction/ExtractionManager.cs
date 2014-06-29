using OpenSauceIDE.Aether.SceneObject;
using System;
using System.Collections.Generic;

namespace OpenSauceIDE.Aether.Extraction
{
	/// <summary>	Interface for an extractable object. </summary>
	public interface IObjectExtractable
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the extraction is enabled. </summary>
		///
		/// <value>	true if enable extraction, false if not. </value>
		bool EnableExtraction { get; set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Extracts this object. </summary>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool Extract();
	}

	public class ExtractionManager
	{
		#region Events
		public enum ExtractionResultEnum
		{
			InProgress,
			Succeeded,
			Failed
		}

		public class ExtractionEventArgs : EventArgs
		{
			public string JobID { get; private set; }
			public ExtractionResultEnum ExtractionResult { get; private set; }

			public ExtractionEventArgs(string id, ExtractionResultEnum result)
			{
				JobID = id;
				ExtractionResult = result;
			}
		}

		public event EventHandler<ExtractionEventArgs> ExtractionProgressed;

		private void OnExtractionProgressed(string id, ExtractionResultEnum result)
		{
			var handler = ExtractionProgressed;

			if(handler != null)
			{
				handler(this, new ExtractionEventArgs(id, result));
			}
		}
		#endregion

		#region Extraction
		private struct ExtractionJob
		{
			public string ID;
			public IObjectExtractable ExtractableObject;
		}

		private List<ExtractionJob> mExtractionJobs = new List<ExtractionJob>();

		public void RegisterExtractionJob(IObjectExtractable extractableObject, string id)
		{
			mExtractionJobs.Add(new ExtractionJob { ID = id, ExtractableObject = extractableObject });
		}

		public void RunCurrentJobs()
		{
			foreach(var job in mExtractionJobs)
			{
				if(!job.ExtractableObject.EnableExtraction)
				{
					continue;
				}

				OnExtractionProgressed(job.ID, ExtractionResultEnum.InProgress);
				if(job.ExtractableObject.Extract())
				{
					OnExtractionProgressed(job.ID, ExtractionResultEnum.Succeeded);
				}
				else
				{
					OnExtractionProgressed(job.ID, ExtractionResultEnum.Failed);
				}
			}

			mExtractionJobs.Clear();
		}
		#endregion
	}
}
