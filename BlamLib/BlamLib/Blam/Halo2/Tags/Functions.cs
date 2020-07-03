/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	// TODO: figure out upgrading code to properly extract h2 alpha tags
	#region mapping_function
	//Mapping
	[TI.Struct((int)StructGroups.Enumerated.MAPP, 2, 12)]
	public class mapping_function : TI.Definition
	{
		#region real_block
		[TI.Definition(1, 4)]
		public class real_block : TI.Definition
		{
			#region Fields
			public TI.Real Value;
			#endregion

			#region Ctor
			public real_block() : base(1)
			{
				Add(Value = new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<byte_block> Data;
		#endregion

		#region Ctor
		mapping_function(int field_count) : base(field_count) {}

		[TI.VersionCtorHalo2(1, 32, BlamVersion.Halo2_Alpha)]
		public mapping_function(int major, int minor) : base(12)
		{
			if (major == 1)
			{
				switch (minor)
				{
					case 32:
						this.RemoveAt(this.Count - 1); // Data

						Add(/*Function Type = */ new TI.Enum(TI.FieldType.ByteEnum));
						Add(/*Flags = */ new TI.Flags(TI.FieldType.ByteFlags));
						Add(/*Function 1 = */ new TI.ByteInteger());
						Add(/*Function 2 = */ new TI.ByteInteger());
						Add(/*Color 0 = */ new TI.Color(TI.FieldType.RgbColor));
						Add(/*Color 1 = */ new TI.Color(TI.FieldType.RgbColor));
						Add(/*Color 2 = */ new TI.Color(TI.FieldType.RgbColor));
						Add(/*Color 3 = */ new TI.Color(TI.FieldType.RgbColor));
						Add(/*Values = */ new TI.Block<real_block>(this, 64));
						break;
				}
			}
		}

		internal override bool Upgrade()
		{
			TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
			if(attr.Major == 1)
			{
				switch (attr.Minor)
				{
					case 32:
						// TODO: do this
						break;
				}
			}
			return true;
		}

		public mapping_function() : this(1)
		{
			Add(Data = new TI.Block<byte_block>(this, 1024));
		}
		#endregion
	}
	#endregion

	#region color_function_struct
	[TI.Struct((int)StructGroups.Enumerated.CLFN, 1, 12)]
	public class color_function_struct : TI.Definition
	{
		#region Fields
		public TI.Struct<mapping_function> Function;
		#endregion

		#region Ctor
		public color_function_struct() : base(1)
		{
			Add(Function = new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region scalar_function_struct
	[TI.Struct((int)StructGroups.Enumerated.SCFN, 1, 12)]
	public class scalar_function_struct : TI.Definition
	{
		#region Fields
		public TI.Struct<mapping_function> Function;
		#endregion

		#region Ctor
		public scalar_function_struct() : base(1)
		{
			Add(Function = new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	public class Function
	{
		public enum Type
		{
							// header size?
			Identity,		// 0
			Constant,		// 0
			Transition,		// 0
			Periodic,		// 0
			Linear,			// 2
			LinearKey,		// 4
			MultiLinearKey,	// 16
			Spline,			// 4
			MultiSpline,	// 16
			Exponent,		// 0
			Spline2,		// 4
		};
		static readonly int[] TypeSizeTable = {
			0,
			1,
			2,
			4,
			6,
			20,
			32,
			12,
			4,
			3,
			12,
		};

		[Flags]
		public enum Flags
		{
			Range,
			Unused1,
			Unused2,
			Unused3,
			Color0, // 0, 0, 0, 0
			Color1, // 0, 0, 0, 0
			Color2, // 0, 3, 0, 0
			Color3, // 0, 1, 3, 0
					// 0, 1, 2, 3
		}

		public enum OutputType
		{
			Scalar,
			Constant,
			_2Color,
			_3Color,
			_4Color,
		};

		public enum ExponentTransition
		{
			Linear,
			Early,
			VeryEarly,
			Late,
			VeryLate,
			Cosine,
			One,
			Zero,
		};

		public enum ExponentPeriodic
		{
			One,
			Zero,
			CosineVariablePeriod,
			DiagonalWave,
			DiagonalWaveVariablePeriod,
			Slide,
			SlideVariablePeriod,
			Noise,
			Jitter,
			Wander,
			Spark,
		};

		public static int GetDataSize(Type t)
		{
			return (TypeSizeTable[(int)t] * 8) + 20;
		}
	};
}