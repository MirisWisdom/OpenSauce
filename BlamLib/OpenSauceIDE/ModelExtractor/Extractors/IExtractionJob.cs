using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenSauceIDE.ModelExtractor.MessageHandler;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
    /// <summary>   Interface for an extraction job. </summary>
    public interface IExtractionJob
        : IMessageSource
    {
        void Execute();
    }
}
