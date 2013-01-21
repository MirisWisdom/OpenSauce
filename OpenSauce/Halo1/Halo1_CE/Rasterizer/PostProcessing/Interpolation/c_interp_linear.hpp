/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Interpolation/c_interp_base.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		template<int C>
		class c_interp_linear : public c_interp_base<C>
		{
			struct{
				real current_value;
				real change_time;
			} m_members_linear;

		public:
			void Ctor()
			{
				m_members_linear.current_value = 0.0f;
				m_members_linear.change_time = 0.0f;
			}
			void Dtor()
			{
				m_members_linear.current_value = 0.0f;
				m_members_linear.change_time = 0.0f;
			}

			void Update(real delta_time)
			{
				// increase the interpolation value at the required rate
				if(m_members_linear.change_time == 0.0f)
					m_members_linear.current_value = 1.0f;
				else
					m_members_linear.current_value += CAST(real, (1.0f / m_members_linear.change_time) * delta_time);

				// clamp to a maximum of 1.0f
				if(m_members_linear.current_value > 1.0f)
					m_members_linear.current_value = 1.0f;
				
				SetValues(m_members_linear.current_value);
			}

			void Begin(const real interp_time)
			{
				// set the change time and reset the interpolation to zero
				m_members_linear.change_time = interp_time;
				m_members_linear.current_value = 0.0f;

				// if change time is 0 immediately finish the transition
				if(m_members_linear.change_time == 0.0f)
					m_members_linear.current_value = 1.0f;

				SetValues(m_members_linear.current_value);
			}

			void Reset()
			{
				m_members_linear.change_time = 0.0f;
				m_members_linear.current_value = 0.0f;

				SetValues(m_members_linear.current_value);
			}

		private:
			void SetValues(real interp)
			{
				// in the linear interpolator, all channels have the same value
				for(int i = 0; i < C; i++)
					m_members.values[i] = m_members_linear.current_value;
			}
		};
	};};
};
#endif