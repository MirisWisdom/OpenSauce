/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.Collections.Generic;
using System.Text;
using TI = BlamLib.TagInterface;
using GDI = System.Drawing;

namespace BlamLib.Bitmaps
{
	#region enums
	[Flags]
	public enum Flags
	{
		enable_diffusion_dithering = 1 << 0,
		disable_height_map_compression = 1 << 1,
		uniform_sprite_bug_fix = 1 << 2,
		filthy_sprite_bug_fix = 1 << 3,
		use_sharp_bump_filter = 1 << 4,
		unused = 1 << 5,
		use_clamped_bump_filter = 1 << 6,
		invert_detail_fade = 1 << 7,
		swap_xy_vector_components = 1 << 8,
		convert_from_signed = 1 << 9,
		import_mipmap_chains = 1 << 10,
		intentionally_true_color = 1 << 11,

		// bitmap data flags
		power_of_two = 1 << 0,
		compressed = 1 << 1,
		palettized = 1 << 2,
		swizzled = 1 << 3,
		linear = 1 << 4,
		v16u16 = 1 << 5,
		mipmap_debug_level = 1 << 6,
		prefer_low_detail = 1 << 7,
	};

	public enum Enums
	{
		_2d_texture,
		_3d_texture,
		cube_map,

		sprites,
		interface_bitmaps,

		/// <remarks>Halo 1 only</remarks>
		white = cube_map+1,
	};
	#endregion

	public enum AssetFormat
	{
		tga,
		tif,
		bmp,
		jpg,
		png
	}

	public enum Format
	{
		/// <summary>alpha</summary>
		a8,
		/// <summary>intensity</summary>
		y8,
		/// <summary>combined alpha-intensity</summary>
		ay8,
		/// <summary>separate alpha-intensity</summary>
		a8y8,

		unused1,
		unused2,

		/// <summary>high-color</summary>
		r5g6b5, // Format16bppRgb565
		/// <summary>r6g5b5</summary>
		unused3,
		/// <summary>high-color with 1-bit alpha</summary>
		a1r5g5b5, // Format16bppArgb1555
		/// <summary>high-color with alpha</summary>
		a4r4g4b4,
		/// <summary>true-color</summary>
		x8r8g8b8, // Format32bppRgb
		/// <summary>true-color with alpha</summary>
		a8r8g8b8, // Format32bppArgb

		unused4,
		unused5,

		/// <summary>compressed with color-key transparency</summary>
		dxt1,
		/// <summary>compressed with explicit alpha</summary>
		dxt3,
		/// <summary>compressed with interpolated alpha</summary>
		dxt5,
		/// <summary>palettized bump map</summary>
		p8_bump,
		/// <summary>palettized</summary>
		p8,
		/// <summary>32 bit float ARGB</summary>
		argbfp32,
		/// <summary>32 bit float RGB</summary>
		rgbfp32,
		/// <summary>16 bit float RGB</summary>
		rgbfp16,
		/// <summary>v8u8</summary>
		v8u8,
		/// <summary>g8b8</summary>
		g8b8,

		// new in Halo 3:

#if !NO_HALO3
		dxn,
		ctx1,
		dxt3a_alpha,
		dxt3a_mono,
		dxt5a_alpha,
		dxt5a_mono,
		dxn_mono_alpha,

		// Reach:
		//TODO,
		//TODO,
		//TODO,

		//TODO,
#endif

		Max,
	};

	internal static class Util
	{
		static readonly short[] kFormatPixelSize = {
			0x8,
			0x8,
			0x8,
			0x10,

			0x4,
			0x4,

			0x10,
			0x0,
			0x10,
			0x10,
			0x20,
			0x20,

			0x1,
			0x0,

			0x4,
			0x8,
			0x8,
			0x8,
			0x8,
			0x80,
			0x60,
			0x30,
			0x10,
			0x10,

#if !NO_HALO3
			0x0, //TODO
			0x0, //TODO
			0x0, //TODO
			0x0, //TODO
			0x0, //TODO
			0x0, //TODO
			0x0, //TODO

			0x0, //TODO
			0x0, //TODO
			0x0, //TODO

			0x0, //TODO
#endif
		};

#if !NO_HALO3
		static readonly uint[] kXenonFormats = {
			0x04900102, // D3DFMT_A8
			0x28000102, // D3DFMT_L8

			0x1A200102,

			0x0800014A, // D3DFMT_A8L8
			0xFFFFFFFF,
			0xFFFFFFFF,
			0x28280144, // D3DFMT_R5G6B5
			0xFFFFFFFF,
			0x18280143, // D3DFMT_A1R5G5B5
			0x1828014F, // D3DFMT_A4R4G4B4
			0x28280186, // D3DFMT_X8R8G8B8
			0x18280186, // D3DFMT_A8R8G8B8
			0xFFFFFFFF,
			0xFFFFFFFF,
			0x1A200152, // D3DFMT_DXT1
			0x1A200153, // D3DFMT_DXT2
			0x1A200154, // D3DFMT_DXT4

			0x1829FF4F,

			0xFFFFFFFF,
			0xFFFFFFFF,
			0xFFFFFFFF,
			0xFFFFFFFF,
			0x2D20AB4A, // D3DFMT_V8U8
			0xFFFFFFFF,

			0x1A22ABA6, // D3DFMT_A32B32G32R32F
			0x1A22AB5D, // D3DFMT_A16B16G16R16F_EXPAND
			0x1A20AB86, // D3DFMT_Q8W8V8U8
			0x182801B6, // D3DFMT_A2R10G10B10
			0x1A20015A, // D3DFMT_A16B16G16R16
			0x2D20AB99, // D3DFMT_V16U16
			0x1A20017A, // D3DFMT_DXT3A
			0x1A20017B, // D3DFMT_DXT5A
			0x1A20017D, // D3DFMT_DXT3A_1111

			0x1A215571,
			0x1A21557C,
			0x1C90017A,
			0x2A20017A,
			0x1C90017B,
			0x2A20017B,
			0x08000171,

			// Reach:
			0x022C0154,
			0x0A600154,
			0x13200154,

			0x2D200196,
		};

		public static uint GetD3DFormat(this Format f)
		{
			return kXenonFormats[(int)f];
		}
#endif

		public static int GetFormatPixelSize(this Format f, BlamVersion engine)
		{
			Debug.Assert.If(f >= Format.a8 && f < Format.Max);

#if !XDK_NO_360
			if ((engine & BlamVersion.Halo3) != 0 || (engine & BlamVersion.HaloOdst) != 0 ||
				(engine & BlamVersion.HaloReach) != 0)
				return (int)LowLevel.Xbox360.Graphics.GetFormatSize( f.GetD3DFormat() );
#endif
			return kFormatPixelSize[(int)f];
		}

		public static GDI.Imaging.PixelFormat ToPixelFormat(this Format f)
		{
			switch(f)
			{
				case Format.r5g6b5:		return GDI.Imaging.PixelFormat.Format16bppRgb565;
				case Format.a1r5g5b5:	return GDI.Imaging.PixelFormat.Format16bppArgb1555;
				case Format.x8r8g8b8:	return GDI.Imaging.PixelFormat.Format32bppRgb;
				case Format.a8r8g8b8:	return GDI.Imaging.PixelFormat.Format32bppArgb;
				//case Format: return GDI.Imaging.PixelFormat.;
			}
			return GDI.Imaging.PixelFormat.Undefined;
		}
		
		private static readonly Dictionary<AssetFormat, string> kAssetExtensions = new Dictionary<AssetFormat,string>
		{
			{ AssetFormat.tga, "tga" },
			{ AssetFormat.tif, "tif" },
			{ AssetFormat.bmp, "bmp" },
			{ AssetFormat.jpg, "jpg" },
			{ AssetFormat.png, "png" },
		};

		public static string GetAssetExtension(AssetFormat format)
		{
			return kAssetExtensions[format];
		}
	};

	public abstract class bitmap_data_block : TI.Definition
	{
		#region Fields
		public TI.Tag Signature;
		public TI.ShortInteger Width;
		public TI.ShortInteger Height;
		public TI.Enum Type;
		public TI.Enum Format;
		public TI.Flags Flags;
		public TI.Point2D RegistrationPoint;
		public TI.LongInteger PixelsOffset;
		#endregion

		protected bitmap_data_block (int field_count) : base(field_count) {}

		public abstract int GetDepth();
		public abstract short GetMipmapCount();

		#region util
		int MipmapGetHeight(short mipmap)
		{
			int val = Math.Max(1, Height >> mipmap);
			if ((Flags.Value & (uint)Bitmaps.Flags.compressed) != 0)
				val += (4 - (val & 3)) & 3;
			return val;
		}

		int MipmapGetWidth(short mipmap)
		{
			int val = Math.Max(1, Width >> mipmap);
			if ((Flags.Value & (uint)Bitmaps.Flags.compressed) != 0)
				val += (4 - (val & 3)) & 3;
			return val;
		}

		int MipmapGetDepth(short mipmap) { return Math.Max(1, GetDepth() >> mipmap); }

		int MipmapGetPixelCount(short mipmap)
		{
			int h, w, d, calc;

			h = MipmapGetHeight(mipmap);
			w = MipmapGetWidth(mipmap);
			d = MipmapGetDepth(mipmap);

			calc = (h * w) * d;
			if (Type == (int)Enums.cube_map) calc *= 6;

			return calc;
		}

		int GetPixelCount()
		{
			int calc = 0;
			for (short index = 0; index <= GetMipmapCount(); index++)
				calc += MipmapGetPixelCount(index);

			return calc;
		}

		public int GetPixelDataSize(BlamVersion engine)
		{
			//if (Format >= (int)Format.dxt1 && Format <= (int)Format.dxt5)
			//	;

			int calc = GetPixelCount();
			int bits = ((Format)Format.Value).GetFormatPixelSize(engine);
			calc *= bits;
			calc = (calc + (bits & 7)) >> 3;
			return calc;
		}
		#endregion
	};
}