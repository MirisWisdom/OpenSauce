/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blam/cseries/cseries_base.hpp>
#include <blam/cseries/enum_templates.h>
#include <blam/math/real_math.hpp>
#include <blam/memory/datum_index.hpp>
#include <blam/tag_files/tag_groups_base.hpp>
#include <yelo/tag_files/tag_groups_base_yelo.hpp>
#include <yelo/tag_files/tag_groups_markup.hpp>
#include <blam/tag_files/tag_data.h>

namespace Yelo
{
	namespace TagGroups
	{
		namespace e_bitmap_type
		{
			typedef enum : _enum
			{
				_2d_texture,
				_3d_texture,
				cube_map,
				white,

				k_count
			} type_t;
		}

		namespace e_bitmap_pixel_format
		{
			typedef enum : _enum
			{
				a8,
				y8,
				ay8,
				a8y8,
				unused1,
				unused2,
				r5g6b5,
				unused3,
				a1r5g5b5,
				a4r4g4b4,
				x8r8g8b8,
				a8r8g8b8,
				unused4,
				unused5,
				dxt1,
				dxt3,
				dxt5,
				p8_bump,

				k_count
			} type_t;
		}

		namespace e_bitmap_group_type
		{
			typedef enum : _enum
			{
				_2d_textures,
				_3d_textures,
				cube_maps,
				sprites,
				interface_bitmaps,

				k_count
			} type_t;
		}

		namespace e_bitmap_group_format
		{
			typedef enum : _enum
			{
				compressed_with_color_key_transparency,
				compressed_with_explicit_alpha,
				compressed_with_interpolated_alpha,
				_16_bit_color,
				_32_bit_color,
				monochrome,

				k_count
			} type_t;
		}

		namespace e_bitmap_group_usage
		{
			typedef enum : _enum
			{
				alpha_blend,
				default,
				height_map,
				detail_map,
				light_map,
				vector_map,

				k_count
			} type_t;
		}

		namespace e_sprite_budget_size
		{
			typedef enum : _enum
			{
				_32x32,
				_64x64,
				_128x128,
				_256x256,
				_512x512,

				k_count
			} type_t;
		}

		namespace e_sprite_bitmap_group_usage
		{
			typedef enum : _enum
			{
				blend_add_subtract_max,
				multiply_min,
				double_multiply,

				k_count
			} type_t;
		}

		namespace e_bitmap_group_flags
		{
			typedef enum : word_flags
			{
				enable_diffusion_dithering_bit,
				disable_height_map_compression_bit,
				uniform_sprite_sequences_bit,
				filthy_sprite_bug_fix_bit,

				k_count,

				reserved4_bit = k_count,
				unused5_bit,
				reserved6_bit,
				reserved7_bit,
				reserved8_bit,
				reserved9_bit,
				reserved10_bit,
				reserved11_bit,
				reserved12_bit,

				// the bitmap group's data should never be stored in any sort of 'shared' cache file
				never_share_resources_yelo_bit,

				k_count_yelo,
			} type_t;

			typedef flags_type<type_t> flags_t;
		};

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
		};

		// max count: 64
		static_assert(sizeof(s_bitmap_group_sprite) == 0x20, "s_bitmap_group_sprite should be 32 bytes in length");

		struct s_bitmap_group_sequence
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, first_bitmap_index);
			TAG_FIELD(int16, bitmap_count);
			TAG_PAD(int32, 4);
			TAG_TBLOCK(sprites, s_bitmap_group_sprite);
		};

		// max count: 256
		static_assert(sizeof(s_bitmap_group_sequence) == 0x40, "s_bitmap_group_sequence should be 64 bytes in length");

		struct s_bitmap_data
		{
			struct __flags
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
			};

			static_assert(sizeof(__flags) == sizeof(word_flags), "__flags should be the same size as word_flags");

			TAG_FIELD(tag, signature);
			TAG_FIELD(int16, width, "pixels");
			TAG_FIELD(int16, height, "pixels");
			TAG_FIELD(int16, depth, "pixels", "depth is 1 for 2D textures and cube maps");
			TAG_ENUM(type, e_bitmap_type::type_t, "determines bitmap 'geometry'");
			TAG_ENUM(format, e_bitmap_pixel_format::type_t, "determines how pixels are represented internally");
			TAG_FIELD(__flags, flags);
			TAG_FIELD(point2d, registration_point);
			TAG_FIELD(int16, mipmap_count);
			PAD16;
			TAG_FIELD(int32, pixels_offset);
			UNKNOWN_TYPE(int32);
			datum_index owner_tag_index;
			datum_index texture_cache_index;
			void* hardware_format; // IDirect3DBaseTexture9*
			void* base_address;
		};

		// max count: 2048
		static_assert(sizeof(s_bitmap_data) == 0x30, "s_bitmap_data should be 48 bytes in length");

		struct s_bitmap_definition
		{
			enum { k_group_tag = 'bitm' };

			struct __flags
			{
				TAG_FLAG16(enable_diffusion_dithering);
				TAG_FLAG16(disable_height_map_compression);
				TAG_FLAG16(uniform_sprite_sequences);
				TAG_FLAG16(filthy_sprite_bug_fix);
				TAG_FLAG16(reserved);
				TAG_FLAG16(reserved_1);
				TAG_FLAG16(reserved_2);
				TAG_FLAG16(reserved_3);
				TAG_FLAG16(reserved_4);
				TAG_FLAG16(reserved_5);
				TAG_FLAG16(reserved_6);
				TAG_FLAG16(reserved_7);
				TAG_FLAG16(reserved_8);
				TAG_FLAG16(never_share_resources);
			};

			static_assert(sizeof(__flags) == sizeof(word_flags), "__flags should be the same size as word_flags");


			////////////////////////////////////////////////////////////////
			// type
			// Type controls bitmap 'geometry'. All dimensions must be a power of two except for SPRITES and INTERFACE BITMAPS:
			// 
			// * 2D TEXTURES: Ordinary, 2D textures will be generated.
			// * 3D TEXTURES: Volume textures will be generated from each sequence of 2D texture 'slices'.
			// * CUBE MAPS: Cube maps will be generated from each consecutive set of six 2D textures in each sequence, all faces of a cube map must be square and the same size.
			// * SPRITES: Sprite texture pages will be generated.
			// * INTERFACE BITMAPS: Similar to 2D TEXTURES, but without mipmaps and without the power of two restriction.
			TAG_ENUM(type, e_bitmap_type::type_t);

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
			TAG_ENUM(format, e_bitmap_pixel_format::type_t);

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
			TAG_ENUM(usage, e_bitmap_group_usage::type_t);
			TAG_FIELD(__flags, flags);

			////////////////////////////////////////////////////////////////
			// post-processing
			// These properties control how mipmaps are post-processed.
			TAG_FIELD(real_fraction, detail_fade_factor, "[0,1]",
				"0 means fade to gray by last mipmap, 1 means fade to gray by first mipmap");
			TAG_FIELD(real_fraction, sharpen_amount, "[0,1]", "sharpens mipmap after downsampling");
			TAG_FIELD(real_fraction, bump_height, "repeats",
				"the apparent height of the bump map above the triangle it is textured onto, in texture repeats (i.e., 1.0 would be as high as the texture is wide)"
			);

			////////////////////////////////////////////////////////////////
			// sprite processing
			// When creating a sprite group, specify the number and size of textures that the group is allowed to occupy. During importing, you'll receive feedback about how well the alloted space was used.
			TAG_ENUM(sprite_budget_size, e_sprite_budget_size::type_t);
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
			TAG_ENUM(sprite_bitmap_group_usage, e_sprite_bitmap_group_usage::type_t);
			TAG_FIELD(int16, sprite_spacing);
			PAD16;
			TAG_TBLOCK(sequences, s_bitmap_group_sequence);
			TAG_TBLOCK_(bitmaps, s_bitmap_data);
		};

		// max count: 1
		static_assert(sizeof(s_bitmap_definition) == 0x6C, "s_bitmap_definition should be 108 bytes in length");
	}
}
