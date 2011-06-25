/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
