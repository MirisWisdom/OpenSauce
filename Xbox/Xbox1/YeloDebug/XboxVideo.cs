/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.IO;
using System.Diagnostics;   // debugger display

namespace YeloDebug
{
    // todo: handle different framebuffer formats
    // currently only supports X8R8G8B8 at 640x480

    // ~16.7fps regular immediate presentation 100mbs full duplex
    // ~56fps with regular immediate presentation, 30fps with regular presentation interval two
    // ~63fps with a small video size using an immediate presentation window

    // todo: need to implement alternate videostream (doesn't work with halo - too old), 
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
            /// bandwidth as a regular quality full sized one.  The
            /// actual quality between the two settings is not noticeable,
            /// therefore I recommend you never use the High quality setting.
            /// </summary>
            High = 4
        };

        #region Fields
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        Xbox Xbox;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        byte[] FrameBuffer = new byte[64 + 640 * 480 * 4];
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        byte[] BitmapBuffer = new byte[56 + 640 * 480 * 4];
        #endregion

        #region Properties
        public VideoSize Size { get { return size; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        VideoSize size;


        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        VideoQuality Quality;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        VideoPresentationInterval Interval;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint Width;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint Height;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint BitsPerPixel;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint FrameSize;

        // minimum time between frames - can be used to skip some
        uint timeBetweenFrames = 0;

        /// <summary>
        /// Current frame in the video stream
        /// </summary>
        public uint FrameNumber { get { return frameNumber; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        uint frameNumber;


        public bool IsActive { get { return isActive; } }
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        bool isActive = false;
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
            catch(Exception /*ex*/)
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
		public System.Drawing.Image NextFrame()
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
					return System.Drawing.Image.FromStream(bitmapStream);
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
}