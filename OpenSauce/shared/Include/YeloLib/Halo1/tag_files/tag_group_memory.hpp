/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	struct tag_field;
	struct tag_group;

	namespace TagGroups
	{
		typedef int16 comparison_code_t;

		// Data generated at startup, not strictly related to 'runtime' (ie, game) builds
		struct s_tag_field_set_runtime_data
		{
			// these are, of course, not actual limits (tag system doesn't have any), but assumed.
			enum {
				k_max_direct_references = 31,
				k_max_tag_reference_fields = 31, // doesn't include string_id_yelo fields
				k_max_block_fields = 31,
				k_max_block_index_fields = 31,
				k_max_data_fields = 15,
				k_max_string_fields = 15,
				k_max_string_id_fields = 15, // doesn't include old string id fields (implemented as tag_string)

				k_max_padding_amount = 2048,
				k_max_comparison_codes = 511,
			};

			struct {
				unsigned has_tag_block : 1;			// fieldset contains 1+ tag_block fields
				unsigned has_tag_data : 1;			// fieldset contains 1+ tag_data fields
				unsigned has_tag_reference : 1;		// fieldset contains 1+ tag_reference fields
													// if the tag_reference field is actually a proxy for a string_id_yelo field, should we still count it?
				unsigned has_block_index : 1;		// fieldset contains 1+ block_index fields
				unsigned has_string_id : 1;			// fieldset contains 1+ string_id_yelo fields
				unsigned has_old_string_id : 1;		// fieldset contains 1+ tag_string fields annotated for string_id
				unsigned has_runtime_size : 1;		// fieldset's size for cache build is not the same as in tags build
				unsigned has_alignment : 1;			// fieldset has an explicit alignment_bit set
				unsigned no_shared_memory : 1;		// fieldset should not share memory with other bitwise matching data (eg, it is volatile memory)
				unsigned is_group_header : 1;		// fieldset represents the header_block_definition of a group
				unsigned is_block_indexed : 1;		// fieldset can be referenced by a block_index (eg, the scenario_object_name block would have this set)
				unsigned custom_comparison : 1;		// fieldset's comparison codes are custom, and should be assumed to be statically allocated or managed elsewhere

				// For group headers only
				unsigned group_has_tag_block : 1;
				unsigned group_has_tag_data : 1;
				unsigned group_has_tag_reference : 1;
				unsigned group_has_block_index : 1;
				unsigned group_has_string_id : 1;
				unsigned group_has_old_string_id : 1;
			}flags;
			size_t runtime_size;			// should be set to element_size, even if there's no actual size difference at runtime
			struct {
				// number of references to this fieldset (via block or block_index fields)
				unsigned references				: boost::static_log2<k_max_direct_references>::value;
				unsigned tag_reference_fields	: boost::static_log2<k_max_tag_reference_fields>::value;
				unsigned block_fields			: boost::static_log2<k_max_block_fields>::value;
				unsigned block_index_fields		: boost::static_log2<k_max_block_index_fields>::value;
				unsigned data_fields			: boost::static_log2<k_max_data_fields>::value;
				unsigned string_fields			: boost::static_log2<k_max_string_fields>::value;
				unsigned string_id_fields		: boost::static_log2<k_max_string_id_fields>::value;
				// 4 bits left before we break into another word

				size_t padding_amount;		// total amount of 'pad' annotated in this field set
			}counts;
			// codes used to compare tag data. negative numbers mean bytes to skip, positive numbers are to compare
			// generally, only pointer-like data is skipped
			comparison_code_t* comparison_codes;
			const tag_field* block_name_field;

			void Initialize(const tag_block_definition* group_header_definition, 
				const tag_block_definition* definition);
			void Dispose();

			// Returns true if the fieldset contains any fields which required optimized transformations for runtime builds
			// This would be the case for string_id fields
			bool ContainsRuntimeOptimizedFields() const;

			// Enable the is_group_header bit
			void SetIsGroupHeader();
		private:
			void DecrementRuntimeSize(size_t amount);
			void IncrementDirectReferenceCount();
			void IncrementReferenceCountFromBlockField();
			void IncrementReferenceCountFromBlockIndexField();
			void IncrementTagReferenceFieldCount();
			void IncrementBlockFieldCount();
			void IncrementBlockIndexFieldCount();
			void IncrementDataFieldCount();
			void IncrementStringFieldCount();
			void IncrementStringIdFieldCount();
			void IncrementTotalPaddingSize(size_t size);

			void BuildInfo(const tag_block_definition* group_header_definition, 
				const tag_block_definition* definition);
			void CallCheApeHooks(const tag_block_definition* definition);
			void BuildByteComparisonCodes(const tag_block_definition* definition);
			void DestroyByteComparisonCodes();

		public:
			static bool Enabled();
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_field_set_runtime_data) == 0x18 );

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

			bool IsBlock() const	{ return block_definition != nullptr; }
			bool IsData() const		{ return data_definition != nullptr; }

			s_tag_allocation_statistics& Initialize(const tag_block_definition* definition);
			s_tag_allocation_statistics& Initialize(const tag_data_definition* definition);

			void Update(const tag_block* instance);
			void Update(const tag_data* instance);
		};
		class c_tag_group_allocation_statistics
		{
			typedef std::vector<s_tag_allocation_statistics> children_array_t;

			tag m_group_tag;
			children_array_t m_children;

			c_tag_group_allocation_statistics(tag group_tag, const s_tag_field_set_runtime_data& header_info);
		public:
			s_tag_allocation_statistics* GetChildStats(const tag_block* instance);
			s_tag_allocation_statistics* GetChildStats(const tag_data* instance);

			inline children_array_t::const_iterator begin() const	{ return m_children.cbegin(); }
			inline children_array_t::const_iterator end() const		{ return m_children.cend(); }

			static bool Enabled();
			static void Initialize();
			static void Dispose();

			static c_tag_group_allocation_statistics* GetStats(tag group_tag);
			static s_tag_allocation_statistics* GetBlockStats(datum_index tag_index, const tag_block* instance);
			static s_tag_allocation_statistics* GetDataStats(datum_index tag_index, const tag_data* instance);
		};
	};
};