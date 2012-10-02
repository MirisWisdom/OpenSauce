/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Interpolation
	{
		template<size_t count>
		static void InterpolateValues(bool* lower, bool* upper, real* interp, bool* output)
		{
			ASSERT(lower != NULL, "Interpolation lower values pointer is null");
			ASSERT(upper != NULL, "Interpolation upper values pointer is null");
			ASSERT(interp != NULL, "Interpolation interp values pointer is null");
			ASSERT(output != NULL, "Interpolation output pointer is null");

			// interp the values to a temporary buffer
			bool values[count];
			for(size_t i = 0; i < count; i++)
				values[i] = (interp[i] > 0.5 ? lower[i] : upper[i]);

			// copy the temporary buffer to the output
			memcpy(output, &values, sizeof(bool) * count);
		}
		template<size_t count>
		static void InterpolateValues(int32* lower, int32* upper, real* interp, int32* output)
		{
			ASSERT(lower != NULL, "Interpolation lower values pointer is null");
			ASSERT(upper != NULL, "Interpolation upper values pointer is null");
			ASSERT(interp != NULL, "Interpolation interp values pointer is null");
			ASSERT(output != NULL, "Interpolation output pointer is null");

			// interp the values to a temporary buffer
			int32 values[count];
			for(size_t i = 0; i < count; i++)
			{
				real range = CAST(real, upper[i] - lower[i]);
				values[i] = CAST(int, (range * interp[i]) + lower[i]);
			}

			// copy the temporary buffer to the output
			memcpy(output, &values, sizeof(int) * count);
		}
		template<size_t count>
		static void InterpolateValues(real* lower, real* upper, real* interp, real* output)
		{
			ASSERT(lower != NULL, "Interpolation lower values pointer is null");
			ASSERT(upper != NULL, "Interpolation upper values pointer is null");
			ASSERT(interp != NULL, "Interpolation interp values pointer is null");
			ASSERT(output != NULL, "Interpolation output pointer is null");

			// interp the values to a temporary buffer
			real values[count];
			for(size_t i = 0; i < count; i++)
			{
				real range = upper[i] - lower[i];
				values[i] = (range * interp[i]) + lower[i];
			}

			// copy the temporary buffer to the output
			memcpy(output, &values, sizeof(real) * count);
		}
	};};};
};
#endif