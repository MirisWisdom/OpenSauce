/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;

namespace BlamLib.IO
{
	// http://www.fileformat.info/format/material/
	public class WaveMaterialLibWriter : WaveFrontWriterBase
	{
		public WaveMaterialLibWriter(string path, string name) : base(path, name, "mtl")
		{
		}

		public void BeginNewMaterial(string name)
		{
			name = name.Replace(' ', '_');

			m_stream.WriteLine("newmtl {0}", name);
		}

		public void WriteColor(float r, float g, float b)	{ WriteTri("Ka", r, g, b); }

		public void WriteDiffuse(float r, float g, float b)	{ WriteTri("Kd", r, g, b); }

		public void WriteSpecular(float r, float g, float b){ WriteTri("Ks", r, g, b); }

		public void WriteSpecularExponent(float v)			{ WriteMono("Ka", v); }

		public void WriteTextureMap(string filename)		{ m_stream.WriteLine("map_Ka {0}", filename); }
	};
}