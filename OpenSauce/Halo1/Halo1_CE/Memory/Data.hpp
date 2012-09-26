/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <Memory/DataShared.hpp>
#include <Memory/Halo1/DataShared.hpp>

namespace Yelo
{
	namespace Memory
	{
		template<typename DatumT> struct DynamicArray
		{
		private:
			size_t element_size;
			int32 count;
			DatumT* elements;

		public:
			int32 GetCount() const { return count; }

			API_INLINE DatumT* operator [](int32 index)
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}

			API_INLINE const DatumT* operator [](int32 index) const
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}
		};

		// I've only seen gbx code use this structure format...
		// Therefore, I declare ye' GbxArray!
		// Guess they didn't want to change dynamic_array to support a 'capacity'
		template<typename DatumT> struct GbxArray
		{
		public:
			DatumT* elements;
			int32 count;
			int32 capacity;

		public:
			int32 GetCount() const { return count; }

			int32 GetCapacity() const { return capacity; }

			API_INLINE DatumT* operator [](int32 index)
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}

			API_INLINE const DatumT* operator [](int32 index) const
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}
		};
	};
};