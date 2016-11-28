/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum sound_definition_flags : long_flags
		{
			_sound_definition_fit_to_adpcm_blocksize_bit,
			_sound_definition_split_long_sound_into_permutations_bit,

			k_number_of_sound_definition_flags,

			_sound_definition_reserved2_bit =	// Halo2. always spatialize#always play as 3d sound, even in first person
				k_number_of_sound_definition_flags,
			_sound_definition_reserved3_bit,	// Halo2. never obstruct#disable occlusion/obstruction for this sound
			_sound_definition_reserved4_bit,	// Halo2. internal don't touch
			_sound_definition_reserved5_bit,	// Halo2. use huge sound transmission
			_sound_definition_reserved6_bit,	// Halo2. link count to owner unit
			_sound_definition_reserved7_bit,	// Halo2. pitch range is language
			_sound_definition_reserved8_bit,	// Halo2. don't use sound class speaker flag
			_sound_definition_reserved9_bit,	// Halo2. don't use lipsync data

			// the sound definition's data should never be stored in any sort of 'shared' cache file
			_sound_definition_never_share_resources_yelo_bit,

			k_number_of_sound_definition_flags_yelo,
		};
	};

	namespace TagGroups
	{
		struct s_sound_permutation
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real, skip_fraction);
			TAG_FIELD(real, gain);
			TAG_FIELD(_enum, compression);
			TAG_FIELD(int16, next_permutation_index);

			datum_index cache_block_index;		// 0x2C
			void* cache_base_address;			// 0x30
			datum_index cache_tag_index;		// 0x34
			//int32 unused0;						// 0x38, used on the pc. size of the decompressed xbox adpcm samples buffer (cache_base_address)
			//int32 unused1;						// 0x3C, used on the pc. owner sound tag index
			int32 decompressed_buffer_size;
			datum_index owner_tag_index;

			// samples' tag_data flags
			enum { _samples_in_data_file_bit = 0 }; // data is in the sounds data file, not the cache file
			TAG_FIELD(tag_data, samples);

			TAG_FIELD(tag_data, mouth_data);
			TAG_FIELD(tag_data, subtitle_data);
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_permutation) == 0x7C );

		struct s_sound_pitch_range
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real, natural_pitch);
			TAG_FIELD(real_bounds, bend_bounds);
			TAG_FIELD(int16, actual_permutation_count);
			PAD16;
			TAG_PAD(tag_block, 1);
			TAG_TBLOCK_(permutations, s_sound_permutation);
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_pitch_range) == 0x48 );

		struct s_sound_definition_playback_parameters
		{
			TAG_FIELD(real_bounds, audible_distance);
			TAG_FIELD(real, skip_fraction);
			TAG_FIELD(real_bounds, random_pitch_bounds);
			TAG_FIELD(angle_bounds, cone_angles); // inner, outer
			TAG_FIELD(real, outer_cone_gain);
			TAG_FIELD(real, gain_modifier);
			TAG_FIELD(real, max_bend_per_second);
		};
		struct s_sound_definition_scale_modifiers
		{
			TAG_FIELD(real, skip_fraction_modifier);
			TAG_FIELD(real, gain_modifier);
			TAG_FIELD(real, pitch_modifier);
		};
		struct s_sound_promotion_parameters
		{
			TAG_FIELD(tag_reference, sound, "snd!");
			TAG_FIELD(int16, count);
			PAD16;
		};
		struct sound_definition
		{
			enum { k_group_tag = 'snd!' };

			TAG_FIELD(long_flags, flags, Flags::sound_definition_flags);
			TAG_FIELD(_enum, sound_class);
			TAG_FIELD(_enum, sample_rate);

			TAG_FIELD(s_sound_definition_playback_parameters, playback_parameters);
			// unused, when-scale-is-ZERO, unused, when-scale-is-ONE, unused
			TAG_FIELD(s_sound_definition_scale_modifiers, scale_modifiers)[5];

			TAG_FIELD(_enum, encoding);
			TAG_FIELD(_enum, compression);

			TAG_FIELD(s_sound_promotion_parameters, promotion_parameters);

			TAG_PAD(tag_data, 1);
			TAG_TBLOCK(pitch_ranges, s_sound_pitch_range);

			bool ResourcesAreSharable() const
			{
				return !TEST_FLAG(flags, Flags::_sound_definition_never_share_resources_yelo_bit);
			}
		}; BOOST_STATIC_ASSERT( sizeof(sound_definition) == 0xA4 );


		struct looping_sound_definition
		{
			enum { k_group_tag = 'lsnd' };

			TAG_FIELD(long_flags, flags);
			struct s_scale_function
			{
				TAG_FIELD(real, detail_sound_period);
				UNKNOWN_TYPE(real); // postprocessed field
				UNKNOWN_TYPE(real); // postprocessed field
			}	scale_zero,
				scale_one;

			datum_index runtime_scripting_sound_index;
			real runtime_maximum_audible_distance;
			PAD32;
			PAD32;

			TAG_FIELD(tag_reference, continuous_damage_effect, "cdmg");
			TAG_BLOCK(tracks, looping_sound_track);
			TAG_BLOCK(detail_sounds, looping_sound_detail);
		}; BOOST_STATIC_ASSERT( sizeof(looping_sound_definition) == 0x54 );
	};
};