/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum tag_field_set_runtime_flags : long_flags {
			_tag_field_set_runtime_has_tag_block_bit,		// fieldset contains 1+ tag_block fields
			_tag_field_set_runtime_has_tag_data_bit,		// fieldset contains 1+ tag_data fields
			_tag_field_set_runtime_has_tag_reference_bit,	// fieldset contains 1+ tag_reference fields
															// if the tag_reference field is actually a proxy for a string_id_yelo field, should we still count it?
			_tag_field_set_runtime_has_block_index_bit,		// fieldset contains 1+ block_index fields
			_tag_field_set_runtime_has_string_id_bit,		// fieldset contains 1+ string_id_yelo fields
			_tag_field_set_runtime_has_old_string_id_bit,	// fieldset contains 1+ tag_string fields annotated for string_id
			_tag_field_set_runtime_has_runtime_size_bit,	// fieldset's size for cache build is not the same as in tags build
			_tag_field_set_runtime_has_alignment_bit,		// fieldset has an explicit alignment_bit set
			_tag_field_set_runtime_no_shared_memory_bit,	// fieldset should not share memory with other bitwise matching data
			_tag_field_set_runtime_is_group_header_bit,		// fieldset represents the header_block_definition of a group
			_tag_field_set_runtime_is_block_indexed_bit,	// fieldset can be referenced by a block_index (eg, the scenario_object_name block would have this set)

			k_number_of_tag_field_set_runtime_flags,
		};
	};

	namespace TagGroups
	{
		struct s_tag_field_set_runtime_data
		{
			// these are, of course, not actual limits (tag system doesn't have any), but assumed.
			enum {
				k_max_direct_references = 31,
				k_max_tag_reference_fields = 31,
				k_max_block_fields = 31,
				k_max_block_index_fields = 31,
				k_max_data_fields = 15,

				k_max_padding_amount = 2048,
			};

			long_flags flags;
			size_t runtime_size;			// should be set to element_size, even if there's no actual size difference at runtime
			struct {
				// number of references to this fieldset (via block or block_index fields)
				unsigned references				: boost::static_log2<k_max_direct_references>::value;
				unsigned tag_reference_fields	: boost::static_log2<k_max_tag_reference_fields>::value;
				unsigned block_fields			: boost::static_log2<k_max_block_fields>::value;
				unsigned block_index_fields		: boost::static_log2<k_max_block_index_fields>::value;
				unsigned data_fields			: boost::static_log2<k_max_data_fields>::value;
				size_t padding_amount;		// total amount of 'pad' annotated in this field set
			}counts;
			// codes used to compare tag data. negative numbers mean bytes to skip, positive numbers are to compare
			// generally, only pointer-like data is skipped
			int16* comparison_codes;

			void Initialize(const tag_block_definition* definition, bool is_group_header = false);
			void Dispose();

		private:
			void DecrementRuntimeSize(size_t amount);
			void IncrementDirectReferenceCount();
			void IncrementReferenceCountFromBlockField();
			void IncrementReferenceCountFromBlockIndexField();
			void IncrementTagReferenceFieldCount();
			void IncrementBlockFieldCount();
			void IncrementBlockIndexFieldCount();
			void IncrementDataFieldCount();
			void IncrementTotalPaddingSize(size_t size);

			void BuildInfo(const tag_block_definition* definition);
			void BuildByteComparisonCodes(const tag_block_definition* definition);
			void DestroyByteComparisonCodes();
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_field_set_runtime_data) == 0x14 );

		struct s_tag_allocation_header // keep aligned to 16bytes
		{
			enum { k_signature = 'tahd' };

			tag signature;
			uint32 hash;		// hash of just the allocation
			uint32 total_hash;	// hash of the allocation and then any child data
			PAD32;

			static const s_tag_allocation_header* Get(const tag_block* instance);
			static const s_tag_allocation_header* Get(const tag_data* instance);
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_allocation_header) == 0x10 );

		struct s_tag_allocation_statistics
		{
			const tag_block_definition* block_definition;
			const tag_data_definition* data_definition;

			union { // totals
				struct {
					int32 count;	// number of instances
					int32 elements;	// number of elements
					int32 size;		// total size
					int32 padding;	// total amount of padding, NONE when runtime_data isn't generated
				}block;

				struct {
					int32 count;	// number of instances
					int32 size;		// total size
				}data;
			};

			void Initialize(const tag_block_definition* definition);
			void Initialize(const tag_data_definition* definition);

			void Update(const tag_block* instance);
			void Update(const tag_data* instance);
		};
		struct s_tag_group_allocation_statistics
		{
			tag group_tag;
		};
	};
};