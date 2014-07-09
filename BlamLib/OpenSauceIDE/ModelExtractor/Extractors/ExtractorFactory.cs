using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using BlamLib;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
    public class ModelExtractorAttribute : Attribute
    {
        public BlamVersion GameVersion { get; set; }
        public string TagName { get; set; }
        public string TagExtension { get; set; }

        public ModelExtractorAttribute(BlamVersion gameVersion, string tagName, string tagExtension)
        {
            GameVersion = gameVersion;
            TagName = tagName;
            TagExtension = tagExtension;
        }
    }

    /// <summary>   Interface for an extractor factory. </summary>
    public interface IExtractorFactory
    {
        IExtractor GetExtractor(string extension);
        void RegisterExtractors(BlamVersion gameVersion);
        IEnumerable<Tuple<string, string>> GetFileTypes();
    }

    /// <summary>   An extractor factory. </summary>
    public class ExtractorFactory
        : IExtractorFactory
    {
        /// <summary>   Struct for containing extractor type information. </summary>
        private struct ExtractorEntry
        {
            public string mFileType;
            public string mFileExtension;
            public Type mExtractorType;
        }

        private Dictionary<string, ExtractorEntry> mExtractorDictionary = new Dictionary<string, ExtractorEntry>();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets an extractor according to the provided extension. </summary>
        ///
        /// <exception cref="KeyNotFoundException"> Thrown when no matching extractor can be found. </exception>
        ///
        /// <param name="extension">    The extension. </param>
        ///
        /// <returns>   The extensions extractor. </returns>
        public IExtractor GetExtractor(string extension)
        {
            if(!mExtractorDictionary.ContainsKey(extension))
            {
                throw new KeyNotFoundException("Matching extractor extension not found");
            }

            return Activator.CreateInstance(mExtractorDictionary[extension].mExtractorType) as IExtractor;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Registers extractors made for gameVersion. </summary>
        ///
        /// <param name="gameVersion">  The game version. </param>
        public void RegisterExtractors(BlamVersion gameVersion)
        {
            var assembly = Assembly.GetExecutingAssembly();

            // Get all types that have ModelExtractorAttribute defined and have a matching game version
            var extractorTypes = assembly.GetTypes()
                .Where(
                    type =>
                    {
                        if (!type.IsDefined(typeof(ModelExtractorAttribute), false))
                        {
                            return false;
                        }

                        var attribute = type.GetCustomAttribute<ModelExtractorAttribute>();
                        return attribute.GameVersion.HasFlag(gameVersion);
                    }
            );

            foreach(var type in extractorTypes)
            {
                var attribute = type.GetCustomAttribute<ModelExtractorAttribute>();

                Register(attribute.TagName, attribute.TagExtension, type);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Registers an extractor with the factory. </summary>
        ///
        /// <exception cref="ArgumentException">
        ///     Thrown when one or more arguments have unsupported or illegal values.
        /// </exception>
        ///
        /// <param name="type">             The type of model. </param>
        /// <param name="extension">        The extension for the model. </param>
        /// <param name="extractorType">    Type of the extractor. </param>
        public void Register(string type, string extension, Type extractorType)
        {
            if(mExtractorDictionary.ContainsKey(extension))
            {
                throw new ArgumentException("Attempted to add duplicate extractor entries");
            }

            if (!typeof(IExtractor).IsAssignableFrom(extractorType))
            {
                throw new ArgumentException("Attempted to add an extractor that does not implement IExtractor");
            }

            mExtractorDictionary.Add(extension, new ExtractorEntry { mFileType = type, mFileExtension = extension, mExtractorType = extractorType });
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets the file types in the factory. </summary>
        ///
        /// <returns>
        ///     An enumerator that allows foreach to be used to process the file types in this factory.
        /// </returns>
        public IEnumerable<Tuple<string, string>> GetFileTypes()
        {
            foreach(var entry in mExtractorDictionary)
            {
                yield return new Tuple<string, string>(entry.Value.mFileType, entry.Value.mFileExtension);
            }
        }
    }
}
