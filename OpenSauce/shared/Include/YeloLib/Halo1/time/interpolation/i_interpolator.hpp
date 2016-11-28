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
		class i_interpolator
			abstract
		{
		public:
			virtual void Ctor() {}
			virtual void Dtor() {}

			virtual real* GetValues() { return nullptr; }
			virtual void Update(real delta_time) {}
		};
	};};
};