/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_IS_EDITOR

#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>

//#define __TAG_FIELD_SCAN_USE_BLAM_DATA

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_tag_field_scan_stack_size = 16,
		};
	};

	namespace TagGroups
	{
		struct s_tag_field_scan_state
		{
			friend class c_tag_field_scanner;

			const tag_field* fields;
			void* block_element;
			long_flags field_types[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_tag_field_types)];
			int16 field_count;
			int16 field_size;			// NOTE: tag_field_scan_state_new doesn't initialize this or the next field
			// offset in the block_element where this field ends at
#ifdef __TAG_FIELD_SCAN_USE_BLAM_DATA
			int32 field_end_offset;		// not touched or used outside of tag_field_scan
#else // yelo-data
			uint16 field_end_offset;	
			uint16 fields_debug_bytes;	// Total number of tags-build only bytes
#endif
			// when the scanner finishes (ie, this is true), the field info will rest on the _field_terminator instance
			bool done;
			// NOTE: tag_field_scan_state_new doesn't initialize this (it's compiler-added padding)
			// In debug builds of CheApe , this will most likely equal 0xCC
			// In release builds this will be whatever was on the stack before. If it's magically 0x01, it could spell disaster for IsEngineScanState()
			byte pad;
			int16 stack_index;
			// NOTE: tag_field_scan_state_new doesn't initialize anything below
			struct {
				int16 field_index;
				int16 count;
			}stack[Enums::k_tag_field_scan_stack_size];

			const tag_field* field;
			void* field_address;

			// true if this scan state is allocated in engine code
			bool IsEngineScanState()	{ return pad != TRUE; }
		private:
			// initialize anything important which tag_field_scan_state_new doesn't
			void InitializeWhatNewDoesnt();
			// DON'T TOUCH ME UNLESS YOUR NAME IS c_tag_field_scanner
			void SetYeloScanState()		{ pad = TRUE; }
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_field_scan_state) == 0x64 );


		class c_tag_field_scanner
		{
			s_tag_field_scan_state m_state;

		public:
			bool IsDone() const					{ return m_state.done; }

			const tag_field* GetFields() const	{ return m_state.fields; }
			void* GetBlockElement() const		{ return m_state.block_element; }
			int32 GetFieldCount() const			{ return m_state.field_count; }

			int32 GetFieldIndex() const
			{
				assert(m_state.field_count > 0); // if this fails, scanning hasn't started yet, no one should be requesting an index yet
				return m_state.field_count - 1;
			}
			size_t GetFieldSize() const				{ return CAST(size_t, m_state.field_size); }
			size_t GetFieldOffset() const			{ return CAST(size_t, m_state.field_end_offset) - GetFieldSize(); }
#ifndef __TAG_FIELD_SCAN_USE_BLAM_DATA
			size_t GetFieldRuntimeOffset() const	{ return GetFieldOffset() - m_state.fields_debug_bytes; }
#endif

			Enums::field_type GetTagFieldType() const	{ return m_state.field->type; }
			cstring GetTagFieldName() const				{ return m_state.field->name; }
			template<typename T>
			T* GetTagFieldDefinition() const			{ return m_state.field->Definition<T>(); }

			void* GetFieldAddress() const				{ return m_state.field_address; }
			template<typename T>
			T* GetFieldAs() const						{ return CAST_PTR(T*, m_state.field_address); }

		public:
			c_tag_field_scanner(const tag_field* fields, void* block_element = nullptr);

			c_tag_field_scanner& AddFieldType(Enums::field_type field_type);

			c_tag_field_scanner& AddAllFieldTypes();

			bool Scan();
			c_tag_field_scanner& ScanToEnd();

			bool TagFieldIsStringId() const;
			bool TagFieldIsOldStringId() const;
			int32 StringFieldGetLength() const;


			// an interface to a found tag field and its definition
			struct s_iterator_result
			{
			private:
				const c_tag_field_scanner& m_scanner;

			public:
				s_iterator_result(const c_tag_field_scanner& scanner) : m_scanner(scanner) {}

				// Field's index, relative to the parent block's fields list
				int32 GetIndex() const				{ return m_scanner.GetFieldIndex(); }
				// Field's individual size
				size_t GetSize() const				{ return m_scanner.GetFieldSize(); }
				// Field's offset, relative to the parent block
				size_t GetOffset() const			{ return m_scanner.GetFieldOffset(); }
#ifndef __TAG_FIELD_SCAN_USE_BLAM_DATA
				// Field's cache-build offset, relative to the parent block
				size_t GetRuntimeOffset() const		{ return m_scanner.GetFieldRuntimeOffset(); }
#endif

				// Field's type
				Enums::field_type GetType() const	{ return m_scanner.GetTagFieldType(); }
				// Field's name
				cstring GetName() const				{ return m_scanner.GetTagFieldName(); }
				// Treat the field's definition as a T*
				template<typename T>
				T* DefinitionAs() const				{ return m_scanner.GetTagFieldDefinition<T>(); }

				// Instance's address
				void* GetAddress() const			{ return m_scanner.GetFieldAddress(); }
				// Treat the instance's address as a T*
				template<typename T>
				T* As() const						{ return m_scanner.GetFieldAs<T>(); }

				bool IsReadOnly() const				{ return m_scanner.m_state.field->IsReadOnly(); }
				bool IsAdvanced() const				{ return m_scanner.m_state.field->IsAdvanced(); }
				bool IsBlockName() const			{ return m_scanner.m_state.field->IsBlockName(); }
				bool IsInvisible() const			{ return m_scanner.m_state.field->IsInvisible(); }
				bool IsStringId() const				{ return m_scanner.TagFieldIsStringId(); }
				bool IsOldStringId() const			{ return m_scanner.TagFieldIsOldStringId(); }
				int32 GetStringFieldLength() const	{ return m_scanner.StringFieldGetLength(); }
			};

			struct s_iterator
			{
			private:
				c_tag_field_scanner* m_scanner;

				bool IsEndHack() const			{ return m_scanner == nullptr; }

			public:
				s_iterator(c_tag_field_scanner* scanner) : m_scanner(scanner) {}

				bool operator!=(const s_iterator& other) const;

				s_iterator& operator++()
				{
					m_scanner->Scan();
					return *this;
				}
				s_iterator_result operator*() const
				{
					return s_iterator_result(*m_scanner);
				}
			};

			s_iterator begin() /*const*/
			{
				auto iter = s_iterator(this);
				this->Scan();
				return iter;
			}
			static const s_iterator end() /*const*/
			{
				return s_iterator(nullptr);
			}
		};
	};

	namespace blam
	{
		TagGroups::s_tag_field_scan_state& PLATFORM_API tag_field_scan_state_new(TagGroups::s_tag_field_scan_state& state, 
			const tag_field* fields, void* block_element);
		void PLATFORM_API tag_field_scan_state_add_field_type(TagGroups::s_tag_field_scan_state& state, 
			Enums::field_type field_type);
		bool PLATFORM_API tag_field_scan(TagGroups::s_tag_field_scan_state& state);
	};
};

#endif