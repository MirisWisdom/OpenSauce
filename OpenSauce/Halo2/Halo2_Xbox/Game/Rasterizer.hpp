/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState
	{
		struct s_rasterizer_cinematic_globals : TStructImpl(1020)
		{
			// bool; // 0xAC
			// bool; // 0xAD

			// Depth of field shit
			// real; // 0xD8
			// real; // 0xDC
			// real; // 0xE0
			// real; // 0xE4
			// real; // 0xE8
			// real; // 0xEC
			// real; // 0xF0
			// real; // 0xF8

			// bool CossfadeOn; // 0x170
			// real CrossfadeUnk1; // 0x174
			// real CrossfadeUnk2; // 0x178
			// real CrossfadeValue; // 0x17C

			// real EffectValues[4]; // 0x180
			// real NearClipDistance; // 0x190
			// real FarClipDistance; // 0x194

			// bool FilterDesaturationTintOn; // 0x198
			// real_rgb_color FilterDesaturationTint; // 0x19C
			// real; // 0x1A8

			// bool FilterBitmapOn; // 0x1AC
			// datum_index FilterBitmap; // 0x1B0, has to be a bitmap with a type of _bitmap_type_cube_map

			// set all with function 715
			// bool On; // 0x1B4
			// real; // 0x1B8, 715 arg: 4, a?
			// real; // 0x1BC, 715 arg: 1, r?
			// real; // 0x1C0, 715 arg: 2, g?
			// real; // 0x1C4, 715 arg: 3, b?
			// real; // 0x1C8, 715 arg: 8, a?
			// real; // 0x1CC, 715 arg: 5, r?
			// real; // 0x1D0, 715 arg: 6, g?
			// real; // 0x1D4, 715 arg: 7, b?

			// set with function 717
			// {
			// real_bounds ?[32]; //0x1F8
			// real_bounds ?[32]; // 0x2F8
			// }
		};
		s_rasterizer_cinematic_globals* _RasterizerCinematicGlobals();

		struct s_rasterizer_unk_globals : TStructImpl(52)
		{
		};
		s_rasterizer_unk_globals* _RasterizerUnkGlobals();

		struct s_rasterizer_bloom_globals
		{
			bool Override;
			bool Reset;
			PAD16;
			real UNKNOWN(0);
			real OverrideThreshold;
			real OverrideBrightness;
			real OverrideTint;
			real OverrideModulation;
			bool Initialized;
			PAD24;
			real UNKNOWN(3);
			real UNKNOWN(4);
		};
		s_rasterizer_bloom_globals* _RasterizerBloomGlobals();

		struct s_weather_globals
		{
			int32 UNKNOWN(0);
			real UNKNOWN(1);
			real UNKNOWN(2);
			int32 UNKNOWN(3);
			int32 UNKNOWN(4);
			argb_color UNKNOWN(5);
		};
		s_weather_globals* _WeatherGlobals();
	};
};