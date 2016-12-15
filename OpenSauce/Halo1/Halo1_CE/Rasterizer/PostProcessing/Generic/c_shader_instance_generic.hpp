/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/c_shader_instance.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"
#include "Rasterizer/PostProcessing/Generic/c_parameter_instance.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		class c_shader_instance_generic : public c_shader_instance
		{
			/////////////////////////////////////////////////
			// members
			struct
			{
				c_shader_generic* shader;

				uint32 parameter_count;
				c_parameter_instance* parameters;
			}m_members_generic;

			/////////////////////////////////////////////////
			// member accessors
		public:
			void SetShader(c_shader_postprocess* shader);

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				c_shader_instance::Ctor();

				m_members_generic.shader = NULL;
				m_members_generic.parameter_count = 0;
				m_members_generic.parameters = NULL;
			}
			void Dtor()
			{
				c_shader_instance::Dtor();

				DestroyParameterInstances();
				m_members_generic.shader = NULL;
			}

			/////////////////////////////////////////////////
			// shader instance setup
		public:
			void SetupShaderInstance();
			c_parameter_instance* GetParameterInstance(const char* name);
		private:
			void CreateParameterInstances();
			void DestroyParameterInstances();

			/////////////////////////////////////////////////
			// shader instance application
		public:
			void UpdateShaderInstance(real delta_time);
			void SetShaderInstanceVariables();
		};
	};};};
};
#endif