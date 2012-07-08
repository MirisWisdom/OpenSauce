/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <Common/LinkedList.hpp>

#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"
#include "Rasterizer/PostProcessing/c_shader_instance.hpp"
#include "Rasterizer/PostProcessing/c_quad_manager.hpp"
#include "Rasterizer/PostProcessing/Interpolation/c_interp_linear.hpp"
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		class c_effect_postprocess
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				TagGroups::s_effect_postprocess_definition* effect_definition;

				struct
				{
					int32 count;
					c_shader_instance* list;
				}m_shaders;
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
			virtual void			SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition);
			bool					IsValid();
		protected:
			template<class T>
			T*						GetShaderInstance(int index)
			{
				ASSERT(index >= m_members.m_shaders.count, "shader instance index outside the bounds of the array");

				return CAST_PTR(T*, GetNodeByIndex(&m_members.m_shaders.list, index));
			}

			/////////////////////////////////////////////////
			// initializers
		public:
			virtual void Ctor()
			{
				m_members.effect_definition = NULL;
				m_members.m_shaders.list = NULL;
				m_members.m_shaders.count = 0;
			}

			virtual void Dtor()
			{
				m_members.effect_definition = NULL;
				m_members.m_shaders.list = NULL;
				m_members.m_shaders.count = 0;
			}

			/////////////////////////////////////////////////
			// effect setup
		public:
			virtual void			SetupEffect() {}
			virtual void			SetupEffectPostCreation();
			void					AddShaderInstance(c_shader_instance* instance);
			void					Validate();
		private:
			virtual bool			ValidateImpl();

			/////////////////////////////////////////////////
			// effect application
		public:
			void					Update(real delta_time);
			HRESULT					Render(IDirect3DDevice9* render_device, c_quad_instance* render_quad);
			HRESULT					Render(IDirect3DDevice9* render_device, c_quad_instance* render_quad, real fade_amount);
		protected:
			virtual bool			IsActive();
		};
	}; };
};
#endif