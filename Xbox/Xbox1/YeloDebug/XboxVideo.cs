using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Net.NetworkInformation;
using System.Drawing;
using System.Drawing.Imaging;
using System.Diagnostics;   // debugger display

namespace YeloDebug
{
    // todo: handle different framebuffer formats
    // currently only supports X8R8G8B8 at 640x480

    // ~16.7fps regular immediate presentation 100mbs full duplex
    // ~56fps with regular immediate presentation, 30fps with regular presentation interval two
    // ~63fps with a small video size using an immediate presentation window

    // todo: need to implement alternate videostream (doesnt work with halo - too old), 
    // where we manually inject something to grab frame data locally on xbox
    // have it dump frame as quick as possible to second buffer in memory, 
    // then we can grab it off xbox and transfer to computer

    // inject script that dumps frame in 640x480x16bpp (possibly using the dxt script) to separate buffer and then check a bit in memory
    // have yelodebug constantly polling that bit for frame readiness...when ready, read it all, and uncheck the bit
    // basically copy 16bpp image to buffer and wait for computer to grab it

    // requires 512kb of xbox memory for full screen, or 150kb for dxt but a ~30ms encoding delay per frame at the expense of lower network traffic

    /// <summary>
    /// Creates a standard xbox video stream.
    /// </summary>
    public class XboxVideoStream
    {
        // cache flag = 0x80


        public enum VideoSize
        {
            /// <summary>
            /// 640x480
            /// </summary>
            Full,

            /// <summary>
            /// 320x240
            /// </summary>
            Medium,

            /// <summary>
            /// 160x120
            /// </summary>
            Small,

            /// <summary>
            /// 80x60
            /// </summary>
            Thumbnail
        }

        public enum VideoPresentationInterval
        {
            NoOverride = 0,
            One = 0x10,
            Two = 0x20,
            Three = 0x30,
            Immediate = 0x40,
            OneOrImmediate = 0x50,
            TwoOrImmediate = 0x60,
            ThreeOrImmediate = 0x70
        };

        /// <summary>
        /// I would suggest always using regular quality, it is plenty sufficient.
        /// </summary>
        public enum VideoQuality
        {
            /// <summary>
            /// 16bpp
            /// </summary>
            Regular = 0,

            /// <summary>
            /// 24bpp - actually returns 32bpp in the raw frame data, 
            /// making a high quality medium feed use almost as much 
            /// bandwith as a regular quality full sized one.  The
            /// actual quality between the two settings is not noticeable,
            /// therefore I reccommend you never use the High quality setting.
            /// </summary>
            High = 4
        };

        #region Fields
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private Xbox Xbox;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte[] FrameBuffer = new byte[64 + 640 * 480 * 4];
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte[] BitmapBuffer = new byte[56 + 640 * 480 * 4];
        #endregion

        #region Properties
        public VideoSize Size { get { return size; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private VideoSize size;


        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private VideoQuality Quality;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private VideoPresentationInterval Interval;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint Width;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint Height;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint BitsPerPixel;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint FrameSize;

        // minimum time between frames - can be used to skip some
        private uint timeBetweenFrames = 0;

        /// <summary>
        /// Current frame in the video stream
        /// </summary>
        public uint FrameNumber { get { return frameNumber; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private uint frameNumber;


        public bool IsActive { get { return isActive; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private bool isActive = false;
        #endregion

        #region Constructors
        /// <summary>
        /// Initializes a video stream of optimal settings.
        /// </summary>
        /// <param name="xbox"></param>
        public XboxVideoStream(Xbox xbox)
        {
            Xbox = xbox;
            this.size = VideoSize.Medium;
            Quality = VideoQuality.Regular;
            Interval = VideoPresentationInterval.Two;
        }
        public XboxVideoStream(Xbox xbox, VideoSize size, VideoQuality quality, VideoPresentationInterval interval)
        {
            Xbox = xbox;
            this.size = size;
            Quality = quality;
            Interval = interval;

            // todo: check memory requirements
            // 152kb (38 4kb pages) for medium size and normal quality
            // 604kb (151 4kb pages) for full size and normal quality

            // calculate width*height*quality+1 and round up to next 4kb page

            //640*480*16

            //throw new OutOfMemoryException("Not enough memory present to initialize the stream.");

        }
        #endregion

        #region Methods

        // received at start of video session
        //struct vsheader
        //{
        //    int sizeof
        //    int frame_width
        //    int frame_height
        //    short unk1
        //    short bpp
        //    int unk2
        //    int frame_size
        //    int unk3
        //    int unk4
        //    int unk5
        //    int block_size
        //}

        /// <summary>
        /// Begins a new video stream.
        /// </summary>
        public void Begin()
        {
            try
            {
                End();
                Xbox.SendCommand("VSSnap first={0} last={1} flags=0x2000000 marker=0", (int)Size | (int)Interval | (int)Quality, timeBetweenFrames);
                byte[] header = Xbox.ReceiveBinaryData(40);
                uint size = BitConverter.ToUInt32(header, 0);
                Width = BitConverter.ToUInt32(header, 4);
                Height = BitConverter.ToUInt32(header, 8);
                int Format = BitConverter.ToUInt16(header, 12); // 1 = X8R8G8B8 ??? 
                BitsPerPixel = BitConverter.ToUInt16(header, 14);
                uint unk2 = BitConverter.ToUInt32(header, 16);
                FrameSize = BitConverter.ToUInt32(header, 20);
                uint unk3 = BitConverter.ToUInt32(header, 24);
                uint unk4 = BitConverter.ToUInt32(header, 28);
                uint unk5 = BitConverter.ToUInt32(header, 32);
                uint blockSize = BitConverter.ToUInt32(header, 36);
                System.Threading.Thread.Sleep(100); // wait for it to fully activate
                isActive = true;
            }
            catch(Exception ex)
            {
                End();
            }
        }

        /// <summary>
        /// Ends the current video stream.
        /// </summary>
        public void End()
        {
            try
            {
                isActive = false;
                Xbox.SendCommand("VSSnap first=0 last=1 flags=0x1000000 marker=0");
            }
            catch { }
        }

        /// <summary>
        /// Restarts the video stream.
        /// </summary>
        public bool Restart()
        {
            End();
            Begin();

            return isActive;
        }

        // received when you grab a frame during the video session
        //struct frameheader
        //{
        //    int timestamp
        //    int frame_num
        //    int unk3
        //    int unk4
        //    int unk5
        //    int frame_size
        //    int unk6
        //    int unk7
        //    int unk8
        //    int block_size
        //    int padding[6]
        //}

        /// <summary>
        /// Advances to the next frame in the video stream.  This also controls the rate at which the game plays.
        /// </summary>
        /// <returns></returns>
        public Image NextFrame()
        {
            try
            {
                Xbox.SendCommand("VSSnap first=0 last=1 flags=0x10000000 marker=0");
                FrameBuffer = Xbox.ReceiveBinaryData((int)FrameSize);

                using (MemoryStream frameStream = new MemoryStream(FrameBuffer))
                using (BinaryReader frameReader = new BinaryReader(frameStream))
                using (MemoryStream bitmapStream = new MemoryStream(BitmapBuffer))
                using (BinaryWriter bitmapWriter = new BinaryWriter(bitmapStream))
                {
                    // read frame header information
                    uint timeStamp = frameReader.ReadUInt32();
                    uint unkCount = frameReader.ReadUInt32();
                    uint cpuFrequency = frameReader.ReadUInt32();
                    uint unk2 = frameReader.ReadUInt32();
                    uint unk3 = frameReader.ReadUInt32();
                    uint frameSize = frameReader.ReadUInt32();
                    frameNumber = frameReader.ReadUInt32();
                    uint unk4 = frameReader.ReadUInt32();
                    uint unk6 = frameReader.ReadUInt32();
                    uint blockSize = frameReader.ReadUInt32();
                    // skip 6 bytes of padding
                    frameStream.Position = 64;

                    //uint rowSize = Width * 2;
                    //if (Quality == VideoQuality.High) rowSize *= 2;

                    // write bitmap header
                    bitmapWriter.Write(new char[] { 'B', 'M' }); // ushort bfType - BM (19778)
                    bitmapWriter.Write((uint)FrameSize - 8);    // uint bfSize - Size of the file (bytes) - Header + ImageData + (ushort)0
                    bitmapWriter.Write((ushort)0);    // ushort bfReserved1 - Zero
                    bitmapWriter.Write((ushort)0);    // ushort bfReserved2 - Zero
                    bitmapWriter.Write((uint)54);     // uint bfOffBits - Offset to image data
                    // BITMAPINFOHEADER
                    bitmapWriter.Write((uint)40);     // uint biSize - Size of BITMAPINFOHEADER (bytes)
                    bitmapWriter.Write((uint)Width);  // uint biWidth - Width of image (pixels)
                    bitmapWriter.Write((uint)Height); // uint biHeight - Height of image (pixels)
                    bitmapWriter.Write((ushort)1);    // ushort biPlanes - Number of planes of the target device (usually one)
                    if (Quality == VideoQuality.Regular)
                    {
                        bitmapWriter.Write((ushort)16); // ushort biBitCount - Bits per pixel (1=black/white, 4=16 colors, 8=256 colors, 24=16.7 million colors)
                    }
                    else
                    {
                        bitmapWriter.Write((ushort)32); // ushort biBitCount - Bits per pixel (1=black/white, 4=16 colors, 8=256 colors, 24=16.7 million colors)
                    }
                    bitmapWriter.Write((uint)0);      // uint biCompression - Type of compression (0=None)
                    bitmapWriter.Write((uint)0);      // uint biSizeImage - Size of image data (bytes) - Zero if no compression
                    bitmapWriter.Write((uint)0);      // uint biXPelsPerMeter - Hoizontal pixels per meter (usually zero) (2834=72 Pixels Per Inch)
                    bitmapWriter.Write((uint)0);      // uint biYPelsPerMeter - Vertical pixels per meter (usually zero) (2834=72 Pixels Per Inch)
                    bitmapWriter.Write((uint)0);      // uint biClrUsed - Number of colors used - If zero, calculated by biBitCount
                    bitmapWriter.Write((uint)0);      // uint biClrImportant - Number of "important" colors (0=All)

                    byte[] bmpRow = new byte[blockSize];
                    int rowCount = (int)((FrameSize - 64) / blockSize);
                    for (int i = rowCount; i > 0; i--)
                    {
                        // get row
                        frameStream.Position = 64 + i * blockSize;
                        frameStream.Read(bmpRow, 0, bmpRow.Length);

                        // write to bitmap data
                        bitmapWriter.Write(bmpRow);
                    }
                    bitmapWriter.Write((ushort)0);
                    return Image.FromStream(bitmapStream);
                }
            }
            catch
            {
                // xbox not responding - timeout, retry
                End();
            }
            return null;
        }
        #endregion
    }

    public partial class Xbox : IDisposable
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte[] FrameBuffer = new byte[64 + 640 * 480 * 4];
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private byte[] BitmapBuffer = new byte[56 + 640 * 480 * 4];


        public SurfaceInformation GetSurfaceInformation(Surface surface)
        {
            SurfaceInformation si = new SurfaceInformation();
            StatusResponse response = SendCommand("getsurf id={0}", (int)surface);
            List<object> info = Util.ExtractResponseInformation(response.Message);
            si.Size = (uint)info[0];
            si.Format = (uint)info[1];
            si.Address = (uint)info[2];
            si.PushBufferPut = (uint)info[3];
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
        public Image Screenshot()
        {
            int read = 0;
            MemoryStream.Read(LockGPU(), FrameBuffer.Length, ref FrameBuffer, 0, ref read);
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

                return Bitmap.FromStream(ms);
            }
        }
    }
}
