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