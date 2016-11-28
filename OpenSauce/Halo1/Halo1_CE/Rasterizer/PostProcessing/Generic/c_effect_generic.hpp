/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>
#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"
#include "Rasterizer/PostProcessing/Generic/c_parameter_instance.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		class c_effect_generic : public c_effect_postprocess
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				TagGroups::s_effect_postprocess_generic* definition;
			}m_members_generic;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition);

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_effect_postprocess::Ctor();
			}
			void Dtor()
			{
				c_effect_postprocess::Dtor();
			}

			/////////////////////////////////////////////////
			// effect setup
		public:
			void SetupEffect();
		private:
			void SetupExposedParameters();
		protected:
			c_parameter_instance* GetExposedParameter(const char* exposed_name);
		};
	};};};
};
#endif