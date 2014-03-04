/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include <YeloLib/Halo1/time/interpolation/i_interpolator.hpp>

namespace Yelo
{
	namespace Time { namespace Interpolation
	{
		template<int ValueCount>
		class c_interp_base : public i_interpolator
		{
		protected:
			struct
			{
				real values[ValueCount];
			}m_members;

		public:
			real* GetValues() { return &m_members.values[0]; }

			virtual void Ctor()
			{
				memset(&m_members.values, 0, sizeof(m_members.values));
			}
			virtual void Dtor()
			{
				memset(&m_members.values, 0, sizeof(m_members.values));
			}

			void Update(real delta_time) = 0;
		};
	};};
};