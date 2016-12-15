/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_effect_instance.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		class c_effect_instance_generic : public c_effect_instance
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				TagGroups::s_effect_postprocess_generic_effect_instance* definition;
			}m_members_generic;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetEffectInstanceDefinition(TagGroups::s_effect_postprocess_generic_effect_instance* definition);
			const char* GetName();
			Enums::postprocess_render_stage GetRenderStage();

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_effect_instance::Ctor();

				m_members_generic.definition = NULL;
			}

			void Dtor()
			{
				c_effect_instance::Dtor();

				m_members_generic.definition = NULL;
			}

			/////////////////////////////////////////////////
			// effect instance setup
		public:
			void SetupEffectInstance();
			bool GetInitiallyActive();

			/////////////////////////////////////////////////
			// shader instance application
			bool IsActive();
			TagGroups::s_effect_postprocess_effect_activation_control*
				FindActivationControl(Enums::effect_activation_state state);
			bool EvaluateActivationControls();
			bool GetActivationValue(TagGroups::s_effect_postprocess_effect_activation_control& control);
		};
	};};}
};
#endif