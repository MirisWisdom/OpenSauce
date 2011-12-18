/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Rasterizer/PostProcessing/Interfaces/IInterpolator.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		template<int C>
		class c_interp_base : public IInterpolator
		{
		protected:
			struct
			{
				real values[C];
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

			void Update(real delta_time) {}
		};
	};};
};