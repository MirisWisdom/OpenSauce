using System;
using BlamLib.Blam;

namespace BlamLib.Render.COLLADA.Halo2
{
	public interface IHalo2ShaderDatumList
	{
		int GetShaderCount();
		DatumIndex GetShaderDatum(int index);
	}

	public interface IHalo2LightmapInterface : IHalo2ShaderDatumList
	{

	}
}
