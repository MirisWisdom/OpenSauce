/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <yelolib/tag_files/TagBlock.h>
#include <yelolib/tag_files/TagData.h>

namespace Yelo
{
	namespace blam
	{
		#if PLATFORM_IS_EDITOR
		template <typename T>
		T* tag_block_get_element(
			TagBlock<T>& block,
			int32 element)
		{
			return CAST_PTR(T*, tag_block_get_element(block.to_tag_block(), element));
		}

		template <typename T>
		const T* tag_block_get_element(
			const TagBlock<T>& block,
			int32 element)
		{
			return CAST_PTR(const T*, tag_block_get_element(block.to_tag_block(), element));
		}

		template <typename T>
		int32 tag_block_add_element(
			TagBlock<T>& block)
		{
			return tag_block_add_element(block.to_tag_block());
		}

		template <typename T>
		bool tag_block_resize(
			TagBlock<T>& block,
			int32 element_count)
		{
			return tag_block_resize(block.to_tag_block(), element_count);
		}

		template <typename T>
		void tag_block_delete_element(
			TagBlock<T>& block,
			int32 element)
		{
			tag_block_delete_element(block.to_tag_block(), element);
		}

		template <typename T>
		T* tag_block_add_and_get_element(
			TagBlock<T>& block)
		{
			return CAST_PTR(T*, tag_block_add_and_get_element(block.to_tag_block()));
		}
		#endif
	}

	namespace blam
	{
		#if PLATFORM_IS_EDITOR
		template <typename T>
		bool tag_data_resize(
			TagData<T>& data,
			size_t new_size = 0)
		{
			return tag_data_resize(data.to_tag_data(), new_size);
		}
		#endif
	}
}
