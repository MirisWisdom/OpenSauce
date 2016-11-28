/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Managers;

namespace BlamLib.Render.COLLADA.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Base class for Halo1 exporters to derive from. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ColladaExporterHalo1 : ColladaExporter
	{
		#region Class Members
		protected static readonly LowLevel.Math.real_matrix3x3 mTransformMatrix = new LowLevel.Math.real_matrix3x3(
			new LowLevel.Math.real_vector3d(1, 0, 0),
			new LowLevel.Math.real_vector3d(0, 1, 0),
			new LowLevel.Math.real_vector3d(0, 0, 1)
		);
		#endregion

		#region Fields
		protected TagIndexBase mTagIndex;
		#endregion Fields

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Base class constructor. </summary>
		/// <param name="arguments">	The collada config variables. </param>
		/// <param name="tagIndex"> 	Halo1 tag index. </param>
		///-------------------------------------------------------------------------------------------------
		public ColladaExporterHalo1(IColladaSettings settings, TagIndexBase tagIndex)
			: base(settings)
		{
			mTagIndex = tagIndex;
		}
		#endregion Constructor
	};
}