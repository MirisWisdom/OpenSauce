/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region mapping_function
	//Mapping
	[TI.Struct((int)StructGroups.Enumerated.MAPP, 1, 20)]
	public class mapping_function : TI.Definition
	{
		#region Fields
		public TI.Data Data;
		#endregion

		#region Ctor
		public mapping_function()
		{
			Add(Data = new TI.Data(this));
		}
		#endregion
	}
	#endregion

	public class Function
	{
		public enum Type
		{
// 			Identity,
// 			Constant,
// 			Transition,
// 			Periodic,
// 			Linear,
// 			LinearKey,
// 			MultiLinearKey,
// 			Spline,
// 			MultiSpline,
// 			Exponent,
// 			Spline2,
		};
		static readonly int[] TypeSizeTable = {
			0,
// 			1,
// 			2,
// 			4,
// 			6,
// 			20,
// 			32,
// 			12,
// 			4,
// 			3,
// 			12,
		};

		public enum OutputType
		{
// 			Scalar,
// 			Constant,
// 			_2Color,
// 			_3Color,
// 			_4Color,
		};

		public enum ExponentTransition
		{
// 			Linear,
// 			Early,
// 			VeryEarly,
// 			Late,
// 			VeryLate,
// 			Cosine,
// 			One,
// 			Zero,
		};

		public enum ExponentPeriodic
		{
// 			One,
// 			Zero,
// 			CosineVariablePeriod,
// 			DiagonalWave,
// 			DiagonalWaveVariablePeriod,
// 			Slide,
// 			SlideVariablePeriod,
// 			Noise,
// 			Jitter,
// 			Wander,
// 			Spark,
		};

		public static int GetDataSize(Type t)
		{
			return (TypeSizeTable[(int)t] * 8) + 20;
		}
	};
}