/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Time { namespace Interpolation
	{
		interface IInterpolator
			abstract
		{
			virtual void Ctor() {}
			virtual void Dtor() {}

			virtual real* GetValues() { return NULL; }
			virtual void Update(real delta_time) {}
		};
	};};
};