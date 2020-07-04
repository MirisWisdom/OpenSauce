/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

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