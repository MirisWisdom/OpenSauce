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
				std::array<real, C> values;
			}m_members;

		private:
			void ClearMembers()
			{
				m_members.values.fill(0.0f);
			}

		public:
			real* GetValues() { return m_members.values.data(); }

			virtual void Ctor()
			{
				ClearMembers();
			}
			virtual void Dtor()
			{
				ClearMembers();
			}

			void Update(real delta_time) {}
		};
	};};
};