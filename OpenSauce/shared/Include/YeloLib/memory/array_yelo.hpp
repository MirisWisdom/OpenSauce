/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		// C++ wrapper for blamlib's s_dynamic_array
		template<typename DatumT>
		struct DynamicArray
		{
		private:
			size_t element_size;
			int32 count;
			DatumT* elements;

		public:
			int32 GetCount() const { return count; }

			inline const DatumT* operator [](int32 index) const
			{
				if (index < 0 || index >= count)
					return nullptr;

				return &CAST_PTR(DatumT*, elements)[index];
			}

			inline DatumT* operator [](int32 index)
			{
				return CAST_THIS_NONCONST_MEMBER_FUNC(DatumT*, [index]);
			}
		};

		// I've only seen gbx code use this structure format...
		// Therefore, I declare ye' GbxArray!
		// Guess they didn't want to change dynamic_array to support a 'capacity'
		template<typename DatumT>
		struct GbxArray
		{
		public:
			DatumT* elements;
			int32 count;
			int32 capacity;

		public:
			int32 GetCount() const { return count; }

			int32 GetCapacity() const { return capacity; }

			inline const DatumT* operator [](int32 index) const
			{
				if (index < 0 || index >= count)
					return nullptr;

				return &CAST_PTR(DatumT*, elements)[index];
			}

			inline DatumT* operator [](int32 index)
			{
				return CAST_THIS_NONCONST_MEMBER_FUNC(DatumT*, [index]);
			}
		};
	};
};