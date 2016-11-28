/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_IS_EDITOR
// DON'T INCLUDE ME UNLESS YOUR NAME IS TagFieldSetReplacements.cpp

#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		// Fieldsets are replaced before anything major is done to tag groups. Eg, building group parents.
		// When group parents are built, the fields in the root block of the child groups are dynamically
		// allocated to be merged with the parent group(s)'s fields (and then later free'd)
		// So we cannot use dynamic memory allocation, as we would be stepping on the toes of this process.
		// Instead, we use statically allocated field arrays, to which the block definitions will reference.
		// This static memory will only be 'wasted' when used to replace child group fields (eg, in weapon_block).
		template<	typename TFieldSetType,	// unreferenced. Only used as a 'key' to keep the memory unique to each block type
											// Use the code definition of block definition to be replaced, eg TagGroups::s_bitmap_data
					size_t kMaxFieldCount
		>
		class c_tag_field_set_replacement_memory
		{
			static size_t g_new_fields_count;
			static std::array<tag_field, kMaxFieldCount> g_new_fields;

		public:
			size_t GetMaxFieldCount() const				{ return kMaxFieldCount; }

			size_t GetFieldCount() const				{ return g_new_fields_count; }
			// for c_tag_field_set_replacement_builder only. fuck friend class
			size_t& GetFieldCountRef()					{ return g_new_fields_count; }

			tag_field* GetFields() const				{ return g_new_fields.data(); }
			tag_field& operator[](size_t index) const	{ return g_new_fields[index]; }
		};
		template<typename TFieldSetType, size_t kMaxFieldCount>
		size_t									c_tag_field_set_replacement_memory<TFieldSetType, kMaxFieldCount>::g_new_fields_count;
		template<typename TFieldSetType, size_t kMaxFieldCount>
		std::array<tag_field, kMaxFieldCount>	c_tag_field_set_replacement_memory<TFieldSetType, kMaxFieldCount>::g_new_fields;


		// The builder currently only supports inserting fields where 'useless padding' exists in block data
		// Contrary to the class name, replacing existing fields is not supported currently (such operations can be done via a fixup in CheApe.map).
		// Field arrays (_field_array_start, _field_array_end) are NOT supported.
		class c_tag_field_set_replacement_builder
		{
			const tag_block_definition* m_source_definition;	// only used for verification information

			tag_field* m_target_fields;
			size_t m_target_fields_cursor;						// current index into [m_target_fields]
			size_t& m_target_fields_count;						// should reference [c_tag_field_set_replacement_memory.g_new_fields_count]
			size_t m_target_fields_max_count;					// should equal [c_tag_field_set_replacement_memory.kMaxFieldCount]

			struct {
				// true whenever a field insertion fails. resets on UselessPaddingBegin() calls
				unsigned insertion_failed : 1;
			}m_flags;

			struct {
				int32 detected_pad_field_first_index;			// index in the source fields of the first pad field
				size_t detected_pad_field_count;				// number of pad fields found in source fields when UselessPaddingBegin() is executed
				size_t detected_pad_size;						// amount of padding bytes accounted for in the pad fields

				size_t untapped_offset;							// offset of the useless padding bytes left for field insertion
				size_t untapped_useless_size;					// amount of useless padding bytes left for field insertion
			}m_padding;

			c_tag_field_scanner m_source_scanner;				// scanner for m_source_definition's fields, for field size/offset interop

			size_t m_total_fields_size;							// total field size of what has been copied to the target fields

		private:
			const tag_field* GetSourceFields() const;
			int32 GetSourceFieldIndex() const;
			size_t GetSourceFieldOffset() const;
			bool NotEndOfSourceFields() const;
			bool SourceFieldMatchesName(cstring field_name) const;

			bool NotEndOfTargetFields() const;
			void CopyFieldToTargetFields(const tag_field* field);

			void CopySourceField(int32 index);
			// Goto the next source field, optionally copying the current one to the current target field
			bool NextSourceField(bool copy_current_field_first);

			// Scans forward, starting at the current source field, for pad fields
			// Returns false if no padding fields are found, otherwise true
			bool DetectPaddingInfo();

			c_tag_field_set_replacement_builder(
				const tag_block_definition* source_definition,
				tag_field* target_fields, size_t target_fields_max_count, size_t& target_fields_count_ref);
		public:
			~c_tag_field_set_replacement_builder();

			template<typename TFieldSetType, size_t kMaxFieldCount>
			c_tag_field_set_replacement_builder(const tag_block_definition* source_definition,
				c_tag_field_set_replacement_memory<TFieldSetType, kMaxFieldCount> memory)
				// delegating ctor
				: c_tag_field_set_replacement_builder(source_definition,
					memory.GetFields(), memory.GetMaxFieldCount(), memory.GetFieldCountRef())
			{
			}

			void VerifyTotalFieldsSize(size_t expected_size);
			tag_field* Finish();

			void CopyFieldsFromSource(size_t field_count);
			void SkipFieldsFromSource(size_t field_count);

			// Copies the source fields to target fields, stopping after the first field named [field_name]
			// Returns the number of fields copied. Zero should indicate an error
			size_t CopyFieldsFromSourceUpTo(cstring field_name);
			// Skips source fields, stopping after the first field named [field_name]
			// Returns the number of fields skipped. Zero should indicate an error
			size_t SkipFieldsFromSourceUpTo(cstring field_name);

			// Begin an expected useless padding region in the source fields
			// [pad_size] is the amount of requested padding to overwrite, or 0 to implicitly consume pad fields found in the source fields
			void UselessPaddingBegin(size_t pad_size = 0);
			// End an expected useless padding region in the source fields
			// Returns true if all insertion operations since the initial UselessPaddingBegin() call were successful
			// [ending_pad_size] is the amount of assumed useless padding left over after any fields have been inserted after the initial UselessPaddingBegin
			bool UselessPaddingEnd(_Out_opt_ size_t* ending_pad_size = nullptr);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds an explanation to 'description'. </summary>
			///
			/// <param name="name">		  	Explanation name (the bold text). </param>
			/// <param name="description">	Explanation details. Use empty string for nothing, not NULL </param>
			///
			/// <returns>	this </returns>
			/// <remarks>	If any of the parameters are bad, this will log a warning but won't invalidate the builder </remarks>
			c_tag_field_set_replacement_builder& AddExplanation(cstring name, cstring description = "");

			c_tag_field_set_replacement_builder& InsertField(size_t expected_offset, 
				Enums::field_type type, cstring name, void* definition = nullptr);

		private:
			c_tag_field_set_replacement_builder& InsertEnum(size_t expected_offset,
				size_t enum_size, cstring name, string_list* enum_definition);
			c_tag_field_set_replacement_builder& InsertFlags(size_t expected_offset,
				size_t flags_size, cstring name, string_list* flags_definition);
			c_tag_field_set_replacement_builder& InsertBlockIndex(size_t expected_offset,
				size_t index_size, cstring name, tag_block_definition* block_definition);
		public:
			template<typename TEnum>
			c_tag_field_set_replacement_builder& InsertEnum(size_t expected_offset,
				cstring name, string_list* enum_definition)
			{
				BOOST_STATIC_ASSERT(sizeof(TEnum)==sizeof(_enum)
									);

				return InsertEnum(expected_offset, 
					sizeof(TEnum), name, enum_definition);
			}
			template<typename TFlags>
			c_tag_field_set_replacement_builder& InsertFlags(size_t expected_offset,
				cstring name, string_list* flags_definition)
			{
				BOOST_STATIC_ASSERT(sizeof(TFlags)==sizeof(byte_flags) || 
									sizeof(TFlags)==sizeof(word_flags) ||
									sizeof(TFlags)==sizeof(long_flags) );

				return InsertFlags(expected_offset,
					sizeof(TFlags), name, flags_definition);
			}
			template<typename TBlockIndex>
			c_tag_field_set_replacement_builder& InsertBlockIndex(size_t expected_offset,
				cstring name, tag_block_definition* block_definition)
			{
				BOOST_STATIC_ASSERT(sizeof(TBlockIndex)==sizeof(int16) || 
									sizeof(TBlockIndex)==sizeof(int32) );

				return InsertBlockIndex(expected_offset, 
					sizeof(TBlockIndex), name, block_definition);
			}

			c_tag_field_set_replacement_builder& InsertStringId(size_t expected_offset,
				cstring name);

			c_tag_field_set_replacement_builder& InsertBlock(size_t expected_offset,
				cstring name, tag_block_definition* block_definition);

			c_tag_field_set_replacement_builder& InsertData(size_t expected_offset,
				cstring name, tag_data_definition* data_definition);

			c_tag_field_set_replacement_builder& InsertTagReference(size_t expected_offset,
				cstring name, tag_reference_definition* reference_definition);

			c_tag_field_set_replacement_builder& InsertPadData(size_t expected_offset,
				int32 padding_size, cstring name = nullptr);
			c_tag_field_set_replacement_builder& InsertPostprocessedData(size_t expected_offset,
				int32 postprocessed_data_size, cstring name);
			c_tag_field_set_replacement_builder& InsertSkipData(size_t expected_offset,
				int32 skip_size, cstring name = nullptr);
		};
	};
};

#define FIELDSET_REPLACEMENT_BEGIN(field_set_type, max_fields, source_definition) {								\
	assert(sizeof(field_set_type) == source_definition->element_size);											\
	typedef field_set_type field_set_type_t;																	\
	typedef c_tag_field_set_replacement_memory<field_set_type_t, max_fields> field_set_replacement_memory_t;	\
	tag_block_definition* field_set_source_definition = source_definition;										\
	c_tag_field_set_replacement_builder fsr_builder(source_definition, field_set_replacement_memory_t());

#define FIELDSET_REPLACEMENT_END()										\
	field_set_source_definition->fields = fsr_builder.Finish();			\
	fsr_builder.VerifyTotalFieldsSize(sizeof(field_set_type_t));		\
}

#define FIELDSET_SEEK_COUNT_WITH_COPY(count) fsr_builder.CopyFieldsFromSource(count);
#define FIELDSET_SEEK_COUNT_WITH_SKIP(count) fsr_builder.SkipFieldsFromSource(count);

#define FIELDSET_SEEK_AFTER_WITH_COPY(name) fsr_builder.CopyFieldsFromSourceUpTo(name);
#define FIELDSET_SEEK_AFTER_WITH_SKIP(name) fsr_builder.SkipFieldsFromSourceUpTo(name);

#define FIELDSET_ADD_EXPLANATION(name, ...) fsr_builder.AddExplanation(name, __VA_ARGS__);

#define FIELDSET_INSERT_BEGIN(...) fsr_builder.UselessPaddingBegin(__VA_ARGS__);
#define FIELDSET_INSERT_END(...) fsr_builder.UselessPaddingEnd(__VA_ARGS__);

#define FIELDSET_INSERT(field_cname, type, name, ...) fsr_builder.				\
	InsertField(																\
		offsetof(field_set_type_t, field_cname), type, name, __VA_ARGS__);

#define FIELDSET_INSERT_ENUM(field_cname, name, definition) fsr_builder.		\
	InsertEnum<decltype(field_set_type_t::field_cname)>(						\
		offsetof(field_set_type_t, field_cname), name, definition);

#define FIELDSET_INSERT_FLAGS(field_cname, name, definition) fsr_builder.		\
	InsertFlags<decltype(field_set_type_t::field_cname)>(						\
		offsetof(field_set_type_t, field_cname), name, definition);

#define FIELDSET_INSERT_BLOCK_INDEX(field_cname, name, definition) fsr_builder.	\
	InsertBlockIndex<decltype(field_set_type_t::field_cname)>(					\
		offsetof(field_set_type_t, field_cname), name, definition);

#define FIELDSET_INSERT_STRING_ID(field_cname, name) fsr_builder.				\
	InsertStringId(																\
		offsetof(field_set_type_t, field_cname), name);

#define FIELDSET_INSERT_BLOCK(field_cname, name, definition) fsr_builder.		\
	InsertBlock(																\
		offsetof(field_set_type_t, field_cname), name, definition);

#define FIELDSET_INSERT_DATA(field_cname, name, definition) fsr_builder.		\
	InsertData(																	\
		offsetof(field_set_type_t, field_cname), name, definition);

#define FIELDSET_INSERT_TAG_REF(field_cname, name, definition) fsr_builder.		\
	InsertTagReference(															\
		offsetof(field_set_type_t, field_cname), name, definition);

// Insert a padding field that follows [field_cname]
// [offset] is how many bytes after [field_cname] ends where the padding begins
#define FIELDSET_INSERT_PAD(field_cname, offset, size) fsr_builder.				\
	InsertPadData(sizeof( decltype(field_set_type_t::field_cname) )	+ offset +	\
		offsetof(field_set_type_t, field_cname), size);

// Document [field_cname] as being a postprocessed (ie, runtime-only) field
// Requires a non-NULL [name] (could use an empty string, I don't care)
#define FIELDSET_INSERT_POSTPROCESSED(field_cname, name) fsr_builder.			\
	InsertPostprocessedData(													\
		offsetof(field_set_type_t, field_cname), sizeof( decltype(field_set_type_t::field_cname), name ));

// Insert a skip field for the field named [field_cname]
#define FIELDSET_INSERT_SKIP(field_cname) fsr_builder.							\
	InsertSkipData(																\
		offsetof(field_set_type_t, field_cname), sizeof( decltype(field_set_type_t::field_cname) ));

#endif