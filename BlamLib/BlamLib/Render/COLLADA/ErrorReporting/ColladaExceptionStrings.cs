/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Text;

namespace BlamLib.Render.COLLADA
{
	static class ColladaExceptionStrings
	{
		public static readonly string InvalidArrayIndex =
			"COLLADA_EXPORTER: invalid array index : {0} : {1}";
		public static readonly string InvalidDatumIndex =
			"COLLADA_EXPORTER: invalid datum index : {0}";
		public static readonly string ModelBoneCountZero =
			"COLLADA_EXPORTER: model bone count is zero : {0}";
		public static readonly string ValidationFailed =
			"COLLADA_EXPORTER: validation failed";
		public static readonly string FileExists =
			"COLLADA_EXPORTER: file exists and overwriting is disabled [{0}]";
		/// <summary>
		/// Used when a tag is being cast to an incompatible type
		/// </summary>
		public static readonly string InvalidDefinitionCast = "COLLADA_BUILDER: invalid tag definition cast\nTag name: {0}\nFrom: {1}\tTo: {2}";
		/// <summary>
		/// Used when unexpected values are used due to incorrect usage
		/// </summary>
		public static readonly string ImplimentationBug = "COLLADA_BUILDER: {0}";
	}
}