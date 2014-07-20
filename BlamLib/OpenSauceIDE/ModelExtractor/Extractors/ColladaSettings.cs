/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Render.COLLADA;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>   A collada settings implementation. </summary>
	public class ColladaSettings
		: IColladaSettings
		, ICloneable
	{
		public bool Overwrite { get; set; }
		public string RootDirectory { get; set; }
		public string BitmapExtension { get; set; }

		public object Clone()
		{
			return MemberwiseClone();
		}
	}
}
