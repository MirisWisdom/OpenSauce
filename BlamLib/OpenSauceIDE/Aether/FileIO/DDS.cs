using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using BlamLib.Blam;

using H1Tags = BlamLib.Blam.Halo1.Tags;

namespace OpenSauceIDE.Aether.FileIO
{
    namespace DDS
    {    
        public class DDS_PIXELFORMAT
        {
            [FlagsAttribute]
            public enum DDPF : uint
            {
                DDPF_ALPHAPIXELS = 0x1,
                DDPF_ALPHA = 0x2,
                DDPF_FOURCC = 0x4,
                DDPF_PALLETEINDEXED8 = 0x20,
                DDPF_RGB = 0x40,
                DDPF_YUV = 0x200,
                DDPF_LUMINANCE = 0x20000,
                DDPF_BUMPDUDV = 0x80000
            }
            #region DDS FourCC
            public static readonly uint[] DDSFCC = 
            {
                0x0000001C,
                0x00000032,
                0x00000000,
                0x00000033,
                0x00000000,
                0x00000000,
                0x00000000,
                0x00000000,
                0x00000019,
                0x0000001A,
                0x00000016,
                0x00000015,
                0x00000000,
                0x00000000,
                0x31545844,
                0x33545844,
                0x35545844,
                0x20203850,
                0x20203850,
                0x00000074,
                0x00000072,
                0x0000006F,
                0x00000000,
                0x00000000
            };
            #endregion
            /// <summary>
            /// Structure size; set to 32 (bytes).
            /// </summary>
            public const uint dwSize = 32;
            /// <summary>
            /// Values which indicate what type of data is in the surface. 
            /// </summary>
            public DDPF dwFlags = 0;
            /// <summary>
            /// Four-character codes for specifying compressed or custom formats. 
            /// Possible values include: DXT1, DXT2, DXT3, DXT4, or DXT5.
            /// When using a four-character code, dwFlags must include DDPF_FOURCC.
            /// </summary>
            public uint dwFourCC = 0;
            /// <summary>
            /// Number of bits in an RGB (possibly including alpha) format. 
            /// Valid when dwFlags includes DDPF_RGB, DDPF_LUMINANCE, or DDPF_YUV.
            /// </summary>
            public uint dwRGBBitCount = 0;
            /// <summary>
            /// Red (or lumiannce or Y) mask for reading color data.
            /// </summary>
            public uint dwRBitMask = 0;
            /// <summary>
            /// Green (or U) mask for reading color data.
            /// </summary>
            public uint dwGBitMask = 0;
            /// <summary>
            /// Blue (or V) mask for reading color data.
            /// </summary>
            public uint dwBBitMask = 0;
            /// <summary>
            /// Alpha mask for reading alpha data.
            /// </summary>
            public uint dwABitMask = 0;

            /// <summary>
            /// Writes the contents of the class to a byte array
            /// </summary>
            /// <param name="binary_writer">Reference to a BinaryWriter to use</param>
            public void WriteToMemory(ref BinaryWriter binary_writer)
            {
                binary_writer.Write(dwSize);
                binary_writer.Write((uint)dwFlags);
                binary_writer.Write(dwFourCC);
                binary_writer.Write(dwRGBBitCount);
                binary_writer.Write(dwRBitMask);
                binary_writer.Write(dwGBitMask);
                binary_writer.Write(dwBBitMask);
                binary_writer.Write(dwABitMask);
            }

            public void ReadFromMemory(ref BinaryReader binary_reader)
            {
                binary_reader.BaseStream.Position += 4;
                dwFlags = (DDPF)binary_reader.ReadUInt32();
                dwFourCC = binary_reader.ReadUInt32();
                dwRGBBitCount = binary_reader.ReadUInt32();
                dwRBitMask = binary_reader.ReadUInt32();
                dwGBitMask = binary_reader.ReadUInt32();
                dwBBitMask = binary_reader.ReadUInt32();
                dwABitMask = binary_reader.ReadUInt32();
            }
        }
        
        public class DDS_HEADER
        {
            #region DDS Enumerators
            [FlagsAttribute]
            public enum DDSD : uint
            {
                DDSD_CAPS = 0x1,
                DDSD_HEIGHT = 0x2,
                DDSD_WIDTH = 0x4,
                DDSD_PITCH = 0x8,
                DDSD_PIXELFORMAT = 0x1000,
                DDSD_MIPMAPCOUNT = 0x20000,
                DDSD_LINEARSIZE = 0x80000,
                DDSD_DEPTH = 0x800000
            }
            public static readonly DDSD DDS_HEADER_FLAGS_TEXTURE =
                DDSD.DDSD_CAPS | DDSD.DDSD_HEIGHT | DDSD.DDSD_WIDTH | DDSD.DDSD_PIXELFORMAT;
            
            [FlagsAttribute]
            public enum DDSCAPS : uint
            {
                DDSCAPS_COMPLEX = 0x08,
                DDSCAPS_MIPMAP = 0x400000,
                DDSCAPS_TEXTURE = 0x1000
            }
            public static readonly DDSCAPS DDS_SURFACE_FLAGS_MIPMAP =
                DDSCAPS.DDSCAPS_COMPLEX | DDSCAPS.DDSCAPS_MIPMAP;

            [FlagsAttribute]
            public enum DDSCAPS2 : long
            {
                DDSCAPS2_CUBEMAP = 0x200,
                DDSCAPS2_CUBEMAP_POSITIVEX = 0x400,
                DDSCAPS2_CUBEMAP_NEGATIVEX = 0x800,
                DDSCAPS2_CUBEMAP_POSITIVEY = 0x1000,
                DDSCAPS2_CUBEMAP_NEGATIVEY = 0x2000,
                DDSCAPS2_CUBEMAP_POSITIVEZ = 0x4000,
                DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x8000,
                DDSCAPS2_VOLUME = 0x200000
            }
            public static readonly DDSCAPS2 DDS_CUBEMAP_POSITIVEX =
                DDSCAPS2.DDSCAPS2_CUBEMAP | DDSCAPS2.DDSCAPS2_CUBEMAP_POSITIVEX;
            public static readonly DDSCAPS2 DDS_CUBEMAP_NEGATIVEX =
                DDSCAPS2.DDSCAPS2_CUBEMAP | DDSCAPS2.DDSCAPS2_CUBEMAP_NEGATIVEX;
            public static readonly DDSCAPS2 DDS_CUBEMAP_POSITIVEY =
                DDSCAPS2.DDSCAPS2_CUBEMAP | DDSCAPS2.DDSCAPS2_CUBEMAP_POSITIVEY;
            public static readonly DDSCAPS2 DDS_CUBEMAP_NEGATIVEY =
                DDSCAPS2.DDSCAPS2_CUBEMAP | DDSCAPS2.DDSCAPS2_CUBEMAP_NEGATIVEY;
            public static readonly DDSCAPS2 DDS_CUBEMAP_POSITIVEZ =
                DDSCAPS2.DDSCAPS2_CUBEMAP | DDSCAPS2.DDSCAPS2_CUBEMAP_POSITIVEZ;
            public static readonly DDSCAPS2 DDS_CUBEMAP_NEGATIVEZ =
                DDSCAPS2.DDSCAPS2_CUBEMAP | DDSCAPS2.DDSCAPS2_CUBEMAP_NEGATIVEZ;
            public static readonly DDSCAPS2 DDS_CUBEMAP_ALLFACES =
                DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |
                DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |
                DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ;
            #endregion

            /// <summary>
            /// Size of structure. This member must be set to 124.
            /// </summary>
            public const uint dwSize = 124;
            /// <summary>
            /// Flags to indicate which members contain valid data. 
            /// </summary>
            public DDSD dwFlags = 0;
            /// <summary>
            /// Surface height (in pixels).
            /// </summary>
            public uint dwHeight = 0;
            /// <summary>
            /// Surface width (in pixels).
            /// </summary>
            public uint dwWidth = 0;
            /// <summary>
            /// The pitch or number of bytes per scan line in an uncompressed texture; the total number of bytes in the top level texture for a compressed texture.
            /// </summary>
            public uint dwPitchOrLinearSize = 0;
            /// <summary>
            /// Depth of a volume texture (in pixels), otherwise unused. 
            /// </summary>
            public uint dwDepth = 0;
            /// <summary>
            /// Number of mipmap levels, otherwise unused.
            /// </summary>
            public uint dwMipMapCount = 0;
            /// <summary>
            /// Unused.
            /// </summary>
            public readonly byte[] dwReserved1 = new byte[44];
            /// <summary>
            /// The pixel format.
            /// </summary>
            public DDS_PIXELFORMAT ddspf = new DDS_PIXELFORMAT();
            /// <summary>
            /// Specifies the complexity of the surfaces stored.
            /// </summary>
            public DDSCAPS dwCaps = 0;
            /// <summary>
            /// Additional detail about the surfaces stored.
            /// </summary>
            public DDSCAPS2 dwCaps2 = 0;
            /// <summary>
            /// Unused.
            /// </summary>
            public readonly uint dwCaps3 = 0;
            /// <summary>
            /// Unused.
            /// </summary>
            public readonly uint dwCaps4 = 0;
            /// <summary>
            /// Unused.
            /// </summary>
            public readonly uint dwReserved2 = 0;

            /// <summary>
            /// Writes the contents of the class to a byte array
            /// </summary>
            /// <param name="binary_writer">Reference to a BinaryWriter to use</param>
            public void WriteToMemory(ref BinaryWriter binary_writer)
            {
                binary_writer.Write(dwSize);
                binary_writer.Write((uint)dwFlags);
                binary_writer.Write(dwHeight);
                binary_writer.Write(dwWidth);
                binary_writer.Write(dwPitchOrLinearSize);
                binary_writer.Write(dwDepth);
                binary_writer.Write(dwMipMapCount);
                binary_writer.Write(dwReserved1);
                ddspf.WriteToMemory(ref binary_writer);
                binary_writer.Write((uint)dwCaps);
                binary_writer.Write((uint)dwCaps2);
                binary_writer.Write(dwCaps3);
                binary_writer.Write(dwCaps4);
                binary_writer.Write(dwReserved2);
            }

            public void ReadFromMemory(ref BinaryReader binary_reader)
            {
                binary_reader.BaseStream.Position += 4;
                dwFlags = (DDSD)binary_reader.ReadUInt32();
                dwHeight = binary_reader.ReadUInt32();
                dwWidth = binary_reader.ReadUInt32();
                dwPitchOrLinearSize = binary_reader.ReadUInt32();
                dwDepth = binary_reader.ReadUInt32();
                dwMipMapCount = binary_reader.ReadUInt32();
                binary_reader.BaseStream.Position += 44;
                ddspf.ReadFromMemory(ref binary_reader);
                dwCaps = (DDSCAPS)binary_reader.ReadUInt32();
                dwCaps2 = (DDSCAPS2)binary_reader.ReadUInt32();
                binary_reader.BaseStream.Position += 12;
            }
        }

        public class DDS_FILE
        {
            /// <summary>
            /// The four character code value 'DDS '.
            /// </summary>
            public const uint dwMagic = 0x20534444;
            /// <summary>
            /// A description of the data in the file.
            /// </summary>
            public DDS_HEADER header = new DDS_HEADER();
            /// <summary>
            /// Byte array containing the images pixel data
            /// </summary>
            public byte[] data;

            /// <summary>
            /// Writes the contents of the class to a byte array
            /// </summary>
            /// <param name="binary_writer">Reference to a BinaryWriter to use</param>
            public void WriteToMemory(ref BinaryWriter binary_writer)
            {
                binary_writer.Write(dwMagic);
                header.WriteToMemory(ref binary_writer);
                binary_writer.Write(data);
            }

            public void ReadFromMemory(ref BinaryReader binary_reader)
            {
                binary_reader.BaseStream.Position += 4;
                header.ReadFromMemory(ref binary_reader);

                data = new byte[binary_reader.BaseStream.Length - binary_reader.BaseStream.Position];
                binary_reader.Read(data,
                    0,
                    data.Length);
            }
        }
        
        public class DDSBuilder
        {
            /// <summary>
            /// Creates a DDS file from a bitmap_data_block
            /// </summary>
            public static DDS_FILE CreateDDSFile(BlamLib.Bitmaps.bitmap_data_block bitmap_data,
                ref byte[] source_data,
                uint data_length,
                bool sheet_3d)
            {
                DDS_FILE dds_file = new DDS_FILE();
                // set basic dimension information
                dds_file.header.dwFlags = DDS_HEADER.DDS_HEADER_FLAGS_TEXTURE;
                dds_file.header.dwHeight = (uint)bitmap_data.Height;
                dds_file.header.dwWidth = (uint)bitmap_data.Width;
                dds_file.header.dwDepth = (uint)bitmap_data.GetDepth();

                // set caps that depend on bitmap type
                switch ((BlamLib.Bitmaps.Enums)bitmap_data.Type.Value)
                {
                    case BlamLib.Bitmaps.Enums._3d_texture:
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_DEPTH;
                        dds_file.header.dwCaps2 = DDS_HEADER.DDSCAPS2.DDSCAPS2_VOLUME;
                        break;

                    case BlamLib.Bitmaps.Enums.cube_map:
                        dds_file.header.dwCaps |= DDS_HEADER.DDSCAPS.DDSCAPS_COMPLEX;
                        dds_file.header.dwCaps2 = DDS_HEADER.DDS_CUBEMAP_ALLFACES;
                        break;

                    case BlamLib.Bitmaps.Enums._2d_texture:
                    case BlamLib.Bitmaps.Enums.white:
                        dds_file.header.dwMipMapCount = (uint)bitmap_data.GetMipmapCount();
                        if (bitmap_data.GetMipmapCount() > 0)
                            dds_file.header.dwCaps |= DDS_HEADER.DDSCAPS.DDSCAPS_MIPMAP;
                        dds_file.header.dwCaps2 = 0;
                        break;
                }

                // TODO: create the sheet in DevIL
                // if the created bitmap should have all cube faces/volume slices in one bitmap
                //if (sheet_3d)
                //{
                //    BlamLib.Bitmaps.Enums bitmap_type = (BlamLib.Bitmaps.Enums)bitmap_data.Type.Value;
                //    do
                //    {
                //        if (bitmap_type == BlamLib.Bitmaps.Enums._2d_texture || bitmap_type == BlamLib.Bitmaps.Enums.white)
                //            break;

                //        if ((BlamLib.Bitmaps.Enums)bitmap_data.Type.Value == BlamLib.Bitmaps.Enums._3d_texture)
                //            dds_file.header.dwHeight *= dds_file.header.dwDepth;
                //        else if ((BlamLib.Bitmaps.Enums)bitmap_data.Type.Value == BlamLib.Bitmaps.Enums.cube_map)
                //            dds_file.header.dwHeight *= 6;

                //        dds_file.header.dwMipMapCount = (uint)bitmap_data.GetMipmapCount();
                //        dds_file.header.dwCaps2 = 0;
                //        if (dds_file.header.dwMipMapCount > 0)
                //            dds_file.header.dwCaps |= DDS_HEADER.DDSCAPS.DDSCAPS_MIPMAP;
                //    } while (false);
                //}

                // set pixel masks
                switch ((BlamLib.Bitmaps.Format)bitmap_data.Format.Value)
                {
                    case BlamLib.Bitmaps.Format.a8:
                        dds_file.header.ddspf.dwABitMask = 0x000000FF;
                        break;
                    case BlamLib.Bitmaps.Format.y8:
                        dds_file.header.ddspf.dwRBitMask = 0x000000FF;
                        break;
                    case BlamLib.Bitmaps.Format.ay8:
                        dds_file.header.ddspf.dwRBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwGBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwBBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwABitMask = 0x000000FF;
                        break;
                    case BlamLib.Bitmaps.Format.a8y8:
                        dds_file.header.ddspf.dwRBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwABitMask = 0x0000FF00;
                        break;
                    case BlamLib.Bitmaps.Format.r5g6b5:
                        dds_file.header.ddspf.dwRBitMask = 0x0000F800;
                        dds_file.header.ddspf.dwGBitMask = 0x000007E0;
                        dds_file.header.ddspf.dwBBitMask = 0x0000001F;
                        break;
                    case BlamLib.Bitmaps.Format.a1r5g5b5:
                        dds_file.header.ddspf.dwRBitMask = 0x00007C00;
                        dds_file.header.ddspf.dwGBitMask = 0x000003E0;
                        dds_file.header.ddspf.dwBBitMask = 0x0000001F;
                        dds_file.header.ddspf.dwABitMask = 0x00008000;
                        break;
                    case BlamLib.Bitmaps.Format.a4r4g4b4:
                        dds_file.header.ddspf.dwRBitMask = 0x00000F00;
                        dds_file.header.ddspf.dwGBitMask = 0x000000F0;
                        dds_file.header.ddspf.dwBBitMask = 0x0000000F;
                        dds_file.header.ddspf.dwABitMask = 0x0000F000;
                        break;
                    case BlamLib.Bitmaps.Format.x8r8g8b8:
                        dds_file.header.ddspf.dwRBitMask = 0x00FF0000;
                        dds_file.header.ddspf.dwGBitMask = 0x0000FF00;
                        dds_file.header.ddspf.dwBBitMask = 0x000000FF;
                        break;
                    case BlamLib.Bitmaps.Format.a8r8g8b8:
                        dds_file.header.ddspf.dwRBitMask = 0x00FF0000;
                        dds_file.header.ddspf.dwGBitMask = 0x0000FF00;
                        dds_file.header.ddspf.dwBBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwABitMask = 0xFF000000;
                        break;
                    case BlamLib.Bitmaps.Format.v8u8:
                        dds_file.header.ddspf.dwRBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwGBitMask = 0x0000FF00;
                        break;
                    case BlamLib.Bitmaps.Format.g8b8:
                        dds_file.header.ddspf.dwGBitMask = 0x000000FF;
                        dds_file.header.ddspf.dwBBitMask = 0x0000FF00;
                        break;
#if !NO_HALO3
                    //TODO: find out how to handle these, DevIL might not support many of them
                    case BlamLib.Bitmaps.Format.dxn:
                    case BlamLib.Bitmaps.Format.ctx1:
                    case BlamLib.Bitmaps.Format.dxt3a_alpha:
                    case BlamLib.Bitmaps.Format.dxt3a_mono:
                    case BlamLib.Bitmaps.Format.dxt5a_alpha:
                    case BlamLib.Bitmaps.Format.dxt5a_mono:
                    case BlamLib.Bitmaps.Format.dxn_mono_alpha:
#endif
                    case BlamLib.Bitmaps.Format.p8_bump:
                    case BlamLib.Bitmaps.Format.p8:
                    case BlamLib.Bitmaps.Format.argbfp32:
                    case BlamLib.Bitmaps.Format.rgbfp32:
                    case BlamLib.Bitmaps.Format.rgbfp16:
                    case BlamLib.Bitmaps.Format.unused1:
                    case BlamLib.Bitmaps.Format.unused2:
                    case BlamLib.Bitmaps.Format.unused3:
                    case BlamLib.Bitmaps.Format.unused4:
                    case BlamLib.Bitmaps.Format.unused5:
                    case BlamLib.Bitmaps.Format.dxt1:
                    case BlamLib.Bitmaps.Format.dxt3:
                    case BlamLib.Bitmaps.Format.dxt5:
                        break;
                }

                // set pixel format specifics
                switch ((BlamLib.Bitmaps.Format)bitmap_data.Format.Value)
                {
                    case BlamLib.Bitmaps.Format.a8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_ALPHAPIXELS;
                        dds_file.header.ddspf.dwRGBBitCount = 8;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;                        
                    case BlamLib.Bitmaps.Format.y8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_LUMINANCE;
                        dds_file.header.ddspf.dwRGBBitCount = 8;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.ay8:
                        dds_file.header.dwMipMapCount = 0;
                        dds_file.header.dwCaps &= (0xFFFFFFFF - DDS_HEADER.DDSCAPS.DDSCAPS_MIPMAP);
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_LUMINANCE;
                        dds_file.header.ddspf.dwRGBBitCount = 8;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.a8y8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_LUMINANCE | DDS_PIXELFORMAT.DDPF.DDPF_ALPHAPIXELS;
                        dds_file.header.ddspf.dwRGBBitCount = 16;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.r5g6b5:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_RGB;
                        dds_file.header.ddspf.dwRGBBitCount = 16;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.a1r5g5b5:
                    case BlamLib.Bitmaps.Format.a4r4g4b4:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_RGB | DDS_PIXELFORMAT.DDPF.DDPF_ALPHAPIXELS;
                        dds_file.header.ddspf.dwRGBBitCount = 16;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.x8r8g8b8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_RGB;
                        dds_file.header.ddspf.dwRGBBitCount = 32;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.a8r8g8b8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_RGB | DDS_PIXELFORMAT.DDPF.DDPF_ALPHAPIXELS;
                        dds_file.header.ddspf.dwRGBBitCount = 32;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                    case BlamLib.Bitmaps.Format.dxt1:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_FOURCC;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.dxt3:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_FOURCC;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.dxt5:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_FOURCC;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.p8_bump:
                    case BlamLib.Bitmaps.Format.p8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_PALLETEINDEXED8;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        // TODO: does this even work?
                        dds_file.header.dwCaps &= (0xFFFFFFFF - DDS_HEADER.DDSCAPS.DDSCAPS_MIPMAP);
                        break;
                    case BlamLib.Bitmaps.Format.argbfp32:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_FOURCC;
                        dds_file.header.dwFlags = DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.rgbfp32:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_FOURCC;
                        dds_file.header.dwFlags = DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.rgbfp16:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_FOURCC;
                        dds_file.header.dwFlags = DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.v8u8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_BUMPDUDV;
                        dds_file.header.ddspf.dwRGBBitCount = 16;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        dds_file.header.dwFlags = DDS_HEADER.DDSD.DDSD_LINEARSIZE;
                        break;
                    case BlamLib.Bitmaps.Format.g8b8:
                        dds_file.header.ddspf.dwFlags = DDS_PIXELFORMAT.DDPF.DDPF_ALPHAPIXELS | DDS_PIXELFORMAT.DDPF.DDPF_LUMINANCE;
                        dds_file.header.ddspf.dwRGBBitCount = 16;
                        dds_file.header.dwFlags |= DDS_HEADER.DDSD.DDSD_PITCH;
                        break;
                }
                dds_file.header.ddspf.dwFourCC = DDS_PIXELFORMAT.DDSFCC[bitmap_data.Format.Value];

                // copy the pixel data from the bitmap block
                dds_file.data = new byte[data_length];
                Array.Copy(source_data, (int)bitmap_data.PixelsOffset, dds_file.data, 0, data_length);
                return dds_file;
            }
            public static DDS_FILE ReadDDSFile(byte[] dds_data)
            {
                MemoryStream memory_stream = new MemoryStream(dds_data);
                BinaryReader binary_reader = new BinaryReader(memory_stream);

                DDS_FILE dds_file = new DDS_FILE();
                dds_file.ReadFromMemory(ref binary_reader);

                binary_reader.Close();
                memory_stream.Close();

                return dds_file;
            }
        }
    }
}
