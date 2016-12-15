/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum bitmap_type : _enum
		{
			_bitmap_type__2d_texture,
			_bitmap_type__3d_texture,
			_bitmap_type_cube_map,
			_bitmap_type_white,

			_bitmap_type,
		};
		enum bitmap_pixel_format : _enum
		{
			_bitmap_pixel_format_a8,
			_bitmap_pixel_format_y8,
			_bitmap_pixel_format_ay8,
			_bitmap_pixel_format_a8y8,
			_bitmap_pixel_format_unused1,
			_bitmap_pixel_format_unused2,
			_bitmap_pixel_format_r5g6b5,
			_bitmap_pixel_format_unused3,
			_bitmap_pixel_format_a1r5g5b5,
			_bitmap_pixel_format_a4r4g4b4,
			_bitmap_pixel_format_x8r8g8b8,
			_bitmap_pixel_format_a8r8g8b8,
			_bitmap_pixel_format_unused4,
			_bitmap_pixel_format_unused5,
			_bitmap_pixel_format_dxt1,
			_bitmap_pixel_format_dxt3,
			_bitmap_pixel_format_dxt5,
			_bitmap_pixel_format_p8_bump,

			_bitmap_pixel_format,
		};
		enum bitmap_group_type : _enum
		{
			_bitmap_group_type__2d_textures,
			_bitmap_group_type__3d_textures,
			_bitmap_group_type_cube_maps,
			_bitmap_group_type_sprites,
			_bitmap_group_type_interface_bitmaps,

			k_number_of_bitmap_group_types,
		};
		enum bitmap_group_format : _enum
		{
			_bitmap_group_format_compressed_with_color_key_transparency,
			_bitmap_group_format_compressed_with_explicit_alpha,
			_bitmap_group_format_compressed_with_interpolated_alpha,
			_bitmap_group_format__16_bit_color,
			_bitmap_group_format__32_bit_color,
			_bitmap_group_format_monochrome,

			k_number_of_bitmap_group_formats,
		};
		enum bitmap_group_usage : _enum
		{
			_bitmap_group_usage_alpha_blend,
			_bitmap_group_usage_default,
			_bitmap_group_usage_height_map,
			_bitmap_group_usage_detail_map,
			_bitmap_group_usage_light_map,
			_bitmap_group_usage_vector_map,

			k_number_of_bitmap_group_usages,

			//_bitmap_group_usage_height_map_blue255,		// Halo2
			//_bitmap_group_usage_embm,						// Halo2. emblem?
			//_bitmap_group_usage_height_map_a8l8,			// Halo2
			//_bitmap_group_usage_height_map_g8b8,			// Halo2
			//_bitmap_group_usage_height_map_g8b8_y_alpha,	// Halo2
		};
		enum sprite_budget : _enum
		{
			_sprite_budget__32x32,
			_sprite_budget__64x64,
			_sprite_budget__128x128,
			_sprite_budget__256x256,
			_sprite_budget__512x512,

			_sprite_budget,

			//_sprite_budget__1024x1024,	// Halo2. However, sprite processing fields were hidden from view... 
		};
		enum sprite_usage : _enum
		{
			_sprite_usage_blend_add_subtract_max,
			_sprite_usage_multiply_min,
			_sprite_usage_double_multiply,

			_sprite_usage,
		};
	};

	namespace Flags
	{
		enum bitmap_group_flags : word_flags
		{
			_bitmap_group_enable_diffusion_dithering_bit,
			_bitmap_group_disable_height_map_compression_bit,
			_bitmap_group_uniform_sprite_sequences_bit,
			_bitmap_group_filthy_sprite_bug_fix_bit,

			k_number_of_bitmap_group_flags,

			_bitmap_group_reserved4_bit =	// Halo2. Use Sharp Bump Filter
				k_number_of_bitmap_group_flags,
			_bitmap_group_unused5_bit,
			_bitmap_group_reserved6_bit,	// Halo2. Use Clamped/Mirrored Bump Filter
			_bitmap_group_reserved7_bit,	// Halo2. Invert Detail Fade
			_bitmap_group_reserved8_bit,	// Halo2. Swap x-y Vector Components
			_bitmap_group_reserved9_bit,	// Halo2. Convert from Signed
			_bitmap_group_reserved10_bit,	// Halo2. Convert to Signed
			_bitmap_group_reserved11_bit,	// Halo2. Import mipmap Chains
			_bitmap_group_reserved12_bit,	// Halo2. Intentionally True Color

			// the bitmap group's data should never be stored in any sort of 'shared' cache file
			_bitmap_group_never_share_resources_yelo_bit,

			k_number_of_bitmap_group_flags_yelo,
		};
	};

	namespace TagGroups
	{
		struct s_bitmap_group_sprite
		{
			TAG_FIELD(int16, bitmap_index);
			PAD16;
			PAD32;
			TAG_FIELD(real, left);
			TAG_FIELD(real, right);
			TAG_FIELD(real, top);
			TAG_FIELD(real, bottom);
			TAG_FIELD(real_point2d, registration_point);
		}; BOOST_STATIC_ASSERT( sizeof(s_bitmap_group_sprite) == 0x20 ); // max count: 64

		struct s_bitmap_group_sequence
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, first_bitmap_index);
			TAG_FIELD(int16, bitmap_count);
			TAG_PAD(int32, 4);
			TAG_TBLOCK(sprites, s_bitmap_group_sprite);
		}; BOOST_STATIC_ASSERT( sizeof(s_bitmap_group_sequence) == 0x40 ); // max count: 256

		struct s_bitmap_data
		{
			struct _flags
			{
				TAG_FLAG16(power_of_two_dimensions);
				TAG_FLAG16(compressed);
				TAG_FLAG16(palettized);
				TAG_FLAG16(swizzled);
				TAG_FLAG16(linear);
				TAG_FLAG16(v16u16);
				TAG_FLAG16(orphan); // this bitmap and its pixel data are allocated outside of the tag system
				TAG_FLAG16(cached); // _bitmap_cached_bit
				TAG_FLAG16(in_data_file); // data is in the bitmaps data file, not the cache file
			}; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			TAG_FIELD(tag, signature);
			TAG_FIELD(int16, width, "pixels");
			TAG_FIELD(int16, height, "pixels");
			TAG_FIELD(int16, depth, "pixels", "depth is 1 for 2D textures and cube maps", "depth is 1 for 2D textures and cube maps");
			TAG_ENUM(type, Enums::bitmap_type, "determines bitmap 'geometry'");
			TAG_ENUM(format, Enums::bitmap_pixel_format, "determines how pixels are represented internally");
			TAG_FIELD(_flags, flags);
			TAG_FIELD(point2d, registration_point);
			TAG_FIELD(int16, mipmap_count);
			PAD16;
			TAG_FIELD(int32, pixels_offset);

			UNKNOWN_TYPE(int32);
			datum_index owner_tag_index;
			datum_index texture_cache_index;
			void* hardware_format; // IDirect3DBaseTexture9*
			void* base_address;
		}; BOOST_STATIC_ASSERT( sizeof(s_bitmap_data) == 0x30 ); // max count: 2048

		struct s_bitmap_group
		{
			enum { k_group_tag = 'bitm' };

			////////////////////////////////////////////////////////////////
			// type
			// Type controls bitmap 'geometry'. All dimensions must be a power of two except for SPRITES and INTERFACE BITMAPS:
			// 
			// * 2D TEXTURES: Ordinary, 2D textures will be generated.
			// * 3D TEXTURES: Volume textures will be generated from each sequence of 2D texture 'slices'.
			// * CUBE MAPS: Cube maps will be generated from each consecutive set of six 2D textures in each sequence, all faces of a cube map must be square and the same size.
			// * SPRITES: Sprite texture pages will be generated.
			// * INTERFACE BITMAPS: Similar to 2D TEXTURES, but without mipmaps and without the power of two restriction.
			TAG_ENUM(type, Enums::bitmap_group_type);

			////////////////////////////////////////////////////////////////
			// format
			// Format controls how pixels will be stored internally:
			// 
			// * COMPRESSED WITH COLOR-KEY TRANSPARENCY: DXT1 compression, uses 4 bits per pixel. 4x4 blocks of pixels are reduced to 2 colors and interpolated, alpha channel uses color-key transparency instead of alpha from the plate (all zero-alpha pixels also have zero-color).
			// * COMPRESSED WITH EXPLICIT ALPHA: DXT2/3 compression, uses 8 bits per pixel. Same as DXT1 without the color key transparency, alpha channel uses alpha from plate quantized down to 4 bits per pixel.
			// * COMPRESSED WITH INTERPOLATED ALPHA: DXT4/5 compression, uses 8 bits per pixel. Same as DXT2/3, except alpha is smoother. Better for smooth alpha gradients, worse for noisy alpha.
			// * 16-BIT COLOR: Uses 16 bits per pixel. Depending on the alpha channel, bitmaps are quantized to either r5g6b5 (no alpha), a1r5g5b5 (1-bit alpha), or a4r4g4b4 (>1-bit alpha).
			// * 32-BIT COLOR: Uses 32 bits per pixel. Very high quality, can have alpha at no added cost. This format takes up the most memory, however. Bitmap formats are x8r8g8b8 and a8r8g8b.
			// * MONOCHROME: Uses either 8 or 16 bits per pixel. Bitmap formats are a8 (alpha), y8 (intensity), ay8 (combined alpha-intensity) and a8y8 (separate alpha-intensity).
			// 
			// Note: Height maps (a.k.a. bump maps) should use 32-bit color; this is internally converted to a palettized format which takes less memory.
			TAG_ENUM(format, Enums::bitmap_group_format);

			////////////////////////////////////////////////////////////////
			// usage
			// Usage controls how mipmaps are generated:
			// 
			// * ALPHA BLEND: Pixels with zero alpha are ignored in mipmaps, to prevent bleeding the transparent color.
			// * DEFAULT: Downsampling works normally, as in Photoshop.
			// * HEIGHT MAP: The bitmap (normally grayscale) is a height map which gets converted to a bump map. Uses <bump height> below. Alpha is passed through unmodified.
			// * DETAIL MAP: Mipmap color fades to gray, controlled by <detail fade factor> below. Alpha fades to white.
			// * LIGHT MAP: Generates no mipmaps. Do not use!
			// * VECTOR MAP: Used mostly for special effects; pixels are treated as XYZ vectors and normalized after downsampling. Alpha is passed through unmodified.
			TAG_ENUM(usage, Enums::bitmap_group_usage);
			TAG_FIELD(word_flags, flags, Flags::bitmap_group_flags);

			////////////////////////////////////////////////////////////////
			// post-processing
			// These properties control how mipmaps are post-processed.
			TAG_FIELD(real_fraction, detail_fade_factor, "[0,1]", "0 means fade to gray by last mipmap, 1 means fade to gray by first mipmap");
			TAG_FIELD(real_fraction, sharpen_amount, "[0,1]", "sharpens mipmap after downsampling");
			TAG_FIELD(real_fraction, bump_height, "repeats", "the apparent height of the bump map above the triangle it is textured onto, in texture repeats (i.e., 1.0 would be as high as the texture is wide)");

			////////////////////////////////////////////////////////////////
			// sprite processing
			// When creating a sprite group, specify the number and size of textures that the group is allowed to occupy. During importing, you'll receive feedback about how well the alloted space was used.
			TAG_ENUM(sprite_budget_size, Enums::sprite_budget);
			TAG_FIELD(int16, sprite_budget_count);

			////////////////////////////////////////////////////////////////
			// color plate
			// The original TIFF file used to import the bitmap group.
			TAG_FIELD(int16, color_plate_width, "pixels");
			TAG_FIELD(int16, color_plate_height, "pixels");
			TAG_FIELD(tag_data, compressed_color_plate_data);

			////////////////////////////////////////////////////////////////
			// processed pixel data
			// Pixel data after being processed by the tool.
			TAG_FIELD(tag_data, processed_pixel_data);

			////////////////////////////////////////////////////////////////
			// miscellaneous
			TAG_FIELD(real, blur_filter_size, "[0,10] pixels", "blurs the bitmap before generating mipmaps");
			TAG_FIELD(real, alpha_bias, "[-1,1]", "affects alpha mipmap generation");
			TAG_FIELD(int16, mipmap_count, "levels", "0 defaults to all levels");

			////////////////////////////////////////////////////////////////
			// ...more sprite processing
			// Sprite usage controls the background color of sprite plates.
			TAG_ENUM(sprite_usage, Enums::sprite_usage);
			TAG_FIELD(int16, sprite_spacing);
			PAD16;
			TAG_TBLOCK(sequences, s_bitmap_group_sequence);
			TAG_TBLOCK_(bitmaps, s_bitmap_data);

			bool ResourcesAreSharable() const
			{
				return
					!TEST_FLAG(flags, Flags::_bitmap_group_never_share_resources_yelo_bit) &&
					// it makes no sense to ever store lightmaps in a shared cache
					usage != Enums::_bitmap_group_usage_light_map;
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_bitmap_group) == 0x6C );
	};
};