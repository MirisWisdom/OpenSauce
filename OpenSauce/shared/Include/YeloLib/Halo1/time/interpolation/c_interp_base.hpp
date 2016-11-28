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
		class c_interp_base
			: public i_interpolator
		{
		protected:
			struct
			{
				std::array<real, ValueCount> values;
			}m_members;

		private:
			void ClearMembers()
			{
				m_members.values.fill(0.0f);
			}

		public:
			real* GetValues() override
			{
				return m_members.values.data();
			}

			virtual void Ctor() override
			{
				ClearMembers();
			}
			virtual void Dtor() override
			{
				ClearMembers();
			}

			void Update(real delta_time) override = 0;
		};
	};};
};