/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/memory/linked_list.hpp>

#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		class c_effect_instance : public LinkedListNode<c_effect_instance>
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				struct
				{
					bool is_valid;
					bool is_active;
					PAD16;
				}m_flags;

				c_effect_postprocess* definition;
				TagGroups::s_effect_postprocess_quad_definition* quad_definition;

				c_quad_instance* render_quad;

				struct
				{
					real start;
					real end;
					real current;

					Time::Interpolation::c_interp_linear<1> interpolator;
				}m_fade;
			}m_members;

		private:
			void ClearMembers()
			{
				ClearNodeData();

				m_members.m_flags.is_valid = false;
				m_members.m_flags.is_active = true;
				m_members.definition = nullptr;
				m_members.quad_definition = nullptr;
				m_members.render_quad = nullptr;
				m_members.m_fade.start = 1;
				m_members.m_fade.end = 1;
				m_members.m_fade.current = 1;

				m_members.m_fade.interpolator.Begin(0);
			}

			/////////////////////////////////////////////////
			// member accessors
		public:
			virtual void SetEffect(c_effect_postprocess* definition);
			void SetQuadDefinition(TagGroups::s_effect_postprocess_quad_definition* definition);
			bool IsValid();
			real GetCurrentFade();
			int16 GetFadeDirection();
			void SetIsActive(bool active);

			/////////////////////////////////////////////////
			// initializers
		public:
			virtual void Ctor()
			{
				ClearMembers();
			}

			virtual void Dtor()
			{
				ClearMembers();
			}

			/////////////////////////////////////////////////
			// effect instance setup
		public:
			virtual void SetupEffectInstance() {}
			void Validate();

			HRESULT LoadEffectInstance();
			void UnloadEffectInstance();
		protected:
			virtual bool ValidateImpl();

			/////////////////////////////////////////////////
			// effect instance application
		protected:
			virtual bool IsActive();
		public:
			HRESULT Render(IDirect3DDevice9* render_device);

			virtual void UpdateEffectInstance(real delta_time);
			void SetEffectFade(real start, real end, real change_time);

		};
	};};
};
#endif