/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA
{
	public interface IColladaDataProvider { };
	public interface IColladaExternalReference
	{
		string GetRelativeURL();
	};

	public interface IHaloShaderDatumList
	{
		int GetShaderCount();
		Blam.DatumIndex GetShaderDatum(int index);
		string GetShaderName(int index);
	};
}