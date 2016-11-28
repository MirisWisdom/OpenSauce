/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Diagnostics;   // debugger display
using System.IO;
using System.Threading;

namespace YeloDebug
{
	public partial class Xbox : IDisposable
	{
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		byte[] FrameBuffer = new byte[64 + 640 * 480 * 4];
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		byte[] BitmapBuffer = new byte[56 + 640 * 480 * 4];


		public SurfaceInformation GetSurfaceInformation(Surface surface)
		{
			SurfaceInformation si = new SurfaceInformation();
			StatusResponse response = SendCommand("getsurf id={0}", (int)surface);
			var info = Util.ExtractResponseInformation(response.Message);
			si.Size = Convert.ToUInt32(info[0]);
			si.Format = Convert.ToUInt32(info[1]);
			si.Address = Convert.ToUInt32(info[2]);
			si.PushBufferPut = Convert.ToUInt32(info[3]);
			return si;
		}

		/// <summary>
		/// Pauses the game and returns an address to the current framebuffer once it's locked.
		/// </summary>
		/// <returns></returns>
		public uint LockGPU()
		{
			Pause();
			DateTime before = DateTime.Now;

			ulong ptrs = GetUInt64(0xB0033DA0);
			uint ptr1 = (uint)(ptrs >> 32);
			uint ptr2 = (uint)(ptrs & 0xFFFFFFFF);

			// waits until present queue is empty (flipcounter == framecounter)
			while (GetUInt32(ptr1) != GetUInt32(ptr2))
			{
				TimeSpan elapse = DateTime.Now - before;
				if (elapse.TotalMilliseconds > Timeout)
					throw new TimeoutException();
				Thread.Sleep(SleepTime);
			}
			return GetUInt32(0xFD600800) | 0x80000000;
		}

		/// <summary>
		/// Takes a screenshot of the xbox display.
		/// </summary>
		public System.Drawing.Image Screenshot()
		{
			int read = 0;
			MemoryStream.Read(LockGPU(), FrameBuffer.Length, FrameBuffer, 0, ref read);
			Continue();
			int[] block = {
							  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
							  18, 19, 16, 17, 22, 23, 20, 21, 26, 27, 24, 25, 30, 31, 28, 29,
							  33, 34, 35, 32, 37, 38, 39, 36, 41, 42, 43, 40, 45, 46, 47, 44,
							  51, 48, 49, 50, 55, 52, 53, 54, 59, 56, 57, 58, 63, 60, 61, 62
						  };
			int swiz = 0, index = 0, offset = 0;
			int deswiz = 1226240;
			int i, j, k, l;
			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if ((i & 1) == 1)
						if ((j & 1) == 1) deswiz -= 256;
						else deswiz += 256;

					for (l = 0; l < 4; l++)
					{
						for (k = 0; k < 16; k++)
						{
							offset = (((int)(block[index] & 0xFFFFFFFE) >> 2) * 256) + ((block[index] & 3) * 16);
							index = (index + 1) & 63;
							for (int v = 0; v < 15; v++)
								if ((v & 3) != 3)
								{
									BitmapBuffer[deswiz + v] = FrameBuffer[swiz + offset + v];
									BitmapBuffer[deswiz - 2560 + v] = FrameBuffer[swiz + offset + 64 + v];
									BitmapBuffer[deswiz - 5120 + v] = FrameBuffer[swiz + offset + 128 + v];
									BitmapBuffer[deswiz - 7680 + v] = FrameBuffer[swiz + offset + 192 + v];
								}
							deswiz += 16;
						}
						deswiz -= 10496;
					}
					deswiz += 41216;
					swiz += 4096;

					if ((i & 1) == 1)
						if ((j & 1) == 1) deswiz += 256;
						else deswiz -= 256;
				}
				deswiz -= 43520;
			}

			using (System.IO.MemoryStream ms = new System.IO.MemoryStream())
			using (BinaryWriter bw = new BinaryWriter(ms))
			{
				// BITMAPFILEHEADER
				bw.Write(new char[] { 'B', 'M' }); // ushort bfType - BM (19778)
				bw.Write((uint)824); // uint bfSize - Size of the file (bytes) - Header + ImageData + (ushort)0
				bw.Write((ushort)0); // ushort bfReserved1 - Zero
				bw.Write((ushort)0); // ushort bfReserved2 - Zero
				bw.Write((uint)54); // uint bfOffBits - Offset to image data
				// BITMAPINFOHEADER
				bw.Write((uint)40); // uint biSize - Size of BITMAPINFOHEADER (bytes)
				bw.Write((uint)640); // uint biWidth - Width of image (pixels)
				bw.Write((uint)480); // uint biHeight - Height of image (pixels)
				bw.Write((ushort)1); // ushort biPlanes - Number of planes of the target device (usually one)
				bw.Write((ushort)24); // ushort biBitCount - Bits per pixel (1=black/white, 4=16 colors, 8=256 colors, 24=16.7 million colors)
				bw.Write((uint)0); // uint biCompression - Type of compression (0=None)
				bw.Write((uint)770); // uint biSizeImage - Size of image data (bytes) - Zero if no compression
				bw.Write((uint)2834); // uint biXPelsPerMeter - Hoizontal pixels per meter (usually zero) (2834=72 Pixels Per Inch)
				bw.Write((uint)2834); // uint biYPelsPerMeter - Vertical pixels per meter (usually zero) (2834=72 Pixels Per Inch)
				bw.Write((uint)0); // uint biClrUsed - Number of colors used - If zero, calculated by biBitCount
				bw.Write((uint)0); // uint biClrImportant - Number of "important" colors (0=All)

				for (int off = 0; off < 1228800; off += 2560)
				{
					for (int i2 = 0; i2 < 2560; i2 += 4)
					{
						bw.Write(BitmapBuffer[off + i2]);
						bw.Write(BitmapBuffer[off + i2 + 1]);
						bw.Write(BitmapBuffer[off + i2 + 2]);
					}
				}
				bw.Write((ushort)0);
				bw.Flush();

				return System.Drawing.Bitmap.FromStream(ms);
			}
		}
	};
}