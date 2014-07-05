using BlamLib.Render.COLLADA;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenSauceIDE.ModelExtractor.MessageHandler;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
    /// <summary>   Interface for a model extractor. </summary>
    public interface IExtractor
    {
        string TagTypeName { get; }
        string TagExtension { get; }

        void Initialise();
        void Dispose();

        System.Windows.Forms.Control GetFormControl();
        IExtractionJob CreateJob(IColladaSettings colladaSettings
            , string tagsDirectory
            , string tagPath
            , string outputFile);
    }
}
