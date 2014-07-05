using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Render.COLLADA;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
    /// <summary>   A collada settings implementation. </summary>
    public class ColladaSettings
        : IColladaSettings
    {
        public bool Overwrite { get; set; }
        public string RootDirectory { get; set; }
        public string BitmapExtension { get; set; }
    }
}
