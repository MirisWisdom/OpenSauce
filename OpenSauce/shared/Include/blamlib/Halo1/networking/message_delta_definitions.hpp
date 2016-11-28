/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_bitstream;
	};

	namespace Enums
	{
#pragma region field_type
		enum field_type : long_enum
		{
			_field_type_integer,
			_field_type_real,
			_field_type_boolean,
			_field_type_ascii_character,
			_field_type_wide_character,
			_field_type_ascii_string,
			_field_type_wide_string,
			_field_type_arbitary_data,
			_field_type_array,
			_field_type_structure,
			_field_type_pointer,
			_field_type_enumeration,
			_field_type_bounded_index,
			_field_type_translated_index,


			/*!
			* \brief
			* real point 2d\3d
			*
			* parameter specifies if its 2d or 3d.
			* bit count = (parameter << 5) + parameter
			*/
			_field_type_point,

			/*!
			* \brief
			* real vector 2d\3d
			*
			* parameter specifies if its 2d or 3d.
			* bit count = (parameter << 5) + parameter
			*/
			_field_type_vector,
			_field_type_angle,
			_field_type_flags,
			_field_type_time,
			_field_type_grenade_counts,
			_field_type_fixed_width,
			_field_type_fixed_width_normal_vector,
			_field_type_locality_reference_position,
			_field_type_digital_throttle,
			_field_type_fixed_width_weapon_index,
			_field_type_fixed_width_grenade_index,
			_field_type_smart_vector,
			_field_type_item_placement_position,

			NUMBER_OF_MESSAGE_DELTA_FIELD_TYPES,
		};
#pragma endregion

#pragma region field_type_bit_size
		/*!
		* \brief
		* Bit sizes of some message delta fields
		* 
		* Each enumeration has the value of a message
		* delta's constant field bit count (unlike
		* the integer which has various bit sizes)
		*/
		enum field_type_bit_size
		{
			_field_type_bit_size_real = 32,
			_field_type_bit_size_boolean = 1,
			_field_type_bit_size_ascii_character = 8,
			_field_type_bit_size_wide_character = 16,
			_field_type_bit_size_angle = 32,
			_field_type_bit_size_time = 32,
			_field_type_bit_size_grenade_counts = 6,
			_field_type_bit_size_digital_throttle = 4,
			_field_type_bit_size_fixed_width_weapon_index = 3,
			_field_type_bit_size_fixed_width_grenade_index = 2,
			_field_type_bit_size_item_placement_position = 20 /*X*/ + 20 /*Y*/ + 30 /*Z*/,
		};
#pragma endregion

#pragma region integer_width_type
		enum integer_width_type : long_enum
		{
			_integer_width_type_8bits,
			_integer_width_type_16bits,
			_integer_width_type_32bits,
			_integer_width_type_1bit,
			_integer_width_type_3bits,
			_integer_width_type_5bits,
			_integer_width_type_6bits,
		};
#pragma endregion

		enum enumeration_width_type : long_enum
		{
			_enumeration_width_type_1byte,
			_enumeration_width_type_2byte,
			_enumeration_width_type_4byte,
		};

		enum message_delta_parameter_type : long_enum
		{
			_message_delta_parameter_type_real,
			_message_delta_parameter_type_long,
		};

		enum message_delta_mode : long_enum
		{
			_message_delta_mode_stateless, // no baselines
			_message_delta_mode_incremental, // requires baselines
		};

		enum message_delta_encoding_class : long_enum
		{
			_message_delta_encoding_class_lan,
			_message_delta_encoding_class_internet,
		};

		enum {
			k_mdp_maximum_string_length = 0x7FF,
			k_mdp_maximum_flags = 32,

			k_bits_in_grenade_counts_type = 6,
			k_bits_in_grenade_counts_type_upgrade = k_bits_in_grenade_counts_type * 2,
		};
	};

	namespace MessageDeltas
	{
		struct message_delta_parameter
		{
			cstring name;
			Enums::message_delta_parameter_type type;
			union {
				void* address;
				real* real;
				int32* integer;
			}value;
		}; BOOST_STATIC_ASSERT( sizeof(message_delta_parameter) == 0xC );

		// Essentially a hash table
		struct s_index_resolution_table
		{
			struct s_entry
			{
				datum_index key;// local_index
				int32 value;	// translated_index, what we get from the network
				s_entry* next;
			}; BOOST_STATIC_ASSERT( sizeof(s_entry) == 0xC );

			struct s_entry_pool
			{
				enum { k_entry_count = 50 };

				s_entry entries[k_entry_count];
			}; BOOST_STATIC_ASSERT( sizeof(s_entry_pool) == 0x258 );
			struct s_entry_pool_list
			{
				s_entry_pool* pool;
				s_entry* next_entry;
			}; BOOST_STATIC_ASSERT( sizeof(s_entry_pool_list) == 0x8 );

			struct s_slot
			{
				int32 count;
				s_entry* first_entry;
			}; BOOST_STATIC_ASSERT( sizeof(s_slot) == 0x8 );

			bool is_initialized; PAD24;
			int32 number_of_slots;
			s_slot* slots;						// [number_of_slots]
			int32 slots_in_use;
			s_entry* entry_free_list;
			s_entry_pool_list* entry_pool_list;
		};

#pragma region field_type_definition
		typedef int32 (PLATFORM_API* mdp_field_type_maximum_size_calculator)(struct field_properties_definition* properties_definition);
		typedef bool (PLATFORM_API* mdp_field_type_initialize)(struct field_properties_definition* properties_definition);
		typedef void (PLATFORM_API* mdp_field_type_dispose)(struct field_properties_definition* properties_definition);

		struct field_type_definition
		{
			Enums::field_type type;
			bool requires_parameters;
			PAD24;
			mdp_field_type_maximum_size_calculator proc_maximum_size_calculator;
			mdp_field_type_initialize proc_initialize;
			mdp_field_type_dispose proc_dispose;
			bool initialized;
			PAD24;
		};
#pragma endregion

#pragma region field_properties_definition
		// returns the amount of bits written
		// If [source_data] == [baseline_data], [source_data] isn't encoded
		typedef int32 (PLATFORM_API* mdp_field_encode)(struct field_properties_definition* field_properties, const void* baseline_data, const void* source_data, Memory::s_bitstream* output_stream);
		// returns the amount of bits read
		typedef int32 (PLATFORM_API* mdp_field_decode)(struct field_properties_definition* field_properties, void* baseline_data, void* destination_data, Memory::s_bitstream* input_stream);
		
		struct field_properties_definition
		{
			Enums::field_type type;
			char name[76];
			mdp_field_encode proc_encode;
			mdp_field_decode proc_decode;
			union field_type_definition_parameters* parameters;
			int32 maximum_size; ///< in bits
			int32 header_bit_size;
			bool initialized;
			PAD24;
		};
#pragma endregion

		struct field_reference
		{
			field_properties_definition* properties;
			uint32 offset; ///< in bytes
			uint32 baseline_offset; ///< in bytes
			bool initialized;
			PAD24;
		};

		struct field_reference_set
		{
			int32 field_count;
			int32 max_data_size; ///< in bits
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			field_reference fields[];
#pragma warning( pop )
		};

#pragma region field_type_definition_parameters
		struct field_type_translated_index_parameters {
			// On initialization:
			// NONE is registered with network_index '0'
			// [translated_index_allocations] are memset to NONE (datum_index::null)
			// First element of [translated_index_allocations] is set to '1'

			int32 maximum_active_at_once; // should be +1 of actual max, for the 'null' entry (which is always the first)
			int32 number_of_slots;
			int32 bits_needed;
			s_index_resolution_table table;
			int32 cursor;
			// allocations[translated_index] == local_index (eg, player_index)
			datum_index* translated_index_allocations; // [maximum_active_at_once]. These are the local indexes
			int32 peak;
			int32 codings;
			int32 none;
		};

		union field_type_definition_parameters // Note: engine code doesn't actually use unions, so be sure you're accessing the right structure!
		{
			struct {
				Enums::integer_width_type width;
			}integer;
			struct {
				int32 maximum_length;
			}ascii_string, wide_string;
			struct {
				int32 max_size;
			}arbitrary_data;
			struct {
				Enums::field_type width;
				PAD32; // unknown
				field_properties_definition* member_properties;
			}array;
			struct {
				int32 field_count;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
				field_reference members_references[];
#pragma warning( pop )
			}structure;
			struct {
				field_properties_definition* referent_properties;
			}pointer;
			struct {
				Enums::enumeration_width_type width;
			}enumeration;
			struct {
				int32 minimum_value;
				int32 maximum_value;
			}bounded_index;
			field_type_translated_index_parameters translated_index;
			struct {
				int32 component_count;
			}point, vector;
			struct {
				int32 count;
				bool valid_bits[Enums::k_mdp_maximum_flags]; // I guess
			}flags;
			struct {
				int32 number_of_bits;
				int32 range_of_values;
			}fixed_width;
			struct {
				int32 number_of_bits_theta_internet;
				int32 number_of_bits_phi_internet;
				int32 number_of_bits_theta_lan;
				int32 number_of_bits_phi_lan;
			}fixed_width_normal_vector;
			struct {
				real minimum_value;
				real maximum_value;
				int32 number_of_bits_theta_internet;
				int32 number_of_bits_phi_internet; // unused
				int32 number_of_bits_theta_lan;
				int32 number_of_bits_phi_lan; // unused
				int32 vectors_count;
				real_vector3d decode_vectors[32];
				real_vector3d encode_vectors[32];
			}smart_vector;
		};
#pragma endregion

#pragma region message_delta_definition
		struct message_delta_definition
		{
			long_enum definition_type;				// 0x0 [Enums::message_delta]
			int32 message_dependent_header_size;	// 0x4 body_size + body field count
			int32 iteration_size;					// 0x8
			int32 iteration_independent_header_size;// 0xC
			int32 total_size;						// 0x10
			int32 max_iterations;					// 0x14
			bool initialized;		PAD24;			// 0x18
			field_reference_set* header_field_set;	// 0x1C
			field_reference_set body_field_set;		// 0x20
		};
#pragma endregion


		struct decoding_information_data
		{
			Enums::message_delta_mode mode;		// 0x0
			long_enum definition_type;			// 0x4 [Enums::message_delta]
			int32 iteration_count;				// 0x8
			int32 state;						// 0xC
			Memory::s_bitstream* input_stream;	// 0x10
			int32 unknown1;						// 0x14
			int32 current_iteration;			// 0x18
			bool iteration_header_decoded;		// 0x1C
			bool iteration_body_decoded;		// 0x1D
			PAD16;
			UNKNOWN_TYPE(int32);	// 0x20, iteration_independent_overhead_type = 3
			UNKNOWN_TYPE(int32);	// 0x24, iteration_independent_overhead_type = 2
			UNKNOWN_TYPE(int32);	// 0x28, iteration_independent_overhead_type = 1
			UNKNOWN_TYPE(int32);	// 0x2C, iteration_independent_overhead_type = 0

			// This structure may have 8 more bytes of data, but I haven't seen them used in code yet...
		};

		struct message_dependant_header
		{
			decoding_information_data* decoding_information;
			bool body_field_exists[64];
			void* custom_header_destination; // max size 0x84
		}; BOOST_STATIC_ASSERT( sizeof(message_dependant_header) == 0x48 );
	};
};