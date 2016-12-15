/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	// The epsilon value I've seen Halo1 use
	// TODO: See if we need to throw this into the base module
	extern const real K_REAL_EPSILON;

	extern const int32 K_TICKS_PER_SECOND;
	extern const real K_SECONDS_PER_TICK;
};