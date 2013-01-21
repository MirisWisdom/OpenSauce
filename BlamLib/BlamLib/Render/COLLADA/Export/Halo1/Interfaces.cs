/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Render.COLLADA.Halo1
{
	public interface IHalo1ModelInterface : IHaloShaderDatumList
	{
		int GetGeometryCount();
		string GetGeometryName(int index);
		int GetGeometryIndex(int index);
		bool GetIsMultiplePerms();
		int GetPermutation();
	};

	public interface IHalo1BSPInterface : IHaloShaderDatumList
	{
		bool IncludeRenderMesh();
		bool IncludePortalsMesh();
		bool IncludeFogPlanesMesh();
	};
}