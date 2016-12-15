/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Bitmaps
{
	internal sealed class Swizzler
	{
		uint MaskX, MaskY, MaskZ;

		public Swizzler(uint Width, uint Height) : this(Width, Height, 0) {}
		public Swizzler(uint Width, uint Height, uint Depth)
		{
			MaskX = MaskY = MaskZ = 0;

			uint Idx = 1;
			for (int Bit = 1; Bit < Width || Bit < Height || Bit < Depth; Bit <<= 1 /*mul 2*/)
			{
				if (Bit < Width)
				{
					MaskX |= Idx;
					Idx <<= 1;
				}

				if (Bit < Height)
				{
					MaskY |= Idx;
					Idx <<= 1;
				}

				if (Bit < Depth)
				{
					MaskZ |= Idx;
					Idx <<= 1;
				}
			}
		}

		public uint Swizzle(uint Sx, uint Sy) { return Swizzle(Sx, Sy, uint.MaxValue); }
		public uint Swizzle(uint Sx, uint Sy, uint Sz)
		{
			return
				SwizzleAxis(Sx, MaskX) |
				SwizzleAxis(Sy, MaskY) |
				(
					(Sz != uint.MaxValue)
					?
						SwizzleAxis(Sz, MaskZ)
					:
						0
				);
		}

		public uint SwizzleAxis(uint Value, uint Mask)
		{
			uint Result = 0;

			for(uint Bit = 1; Bit <= Mask; Bit <<= 1 /*mul 2*/)
			{
				if ((Mask & Bit) != 0)
					Result |= (Value & Bit);
				else
					Value <<= 1; /*mul 2*/
			}

			return Result;
		}
	};
}