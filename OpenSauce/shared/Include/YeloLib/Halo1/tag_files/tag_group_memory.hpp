/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_IS_EDITOR
#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	struct tag_field;
	struct tag_group;

	namespace Enums
	{
		enum {
			/// <summary>	Signals the end of a field comparison code list </summary>
			_field_comparison_code_end,
			/// <summary>	Signals that a block of bytes should be included from comparison (memcmp) </summary>
			_field_comparison_code_bitwise,
			/// <summary>	Signals that a block of bytes should be excluded from comparison </summary>
			_field_comparison_code_skip,
			/// <summary>	Signals that a uintptr_t comparison should be performed </summary>
			_field_comparison_code_pointer,
			/// <summary>	Signals that a tag_reference comparison should be performed </summary>
			_field_comparison_code_tag_reference,
			/// <summary>	Signals that a tag_data comparison should be performed </summary>
			_field_comparison_code_tag_data,
			/// <summary>	Signals that a tag_block comparison should be performed </summary>
			_field_comparison_code_block,
			/// <summary>	Signals that a block index comparison should be performed </summary>
			/// <remarks>	field_index is the block_index field's index. code size is the byte size of the field. </remarks>
			_field_comparison_code_block_index,
			_field_comparison_code_vertex_buffer,

			k_number_of_tag_field_comparison_code_types,

			/// <remarks>	number of types field_comparison_code's [type] bitfield can support </remarks>
			k_max_tag_field_comparison_code_types = 16,
		};
	};

	namespace TagGroups
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the set of all registered block definitions </summary>
		const std::unordered_set<tag_block_definition*>& GetBlockDefinitionsSet();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the set of all registered data definitions </summary>
		const std::unordered_set<tag_data_definition*>& GetDataDefinitionsSet();

		struct field_comparison_code;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Data generated at startup (like how byte swap codes are), not strictly related to
		/// 	'runtime' (ie, game) builds.
		/// </summary>
		struct s_tag_field_set_runtime_data
		{
			// these are, of course, not actual limits (tag system doesn't have any), but assumed.
			enum {
				k_max_direct_references = 31,
				/// <remarks>	doesn't include string_id_yelo fields. </remarks>
				k_max_tag_reference_fields = 255,	// dialogue has over 200
				k_max_block_fields = 63,			// scenario has over 50
				k_max_block_index_fields = 7,
				k_max_data_fields = 7,
				k_max_string_fields = 7,
				/// <remarks>	doesn't include old string id fields (implemented as tag_string). </remarks>
				k_max_string_id_fields = 15,

				k_max_padding_amount = 2048,
				k_max_comparison_codes = 256,
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
				unsigned has_debug_child_data : 1;	// fieldset has a field whose child data is only used in debug. this probably only applies to tag_data fields
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

			struct {
				// number of references to this fieldset (via block or block_index fields)
				unsigned references				: bitfield_size<k_max_direct_references>::value;
				unsigned tag_reference_fields	: bitfield_size<k_max_tag_reference_fields>::value;
				unsigned block_fields			: bitfield_size<k_max_block_fields>::value;
				unsigned block_index_fields		: bitfield_size<k_max_block_index_fields>::value;
				unsigned data_fields			: bitfield_size<k_max_data_fields>::value;
				unsigned string_fields			: bitfield_size<k_max_string_fields>::value;
				unsigned string_id_fields		: bitfield_size<k_max_string_id_fields>::value;
				// any more bitfields and we'll enter a new word boundary

				size_t padding_amount;		// total amount of 'pad' annotated in this field set
				size_t debug_data_amount;	// total amount of bytes that are used for debug information (definition pointers, etc)
			}counts;

			size_t runtime_size;			// should be set to element_size, even if there's no actual size difference at runtime
			field_comparison_code* comparison_codes;
			const tag_field* block_name_field;

			void Initialize(const tag_block_definition* group_header_definition, 
				const tag_block_definition* definition);
			void Dispose();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Returns true if the fieldset contains any fields which required optimized transformations
			/// 	for runtime builds This would be the case for string_id fields.
			/// </summary>
			bool ContainsRuntimeOptimizedFields() const;

			/// <summary>	Enable the is_group_header bit. </summary>
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
			void IncrementDebugDataSize(size_t size);

			void BuildInfoHandleDataField(tag_data_definition* definition);
			void BuildInfo(const tag_block_definition* group_header_definition, 
				const tag_block_definition* definition);
			void CallCheApeHooks(const tag_block_definition* definition);
			void BuildByteComparisonCodes(const tag_block_definition* definition);
			void DestroyByteComparisonCodes();

			void DumpThisToFile(FILE* file, const tag_block_definition* definition) const;
		public:
			/// <summary>	Dumps all interesting field set data to group_field_set_info.txt in the OS Reports folder. </summary>
			static void DumpToFile();

			static bool Enabled();
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_field_set_runtime_data) == 0x1C );

		struct field_comparison_code
		{
			BOOST_STATIC_ASSERT(Enums::k_number_of_tag_field_comparison_code_types <= Enums::k_max_tag_field_comparison_code_types);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The number of bytes involved in bitwise comparisons, else 0 </summary>
			unsigned size : bitfield_size<s_tag_field_set_runtime_data::k_max_padding_amount>::value;	// 12
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The kind of comparison code </summary>
			unsigned type : bitfield_enum_size<Enums::k_max_tag_field_comparison_code_types>::value;	// 4
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	The field index in the fieldset for non-bitwise comparisons </summary>
			signed field_index : 16;
		}; BOOST_STATIC_ASSERT(sizeof(field_comparison_code) == 0x4);


		struct s_tag_allocation_header // keep aligned to 16bytes
		{
			enum { k_signature = 'tahd' };

			tag signature;
			uint32 hash;		/// <summary>	hash of just the allocation. </summary>
			uint32 total_hash;	/// <summary>	hash of the allocation and then any child data. </summary>
			PAD32;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the body data of this allocation. </summary>
			///
			/// <returns>	. </returns>
			void* GetBody()
			{
				return this + 1;
			}

		private:
			void Initialize();
		public:
			void Initialize(const tag_block_definition* definition);
			void Initialize(const tag_data_definition* definition);

		private:
			friend class c_tag_block_allocations;
			friend class c_tag_data_allocations;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets header pointer from body address. </summary>
			///
			/// <param name="body_address">	If non-null, the body address. </param>
			///
			/// <returns>	The header pointer if body isn't NULL, else NULL. </returns>
			static s_tag_allocation_header* GetHeaderFromBody(void* body_address)
			{
				return body_address 
					? CAST_PTR(s_tag_allocation_header*, body_address) - 1
					: nullptr;
			}
			static s_tag_allocation_header* GetHeader(void* body_address, cstring body_definition_name);
		public:
			static bool Enabled();
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get the allocation header for the given block instance </summary>
			///
			/// <param name="instance">	The block whose allocation header we want </param>
			///
			/// <returns>	null if allocation headers are disabled, an invalid header is encountered, or the instance is has no elements </returns>
			static s_tag_allocation_header* Get(tag_block& instance);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get the allocation header for the given tag data instance </summary>
			///
			/// <param name="instance">	The tag data whose allocation header we want </param>
			///
			/// <returns>	null if allocation headers are disabled, an invalid header is encountered, or the instance is has no data </returns>
			static s_tag_allocation_header* Get(tag_data& instance);
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_allocation_header) == 0x10 );

		class c_tag_block_allocations
		{
			static size_t CalculateSize(const tag_block_definition* definition, int32 element_count);
		public:
			static void* New(const tag_block_definition* definition, int32 element_count, cstring file, uint32 line);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Reallocates the block field's memory, updating its count and address in the process. </summary>
			///
			/// <param name="instance"> 	[in,out] The tag_block instance. </param>
			/// <param name="new_count">	New element count. </param>
			/// <param name="file">	   		The caller's file. </param>
			/// <param name="line">	   		The caller's line. </param>
			///
			/// <returns>	reallocated elements address. </returns>
			static void* Realloc(tag_block& instance, int32 new_count, cstring file, uint32 line);
			static void Delete(void* elements, const tag_block_definition* definition, cstring file, uint32 line);
			static void Delete(tag_block& instance, cstring file, uint32 line);
		};
#define TAG_BLOCK_NEW(definition, element_count) \
	TagGroups::c_tag_block_allocations::New(definition, element_count, __FILE__, __LINE__)
#define TAG_BLOCK_REALLOC(block, new_count) \
	TagGroups::c_tag_block_allocations::Realloc(block, new_count, __FILE__, __LINE__)
#define TAG_BLOCK_DELETE(block) \
	TagGroups::c_tag_block_allocations::Delete(block, __FILE__, __LINE__)
#define TAG_BLOCK_DELETE_ELEMENTS(definition, elements) \
	TagGroups::c_tag_block_allocations::Delete(elements, definition, __FILE__, __LINE__)

		class c_tag_data_allocations
		{
			static int32 CalculateSize(const tag_data_definition* definition, int32 size);
		public:
			static void* New(const tag_data_definition* definition, int32 size, cstring file, uint32 line);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Reallocates the data field's memory, updating its size and address in the process. </summary>
			///
			/// <param name="instance">	[in,out] The tag_data instance. </param>
			/// <param name="new_size">	New size of the data. </param>
			/// <param name="file">	   	The caller's file. </param>
			/// <param name="line">	   	The caller's line. </param>
			///
			/// <returns>	reallocated data address. </returns>
			static void* Realloc(tag_data& instance, int32 new_size, cstring file, uint32 line);
			static void Delete(tag_data& instance, cstring file, uint32 line);
		};
#define TAG_DATA_NEW(definition, size) \
	TagGroups::c_tag_data_allocations::New(definition, size, __FILE__, __LINE__)
#define TAG_DATA_REALLOC(data, new_size) \
	TagGroups::c_tag_data_allocations::Realloc(data, new_size, __FILE__, __LINE__)
#define TAG_DATA_DELETE(data) \
	TagGroups::c_tag_data_allocations::Delete(data, __FILE__, __LINE__)


		/// <summary>	Represents the allocation statistics for tag group child data (ie, blocks and tag data) </summary>
		struct s_tag_allocation_statistics
		{
			const tag_block_definition* block_definition;
			const tag_data_definition* data_definition;

			struct s_block_totals {
				size_t debug_bytes_size;
				rsize_t count;		/// <summary>	number of instances. </summary>
				rsize_t elements;	/// <summary>	number of elements. </summary>
				size_t size;		/// <summary>	total size. </summary>
				size_t padding;		/// <summary>	total amount of padding, 0 when runtime_data isn't generated. </summary>
			};
			struct s_data_totals {
				size_t debug_bytes_size;
				rsize_t count;	/// <summary>	number of instances. </summary>
				size_t size;	/// <summary>	total size. </summary>
			};
			union { // totals
				s_block_totals block;
				s_data_totals data;
			};

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Are these stats for a block definition? </summary>
			bool IsBlock() const	{ return block_definition != nullptr; }
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Are these stats for a data definition? </summary>
			bool IsData() const	{ return data_definition != nullptr; }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes this object for a given block definition </summary>
			///
			/// <param name="definition">	Block definition these stats are for </param>
			///
			/// <returns>	This instance </returns>
			s_tag_allocation_statistics& Initialize(const tag_block_definition* definition);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes this object for a given data definition </summary>
			///
			/// <param name="definition">	Data definition these stats are for </param>
			///
			/// <returns>	This instance </returns>
			s_tag_allocation_statistics& Initialize(const tag_data_definition* definition);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates stats based on an instance of the block definition we're tracking </summary>
			void Update(const tag_block* instance);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Updates stats based on an instance of the data definition we're tracking </summary>
			void Update(const tag_data* instance);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Dumps block definition stats to a file. </summary>
			///
			/// <param name="file">			[in] If non-null, the file to print to. </param>
			/// <param name="group_totals">	[in,out] The owner group's stats totals to update </param>
			void DumpBlockInfoToFile(FILE* file, s_block_totals& group_totals) const;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Dumps data definition stats to a file. </summary>
			///
			/// <param name="file">			[in] If non-null, the file to print to. </param>
			/// <param name="group_totals">	[in,out] The owner group's stats totals to update </param>
			void DumpDataInfoToFile(FILE* file, s_block_totals& group_totals) const;
		};
		/// <summary>	Represents the allocation statistics for a tag group and its child data </summary>
		class c_tag_group_allocation_statistics
		{
			typedef std::vector<s_tag_allocation_statistics>
				children_array_t;

			tag m_group_tag;
			children_array_t m_children;

			c_tag_group_allocation_statistics(tag group_tag, const s_tag_field_set_runtime_data& header_info);
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the allocation stats for a given block definition in this tag group </summary>
			///
			/// <param name="instance">	Block instance to use for definition lookup </param>
			///
			/// <returns>	The child stats. </returns>
			s_tag_allocation_statistics& GetChildStats(const tag_block* instance);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the allocation stats for a given data definition in this tag group </summary>
			///
			/// <param name="instance">	Data instance to use for definition lookup </param>
			///
			/// <returns>	The child stats. </returns>
			s_tag_allocation_statistics& GetChildStats(const tag_data* instance);

			children_array_t::const_iterator begin() const	{ return m_children.cbegin(); }
			children_array_t::const_iterator end() const	{ return m_children.cend(); }

			typedef std::unordered_map<Yelo::tag, Yelo::TagGroups::c_tag_group_allocation_statistics>
				group_tag_to_stats_map_t;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Are group allocation statistics enabled? </summary>
			static bool Enabled();
			/// <summary>	Initializes the group allocation stats system for use </summary>
			static void Initialize();
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Shuts down the group allocation stats system (frees allocated memory, etc) </summary>
			static void Dispose();
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Resets the state for all group stats as if nothing has been recorded yet </summary>
			static void Reset();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get the allocation stats for a given tag group </summary>
			///
			/// <param name="group_tag">	The tag of the group to lookup </param>
			static c_tag_group_allocation_statistics& GetStats(tag group_tag);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets block stats. </summary>
			///
			/// <param name="tag_index">	Index of the tag which ultimately owns the block </param>
			/// <param name="instance"> 	Block instance to use for definition lookup </param>
			static s_tag_allocation_statistics& GetBlockStats(datum_index tag_index, const tag_block* instance);
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets data stats. </summary>
			///
			/// <param name="tag_index">	Index of the tag which ultimately owns the data field </param>
			/// <param name="instance"> 	Data instance to use for definition lookup </param>
			static s_tag_allocation_statistics& GetDataStats(datum_index tag_index, const tag_data* instance);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Dumps all tag group statistics to group_memory.txt in the OS Reports folder.
			/// </summary>
			static void DumpToFile();

		private:
			static const group_tag_to_stats_map_t& GroupStatistics();

			static size_t BuildStatsForTagChildBlockRecursive(c_tag_group_allocation_statistics& group_stats,
				tag_block* instance);
		public:
			static size_t BuildStatsForTag(datum_index tag_index);
		};
	};
};

#endif