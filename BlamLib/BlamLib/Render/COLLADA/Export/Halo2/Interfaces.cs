/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Render.COLLADA.Halo2
{
	public interface IHalo2RenderModelInterface : IHaloShaderDatumList
	{
		int GetGeometryCount();
		string GetGeometryName(int index);
		int GetGeometryIndex(int index);
		bool GetIsMultiplePerms();
		int GetPermutation();
	};

	public interface IHalo2LightmapInterface : IHaloShaderDatumList
	{
	};
	public interface IHalo2BSPInterface : IHaloShaderDatumList
	{
		bool IncludeRenderMesh();
		bool IncludePortalsMesh();
		bool IncludeFogPlanesMesh();
	};
}