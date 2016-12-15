/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/byte_swapping_base.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_tag_field_set_runtime_data; // yelo-based hacks, not blam
	};

	namespace Enums
	{
		enum {
			k_maximum_field_byte_swap_codes = 1024,

			// these chars should all match the TAG_FIELD_MARKUP_* defines in tag_groups_structures_macros.hpp
			
			k_tag_field_markup_character_advanced =		'!',
			k_tag_field_markup_character_help_prefix =	'#',
			k_tag_field_markup_character_read_only =	'*',
			k_tag_field_markup_character_units_prefix =	':',
			k_tag_field_markup_character_block_name =	'^',
		};
		enum field_type : _enum
		{
			_field_string,
			_field_char_integer,
			_field_short_integer,
			_field_long_integer,
			_field_angle,
			_field_tag,
			_field_enum,
			_field_long_flags,
			_field_word_flags,
			_field_byte_flags,
			_field_point_2d,
			_field_rectangle_2d,
			_field_rgb_color,
			_field_argb_color,
			_field_real,
			_field_real_fraction,
			_field_real_point_2d,
			_field_real_point_3d,
			_field_real_vector_2d,
			_field_real_vector_3d,
			_field_real_quaternion,
			_field_real_euler_angles_2d,
			_field_real_euler_angles_3d,
			_field_real_plane_2d,
			_field_real_plane_3d,
			_field_real_rgb_color,
			_field_real_argb_color,
			_field_real_hsv_color,
			_field_real_ahsv_color,
			_field_short_bounds,
			_field_angle_bounds,
			_field_real_bounds,
			_field_real_fraction_bounds,
			_field_tag_reference,
			_field_block,
			_field_short_block_index,
			_field_long_block_index,
			_field_data,
			_field_array_start,
			_field_array_end,
			_field_pad,
			_field_skip,
			_field_explanation,
			_field_custom,
			_field_terminator,

			k_number_of_tag_field_types,
		};

		// Note: AFAICT, the engine code doesn't actually do the postprocess setup this way.
		// They have what is essentially a boolean parameter that could be considered as 'bool for_editor'
		enum tag_postprocess_mode : byte_enum {
			// In this mode, the tag is being postprocessed for runtime values (automatically fill fields, etc)
			_tag_postprocess_mode_for_runtime = FALSE,
			// In this mode we're opening for tag editing (eg, tool process or guerilla) and should skip the postprocessing
			// code which prepares the tag for use in-game (Sapien and when building a cache)
			_tag_postprocess_mode_for_editor = TRUE,
		};
	};

	namespace Flags
	{
		enum {
			// Never streamed, unless the tag is loaded with _tag_load_for_editor_bit
			// Used by the sound tag for delay loading the actual sample data. So, eg, when tool goes to build a cache
			// it has to use tag_data_load on the sound samples. Maybe a better name is just 'lazy_loaded'
			_tag_data_never_streamed_bit = 0,
			_tag_data_is_text_data_bit,
			// ie, 'debug data'
			_tag_data_not_streamed_to_cache_bit,
			k_number_of_tag_data_definition_flags,

			// checked in the tag reference solving code.
			// last condition checked after an assortment of conditionals
			// and if this is FALSE, it won't resolve
			// NOTE: I think this a deprecated flag for loading the 'default' definition of a group.
			// This flag would cause a call of tag_load(group_tag, NULL, 0) to occur. However,
			// tag_load asserts name != NULL.
			// Flag isn't checked in H2EK's code (so more than likely deprecated)
			_tag_reference_unknown0_bit = 0,
			_tag_reference_non_resolving_bit,
			k_number_of_tag_group_tag_reference_flags,

			// set when block/data/reference fields are found during initialization
			_tag_block_has_children_bit = 0,
			k_number_of_tag_block_definition_flags,

			// tag_instance of the tag group will have their file_checksum CRC'd into the resulting cache tag header's crc field 
			_tag_group_include_in_tags_checksum_bit = 0,
			// only seen preferences_network_game use this
			_tag_group_unknown1_bit,
			// haven't seen this used at all
			_tag_group_unknown2_bit,
			// majority of tags have this set
			_tag_group_reloadable_bit,

			// YELO ONLY! tag_group is not meant for use in runtime builds
			_tag_group_debug_only_yelo_bit,
			// YELO ONLY! tag_group is non-standard (ie, official). This is applied to those defined in CheApe.map
			// Set at startup.
			_tag_group_non_standard_yelo_bit,
			// YELO ONLY! tag_group is referenced in other groups via a tag_reference or as a parent group
			// doesn't consider tag_references which can reference ANYTHING (eg, tag_collection's field).
			// Set at startup.
			_tag_group_referenced_yelo_bit,

			// When this is set, implies _tag_postprocess_mode_for_editor, else _for_runtime
			_tag_load_for_editor_bit = 0,
			// Load the tag from the file system, not from a cache/index
			_tag_load_from_file_system_bit,
			// If set: child references of the tag being loaded are not loaded themselves
			// Else, child references are loaded from disk
			_tag_load_non_resolving_references_bit,
		};
	};


	struct tag_field
	{
		Enums::field_type type; PAD16;
		cstring name;
		void* definition;

#if PLATFORM_IS_EDITOR
		// cast the [definition] pointer to a T*
		template<typename T>
		T* Definition() const { return CAST_PTR(T*, definition); }
		// cast the data of [definition] to T
		template<typename T>
		T DefinitionCast() const { return CAST_PTR(T, definition); }

		size_t GetSize(_Out_opt_ size_t* runtime_size) const;
		bool IsReadOnly() const;
		bool IsAdvanced() const;
		bool IsBlockName() const;
		bool IsInvisible() const;
#endif
	}; BOOST_STATIC_ASSERT( sizeof(tag_field) == 0xC );

	// Called as each element is read from the tag stream
	// NOTE: tag_index is non-standard, and will only be valid when invoked by OS code.
	// We can add additional parameters so long as PLATFORM_API is __cdecl (where the caller cleans up the stack).
	// NOTE: tag_index is not guaranteed to be not NONE! Eg: tag_block_add_element
	typedef bool (PLATFORM_API* proc_tag_block_postprocess_element)(void* element, Enums::tag_postprocess_mode mode,
		datum_index tag_index);
	// if [formatted_buffer] returns empty, the default block formatting is done
	typedef cstring (PLATFORM_API* proc_tag_block_format)(datum_index tag_index, tag_block* block, int32 element_index, char formatted_buffer[Enums::k_tag_block_format_buffer_size]);
	// Procedure called during tag_block_delete_element, but before all the child data is freed
	typedef void (PLATFORM_API* proc_tag_block_delete_element)(tag_block* block, int32 element_index);
	struct tag_block_definition
	{
		cstring name;
		long_flags flags;
		int32 maximum_element_count;
		size_t element_size;
		void* unused0;
		tag_field* fields;
		void* unused1;
		proc_tag_block_postprocess_element postprocess_proc;
		proc_tag_block_format format_proc;
		proc_tag_block_delete_element delete_proc;
		byte_swap_code_t* byte_swap_codes;

#if PLATFORM_IS_EDITOR
		// Searches the definition for a field of type [field_type] with a name which starts 
		// with [name] characters. Optionally starts at a specific field index.
		// Returns NONE if this fails.
		int32 FindFieldIndex(_enum field_type, cstring name, int32 start_index = NONE) const;
		tag_field* FindField(_enum field_type, cstring name, int32 start_index = NONE);
		tag_block_definition* FindBlockField(cstring name, int32 start_index = NONE);

		size_t GetAlignmentBit() const;
		TagGroups::s_tag_field_set_runtime_data* GetRuntimeInfo() const;
		void SetRuntimeInfo(TagGroups::s_tag_field_set_runtime_data* info);

		// TAction: void operator()([const] tag_block_definition* block, [const] tag_field& field)
		template<class TAction, bool k_assert_field_type>
		void FieldsDoAction(TAction& action = TAction())
		{
			for(auto& field : *this)
			{
				if(k_assert_field_type)
				{
					YELO_ASSERT( field.type>=0 && field.type<Enums::k_number_of_tag_field_types );
				}

				action(this, field);
			}
		}

		// Mainly a visitor for startup/shutdown processes, performs an action (via a functor) on a root block definition
		// then repeats the action for all child blocks (etc)
		// Uses CRT assert()'s since it is assumed this is used after the group definitions have been verified
		// TAction: void operator()([const] tag_block_definition* block_definition)
		template<class TAction>
		void DoRecursiveAction(TAction& action = TAction())
		{
			action(this); // perform the action on the root first

			size_t field_index = 0;

			do {
				const tag_field& field = fields[field_index];
				assert( field.type>=0 && field.type<Enums::k_number_of_tag_field_types );

				if(field.type == Enums::_field_block)
				{
					assert(field.definition != this);

					action(field.Definition<tag_block_definition>());
				}

			} while(fields[field_index++].type != Enums::_field_terminator);
		}

		struct s_field_iterator
		{
		private:
			tag_field* m_fields;

			bool IsEndHack() const			{ return m_fields == nullptr; }

		public:
			s_field_iterator() : m_fields(nullptr) {} // for EndHack
			s_field_iterator(const tag_block_definition* definition) : m_fields(definition->fields) {}

			bool operator!=(const s_field_iterator& other) const;

			s_field_iterator& operator++()
			{
				++m_fields;
				return *this;
			}
			tag_field& operator*() const
			{
				return *m_fields;
			}
		};

		s_field_iterator begin() /*const*/
		{
			auto iter = s_field_iterator(this);
			return iter;
		}
		static const s_field_iterator end() /*const*/
		{
			return s_field_iterator();
		}
#endif
	}; BOOST_STATIC_ASSERT( sizeof(tag_block_definition) == 0x2C );

	typedef void (PLATFORM_API* proc_tag_data_byte_swap)(void* block_element, void* address, int32 size);
	struct tag_data_definition
	{
		cstring name;
		long_flags flags;
		int32 maximum_size;
		proc_tag_data_byte_swap byte_swap_proc;

		bool IsConsideredDebugOnly() const
		{
			return
				// never streamed is not really debug, but it requires explicit tag_data_load before it is loaded into memory
				TEST_FLAG(flags, Flags::_tag_data_never_streamed_bit) ||
				TEST_FLAG(flags, Flags::_tag_data_not_streamed_to_cache_bit);
		}
	}; BOOST_STATIC_ASSERT( sizeof(tag_data_definition) == 0x10 );

	struct tag_reference_definition
	{
		long_flags flags;
		tag group_tag;
		tag* group_tags;

#if PLATFORM_IS_EDITOR
		struct s_group_tag_iterator
		{
		private:
			tag* m_group_tags;

			bool IsEndHack() const			{ return m_group_tags == nullptr; }

		public:
			s_group_tag_iterator() : m_group_tags(nullptr) {} // for EndHack
			s_group_tag_iterator(const tag_reference_definition* definition) : m_group_tags(definition->group_tags) {}

			bool operator!=(const s_group_tag_iterator& other) const;

			s_group_tag_iterator& operator++()
			{
				++m_group_tags;
				return *this;
			}
			tag& operator*() const
			{
				return *m_group_tags;
			}
		};

		s_group_tag_iterator begin() /*const*/
		{
			auto iter = s_group_tag_iterator(this);
			return iter;
		}
		static const s_group_tag_iterator end() /*const*/
		{
			return s_group_tag_iterator();
		}
#endif
	}; BOOST_STATIC_ASSERT( sizeof(tag_reference_definition) == 0xC );

	// Postprocess a tag definition (eg, automate the creation of fields, etc)
	// Called once the tag has been fully loaded (header_block_definition's postprocess is called before this)
	typedef bool (PLATFORM_API* proc_tag_group_postprocess)(datum_index tag_index, Enums::tag_postprocess_mode mode);
	struct tag_group
	{
		cstring name;
		long_flags flags;
		tag group_tag;
		tag parent_group_tag;
		int16 version; PAD16;
		proc_tag_group_postprocess postprocess_proc;
		tag_block_definition* header_block_definition;
		tag child_group_tags[Enums::k_maximum_children_per_tag];
		int16 child_count; PAD16;

#if PLATFORM_IS_EDITOR
		TagGroups::s_tag_field_set_runtime_data* GetHeaderRuntimeInfo() const { return header_block_definition->GetRuntimeInfo(); }

		bool IsDebugOnly() const { return TEST_FLAG(flags, Flags::_tag_group_debug_only_yelo_bit); }

		// tag_group* [] (ie, tag_group**) qsort procs
		static int __cdecl CompareByNameProc(void*, const tag_group*const* lhs, const tag_group*const* rhs);
		static int __cdecl CompareByGroupTagProc(void*, const tag_group*const* lhs, const tag_group*const* rhs);

		static int __cdecl SearchByNameProc(void*, cstring key, const tag_group*const* group);
#endif
	}; BOOST_STATIC_ASSERT( sizeof(tag_group) == 0x60 );


	struct s_tag_instance : Memory::s_datum_base_aligned
	{
		char filename[Enums::k_max_tag_name_length+1];			// 0x4
		tag group_tag;				// 0x104
		tag parent_group_tags[2];	// 0x108
		bool is_verified;			// 0x110 was loaded with Flags::_tag_load_for_editor_bit
		bool is_read_only;			// 0x111
		bool is_orphan;				// 0x112
		bool is_reload;				// 0x113 true if this instance is the one used for another tag during tag_reload
		datum_index reload_index;	// 0x114 index of the instance used to reload -this- tag's definition
		uint32 file_checksum;		// 0x118
		tag_block root_block;		// 0x11C
	}; BOOST_STATIC_ASSERT( sizeof(s_tag_instance) == 0x128 );


	namespace TagGroups
	{
		struct s_tag_field_definition
		{
			size_t size;						/// <summary>	The size of a single instance of this field. </summary>
			cstring name;						/// <summary>	The user-friendly name of this field. </summary>
			byte_swap_code_t* byte_swap_codes;	/// <summary>	The needed for byte swapping an instance of this field. </summary>

			/// <summary>	The C name of this field. Null if it can't be defined in code (eg, _field_custom) </summary>
			cstring code_name;
		};
	};
};