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
﻿using System;

namespace BlamLib
{
	partial class Util
	{
		/// <summary>
		/// Takes <paramref name="value"/> and returns what it would
		/// be if it were aligned to <paramref name="align_size"/> bytes
		/// </summary>
		/// <param name="align_size">Alignment size in bytes</param>
		/// <param name="value">Value to align</param>
		/// <returns>Alignment of <paramref name="value"/></returns>
		public static uint Align(uint align_size, uint value) { return (value + (align_size - 1)) & ~(align_size - 1); }

		#region Real Math
		public static short CompressReal(float value)
		{
			if(value > 1.0F) value = 1.0F;
			if(value < -1.0F)value = -1.0F;

			return (short)( (int)Math.Floor(value * 32767.5F) );
		}

		public static float DecompressReal(short value)
		{
			return (
						((float)(value + value)) + 1
					) / 65535.0F;
		}

		public static float Int16ToReal(short value, float min, float max)
		{
			return (
						(
							((float)value + 32768) / 65535
						) * (max - min)
					) + min;
		}

		public static short RealToInt16(float value, float min, float max)
		{
			return (short)(ushort)((
						(
							(
								value - min
							) / (max - min)
						) * 65535
					) - (ushort)32768);
		}

		public static float UInt16ToReal(ushort value, float min, float max)
		{
			return ((value / 65535) * (max - min)) + min;
		}

		public static float Int32ToReal(int value, float min, float max)
		{
			return (float)((value + 2.147484E+9F) / 4294967300.0 * (max - min) + min);
		}
		#endregion
	};
}