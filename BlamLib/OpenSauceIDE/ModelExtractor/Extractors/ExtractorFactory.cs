/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using BlamLib;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>   An extractor factory. </summary>
	public partial class ExtractorFactory
		: IExtractorFactory
	{
		/// <summary>   Struct for containing extractor type information. </summary>
		private class ExtractorEntry
		{
			public string mFileType;
			public string mFileExtension;
			public Type mExtractorType;
			public ConstructorInfo mExtractorJobConstructor;
			public List<ExtractorEntry> mChildExtractors;
		}

		private Dictionary<string, ExtractorEntry> mExtractorDictionary = new Dictionary<string, ExtractorEntry>();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds an extraction type. </summary>
		///
		/// <param name="fileType">		  	Type of the file. </param>
		/// <param name="extension">	  	The files extension. </param>
		/// <param name="extractorType">	The extractor type. </param>
		/// <param name="jobType">		  	Type of the job. </param>
		/// <param name="childExtractors">
		/// 	A variable-length parameters list containing child extractors.
		/// </param>
		///
		/// <returns>	An ExtractorEntry. </returns>
		private ExtractorEntry AddType(string fileType
			, string extension
			, Type extractorType
			, Type jobType
			, params ExtractorEntry[] childExtractors)
		{
			if (mExtractorDictionary.ContainsKey(extension))
			{
				throw new ArgumentException("Attempted to add duplicate extractor entries");
			}

			if (!typeof(IExtractor).IsAssignableFrom(extractorType))
			{
				throw new ArgumentException("Attempted to add an extractor type that does not implement IExtractor");
			}

			if (!typeof(IExtractionJob).IsAssignableFrom(jobType))
			{
				throw new ArgumentException("Attempted to add an extractor job that does not implement IExtractionJob");
			}

			var jobConstructor = jobType.GetConstructor(new Type[] { typeof(IExtractionData) });
			if (jobConstructor == null)
			{
				throw new ArgumentException("Attempted to add an extractor job that does have a constructor with a single IExtractionData parameter");
			}

			var entry = new ExtractorEntry()
			{
				mFileType = fileType,
				mFileExtension = extension,
				mExtractorType = extractorType,
				mExtractorJobConstructor = jobConstructor,
				mChildExtractors = new List<ExtractorEntry>(childExtractors)
			};

			mExtractorDictionary.Add(extension, entry);

			return entry;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Registers extractors made for gameVersion. </summary>
		///
		/// <param name="gameVersion">  The game version. </param>
		public void RegisterExtractors(BlamVersion gameVersion)
		{
			switch (gameVersion)
			{
				case BlamVersion.Halo1_CE:
					AddTypesHalo1();
					break;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the extractors for a type according to the provided extension. </summary>
		///
		/// <exception cref="KeyNotFoundException">
		/// 	Thrown when no matching extractor can be found.
		/// </exception>
		///
		/// <param name="extension">	The extension. </param>
		///
		/// <returns>	The extensions extractors. </returns>
		public IEnumerable<IExtractor> GetExtractors(string extension)
		{
			var trimmedExtension = extension.TrimStart('.');
			if (!mExtractorDictionary.ContainsKey(trimmedExtension))
			{
				throw new KeyNotFoundException("Matching extractor extension not found");
			}

			var extractorTypeList = new List<Type>();
			var extractors = new List<ExtractorEntry>();
			extractors.Add(mExtractorDictionary[trimmedExtension]);

			Action<ExtractorEntry> getExtractorTypes =
				(entry) =>
				{
					if (!extractorTypeList.Contains(entry.mExtractorType))
					{
						extractorTypeList.Add(entry.mExtractorType);
					}

					foreach(var childEntry in entry.mChildExtractors)
					{
						if (!extractorTypeList.Contains(childEntry.mExtractorType))
						{
							extractors.Add(childEntry);
						}
					}
				};

			// Get all extractor children non-recursively
			int currentCount = 1;
			while (currentCount != 0)
			{
				for(int i = 0; i < currentCount; ++i)
				{
					getExtractorTypes(extractors[i]);
				}

				extractors.RemoveRange(0, currentCount);
				currentCount = extractors.Count;
			}

			// Return instances of the extractors
			foreach (var extractorType in extractorTypeList)
			{
				yield return Activator.CreateInstance(extractorType) as IExtractor;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets an extraction job. </summary>
		///
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="extension">	 	The files extension. </param>
		/// <param name="extractionData">	Information describing the extraction. </param>
		///
		/// <returns>	The extraction job. </returns>
		public IExtractionJob GetExtractionJob(string extension, IExtractionData extractionData)
		{
			var trimmedExtension = extension.TrimStart('.');
			if (!mExtractorDictionary.ContainsKey(trimmedExtension))
			{
				throw new ArgumentException("Attempted to get a non existant extractor job");
			}

			return mExtractorDictionary[trimmedExtension].mExtractorJobConstructor.Invoke(new object[] { extractionData }) as IExtractionJob;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the file types in the factory. </summary>
		///
		/// <returns>
		///     An enumerator that allows foreach to be used to process the file types in this factory.
		/// </returns>
		public IEnumerable<ExtractorFileType> GetFileTypes()
		{
			foreach(var entry in mExtractorDictionary)
			{
				yield return new ExtractorFileType
				{
					TypeName = entry.Value.mFileType,
					TypeExtension = entry.Value.mFileExtension,
					TypeFilter = String.Format("{0} (*.{1})|*.{1}", entry.Value.mFileType, entry.Value.mFileExtension)
				};
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets file type information. </summary>
		///
		/// <exception cref="ArgumentException">
		/// 	Thrown when one or more arguments have unsupported or illegal values.
		/// </exception>
		///
		/// <param name="extension">	The extension to get info for. </param>
		///
		/// <returns>	The file type information. </returns>
		public ExtractorFileType GetFileType(string extension)
		{
			var trimmedExtension = extension.TrimStart('.');
			if (!mExtractorDictionary.ContainsKey(trimmedExtension))
			{
				throw new ArgumentException("Attempted to fine type info for a non existant extension");
			}

			return new ExtractorFileType
			{
				TypeName = mExtractorDictionary[extension].mFileType,
				TypeExtension = mExtractorDictionary[extension].mFileExtension,
				TypeFilter = String.Format("{0} (*.{1})|*.{1}", mExtractorDictionary[extension].mFileType, mExtractorDictionary[extension].mFileExtension)
			};
		}
	}
}
