/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cseries/random.hpp>

namespace Yelo
{
	namespace Random
	{
		static std::default_random_engine g_generator;

		void InitializeSeed(const int32 seed)
		{
			g_generator.seed(seed);
		}

		real GetReal(const real lower, const real upper)
		{
			std::uniform_real_distribution<real> distribution(lower, upper);

			return distribution(g_generator);
		}

		real GetReal(const real_bounds bounds)
		{
			return GetReal(bounds.lower, bounds.upper);
		}

		real GetReal(const real_fraction_bounds bounds)
		{
			return GetReal(bounds.lower, bounds.upper);
		}

		real GetReal(const angle_bounds bounds)
		{
			return GetReal(bounds.lower, bounds.upper);
		}

		uint32 GetUInt32(const uint32 lower, const uint32 upper)
		{
			std::uniform_int_distribution<uint32> distribution(lower, upper);

			return distribution(g_generator);
		}

		int32 GetInt32(const int32 lower, const int32 upper)
		{
			std::uniform_int_distribution<int32> distribution(lower, upper);

			return distribution(g_generator);
		}

		uint16 GetUInt16(const uint16 lower, const uint16 upper)
		{
			std::uniform_int_distribution<uint16> distribution(lower, upper);

			return distribution(g_generator);
		}

		int16 GetInt16(const int16 lower, const int16 upper)
		{
			std::uniform_int_distribution<int16> distribution(lower, upper);

			return distribution(g_generator);
		}

		int16 GetInt16(const short_bounds bounds)
		{
			return GetInt16(bounds.lower, bounds.upper);
		}
	};
};