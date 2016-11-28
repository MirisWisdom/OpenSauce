/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Bitmaps
{
	/// <summary>
	/// Stolen and ported from SparkEdit...
	/// </summary>
	internal static class Decode
	{
		struct Color
		{
			public byte R, G, B, A;

			public static implicit operator uint(Color c)
			{
				return (uint)((c.A << 24) | (c.R << 16) | (c.G << 8) | c.B);
			}
		};

		static Color IntegerToColor(uint color)
		{
			Color rc;
			rc.R = (byte)((((color >> 11) & 31) * 255) / 31);
			rc.G = (byte)((((color >> 5) & 63) * 255) / 63);
			rc.B = (byte)((((color >> 0) & 31) * 255) / 31);
			rc.A = 255;
			return rc;
		}

		static int IntegerFromColor(Color c) { return (c.A << 24) | (c.R << 16) | (c.G << 8) | c.B; }

		static Color GradientColors(Color col1, Color col2)
		{
			Color ret;
			ret.R = (byte)(((col1.R * 2 + col2.R)) / 3);
			ret.G = (byte)(((col1.G * 2 + col2.G)) / 3);
			ret.B = (byte)(((col1.B * 2 + col2.B)) / 3);
			ret.A = 255;
			return ret;
		}

		static Color GradientColorsHalf(Color col1, Color col2)
		{
			Color ret;
			ret.R = (byte)(col1.R / 2 + col2.R / 2);
			ret.G = (byte)(col1.G / 2 + col2.G / 2);
			ret.B = (byte)(col1.B / 2 + col2.B / 2);
			ret.A = 255;
			return ret;
		}

		delegate bool Decoder(int height, int width, int depth, byte[] src_data, uint[] dest_data);

		static Decoder[] Functions = new Decoder[] {
			LinearA8,
			SwizzledA8,

			LinearY8,
			SwizzledY8,

			LinearAY8,
			SwizzledAY8,

			LinearA8Y8,
			SwizzledA8Y8,

			null,
			null,

			null,
			null,

			LinearR5G6B5,
			SwizzledR5G6B5,

			null,
			null,

			LinearA1R5G5B5,
			SwizzledA1R5G5B5,

			LinearA4R4G4B4,
			SwizzledA4R4G4B4,

			LinearX8R8G8B8,
			SwizzledX8R8G8B8,

			LinearA8R8G8B8,
			SwizzledA8R8G8B8,

			null,
			null,

			null,
			null,

			DXT1,
			DXT1,

			DXT3,
			DXT3,

			DXT5,
			DXT5,

			LinearP8Bump,
			SwizzledP8Bump,

			LinearP8,
			SwizzledP8,

			null, // argbfp32
			null,

			null, // rgbfp32
			null,

			null, // rgbfp16
			null,

			null, // v8u8
			null,

			null, // g8b8
			null,
		};

		public static byte[] Surface(int height, int width, int depth, byte[] src_data, bool is_linear)
		{
			return null;
		}

		static bool LinearA8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			for (int y = 0, index = 0; y < height; y++)
				for (int x = 0; x < width; x++, index = (y * width) + x)
					dest_data[index] = (uint)(src_data[index] << 24);

			return true;
		}

		static bool SwizzledA8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Swizzler swz = new Swizzler((uint)width, (uint)height);

			for (int y = 0; y < height; y++)
				for (int x = 0; x < width; x++)
					dest_data[(y * width) + x] = (uint)(src_data[swz.Swizzle((uint)x, (uint)y, (uint)depth)] << 24);

			return true;
		}

		static bool LinearY8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color color = new Color();

			int index = 0;
			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++, index = (y*width) + x)
				{
					color.R = color.G = color.B = src_data[index];
					dest_data[index] = color;
				}
			}

			return true;
		}

		static bool SwizzledY8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color color = new Color();
			Swizzler swz = new Swizzler((uint)width, (uint)height);

			int index = 0;
			for(int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++, index = (y * width) + x)
				{
					color.R = color.G = color.B = src_data[swz.Swizzle((uint)x, (uint)y, (uint)depth)];
					dest_data[index] = color;
				}
			}

			return true;
		}

		static bool LinearAY8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color color = new Color();

			int index = 0;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++, index = (y * width) + x)
				{
					color.A = color.R = color.G = color.B = src_data[index];
					dest_data[index] = color;
				}
			}

			return true;
		}

		static bool SwizzledAY8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color color = new Color();
			Swizzler swz = new Swizzler((uint)width, (uint)height);

			int index = 0;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++, index = (y * width) + x)
				{
					color.A = color.R = color.G = color.B = src_data[swz.Swizzle((uint)x, (uint)y, (uint)depth)];
					dest_data[index] = color;
				}
			}

			return true;
		}

		static bool LinearA8Y8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color color = new Color();

			int index = 0;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++, index = (y * width) + x)
				{
					color.R = color.G = color.B = src_data[index];
					color.A = src_data[index+1];
					dest_data[index] = color;
				}
			}
			return false;
		}

		static bool SwizzledA8Y8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool LinearR5G6B5(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledR5G6B5(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool LinearA1R5G5B5(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledA1R5G5B5(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool LinearA4R4G4B4(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledA4R4G4B4(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool LinearX8R8G8B8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledX8R8G8B8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool LinearA8R8G8B8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledA8R8G8B8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool DXT1(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color[] color = new Color[4];
			Color ccolor;
			Color zero_color = new Color();
			int dptr = 0;
			int cdata;
			int chunks_per_horiz_line = width / 4;
			uint c1, c2;
			bool trans;
			if (chunks_per_horiz_line == 0) chunks_per_horiz_line = 1;

			for (int i = 0; i <= (width * height) - 1; i += 16)
			{
				c1 = (Convert.ToUInt32(src_data[dptr + 1]) << 8) | (src_data[dptr]);
				c2 = (Convert.ToUInt32(src_data[dptr + 3]) << 8) | (src_data[dptr + 2]);

				if (c1 > c2) trans = false;
				else trans = true;

				color[0] = IntegerToColor(c1);
				color[1] = IntegerToColor(c2);

				if (!(trans))
				{
					color[2] = GradientColors(color[0], color[1]);
					color[3] = GradientColors(color[1], color[0]);
				}
				else
				{
					color[2] = GradientColorsHalf(color[0], color[1]);
					color[3] = zero_color;
				}

				cdata = (Convert.ToInt32(src_data[dptr + 4]) << 0) | (Convert.ToInt32(src_data[dptr + 5]) << 8) | (Convert.ToInt32(src_data[dptr + 6]) << 16) | (Convert.ToInt32(src_data[dptr + 7]) << 24);
				int chunk_index = i / 16;
				int xpos = chunk_index % chunks_per_horiz_line;
				int ypos = (chunk_index - xpos) / chunks_per_horiz_line;
				long ttmp;
				int sizeh = height < 4 ? height : 4;
				int sizew = width < 4 ? width : 4;
				for (int x = 0; x <= sizeh - 1; x++)
				{
					for (int y = 0; y <= sizew - 1; y++)
					{
						ccolor = color[cdata & 3];
						cdata >>= 2;
						ttmp = ((ypos * 4 + x) * width + xpos * 4 + y) * 4;
						dest_data[ttmp] = ccolor.B;
						dest_data[ttmp + 1] = ccolor.G;
						dest_data[ttmp + 2] = ccolor.R;
						dest_data[ttmp + 3] = ccolor.A;
					}
				}
				dptr += 8;
			}

			return true;
		}

		static bool DXT3(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color[] color = new Color[4];
			Color ccolor;
			int cdata;
			int chunks_per_horiz_line = width / 4;
			if (chunks_per_horiz_line == 0) chunks_per_horiz_line = 1;

			for (int i = 0; i <= (width * height) - 1; i += 16)
			{
				color[0] = IntegerToColor(Convert.ToUInt32(src_data[i + 8]) | Convert.ToUInt32(src_data[i + 9]) << 8);
				color[1] = IntegerToColor(Convert.ToUInt32(src_data[i + 10]) | Convert.ToUInt32(src_data[i + 11]) << 8);
				color[2] = GradientColors(color[0], color[1]);
				color[3] = GradientColors(color[1], color[0]);
				cdata = (Convert.ToInt32(src_data[i + 12]) << 0) | (Convert.ToInt32(src_data[i + 13]) << 8) | (Convert.ToInt32(src_data[i + 14]) << 16) | (Convert.ToInt32(src_data[i + 15]) << 24);
				int chunk_index = i / 16;
				long xpos = chunk_index % chunks_per_horiz_line;
				long ypos = (chunk_index - xpos) / chunks_per_horiz_line;
				long ttmp;
				int alpha;
				int sizeh = height < 4 ? height : 4;
				int sizew = width < 4 ? width : 4;

				for (int x = 0; x <= sizeh - 1; x++)
				{
					alpha = src_data[i + (2 * x)] | Convert.ToInt32(src_data[i + (2 * x) + 1]) << 8;
					for (int y = 0; y <= sizew - 1; y++)
					{
						ccolor = color[cdata & 3];
						cdata >>= 2;
						ccolor.A = (byte)((alpha & 15) * 16);
						alpha >>= 4;
						ttmp = ((ypos * 4 + x) * width + xpos * 4 + y) * 4;
						dest_data[ttmp] = ccolor.B;
						dest_data[ttmp + 1] = ccolor.G;
						dest_data[ttmp + 2] = ccolor.R;
						dest_data[ttmp + 3] = ccolor.A;
					}
				}
			}

			return true;
		}

		static bool DXT5(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			Color[] color = new Color[4];
			Color ccolor;
			int cdata;
			int chunks_per_horiz_line = width / 4;
			if (chunks_per_horiz_line == 0) chunks_per_horiz_line = 1;

			for (int i = 0; i <= (width * height) - 1; i += 16)
			{
				color[0] = IntegerToColor(Convert.ToUInt32(src_data[i + 8]) | Convert.ToUInt32(src_data[i + 9]) << 8);
				color[1] = IntegerToColor(Convert.ToUInt32(src_data[i + 10]) | Convert.ToUInt32(src_data[i + 11]) << 8);
				color[2] = GradientColors(color[0], color[1]);
				color[3] = GradientColors(color[1], color[0]);
				cdata = (Convert.ToInt32(src_data[i + 12]) << 0) | (Convert.ToInt32(src_data[i + 13]) << 8) | (Convert.ToInt32(src_data[i + 14]) << 16) | (Convert.ToInt32(src_data[i + 15]) << 24);
				byte[] alpha = new byte[8];
				alpha[0] = src_data[i];
				alpha[1] = src_data[i + 1];
				if ((alpha[0] > alpha[1]))
				{
					alpha[2] = (byte)((6 * alpha[0] + 1 * alpha[1] + 3) / 7);
					alpha[3] = (byte)((5 * alpha[0] + 2 * alpha[1] + 3) / 7);
					alpha[4] = (byte)((4 * alpha[0] + 3 * alpha[1] + 3) / 7);
					alpha[5] = (byte)((3 * alpha[0] + 4 * alpha[1] + 3) / 7);
					alpha[6] = (byte)((2 * alpha[0] + 5 * alpha[1] + 3) / 7);
					alpha[7] = (byte)((1 * alpha[0] + 6 * alpha[1] + 3) / 7);
				}
				else
				{
					alpha[2] = (byte)((4 * alpha[0] + 1 * alpha[1] + 2) / 5);
					alpha[3] = (byte)((3 * alpha[0] + 2 * alpha[1] + 2) / 5);
					alpha[4] = (byte)((2 * alpha[0] + 3 * alpha[1] + 2) / 5);
					alpha[5] = (byte)((1 * alpha[0] + 4 * alpha[1] + 2) / 5);
					alpha[6] = 0;
					alpha[7] = 255;
				}
				long tmpdword;
				int tmpword;
				long alphaDat;
				tmpword = src_data[i + 2] | (Convert.ToInt32(src_data[i + 3]) << 8);
				tmpdword = src_data[i + 4] | (Convert.ToInt32(src_data[i + 5]) << 8) | (src_data[i + 6] << 16) | (Convert.ToInt32(src_data[i + 7]) << 24);
				alphaDat = tmpword | (int)(tmpdword << 16);
				int chunkNum = i / 16;
				long xpos = chunkNum % chunks_per_horiz_line;
				long ypos = (chunkNum - xpos) / chunks_per_horiz_line;
				long temp;
				int sizeh = height < 4 ? height : 4;
				int sizew = width < 4 ? width : 4;
				for (int x = 0; x <= sizeh - 1; x++)
				{
					for (int y = 0; y <= sizew - 1; y++)
					{
						ccolor = color[cdata & 3];
						cdata >>= 2;
						ccolor.A = alpha[alphaDat & 7];
						alphaDat >>= 3;
						temp = ((ypos * 4 + x) * width + xpos * 4 + y) * 4;
						dest_data[temp] = ccolor.B;
						dest_data[temp + 1] = ccolor.G;
						dest_data[temp + 2] = ccolor.R;
						dest_data[temp + 3] = ccolor.A;
					}
				}
			}

			return true;
		}

		static bool LinearP8Bump(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledP8Bump(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool LinearP8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}

		static bool SwizzledP8(int height, int width, int depth, byte[] src_data, uint[] dest_data)
		{
			return false;
		}
	};
}