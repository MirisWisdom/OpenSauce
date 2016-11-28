/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/time/interpolation/i_interpolator.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum parameter_process_type : _enum
		{
			_parameter_process_type_none,
			_parameter_process_type_static,
			_parameter_process_type_animated,
			_parameter_process_type_runtime_value,
			_parameter_process_type_texture,
			_parameter_process_type
		};
	};

	namespace Rasterizer { namespace PostProcessing { namespace Generic
	{
		typedef void (*t_evaluate_runtime_value)(void*);

		class c_parameter_instance
		{
			/////////////////////////////////////////////////
			// members
			struct
			{
				struct {
					bool is_overriden;
					PAD24;
				}m_flags;

				Enums::parameter_process_type parameter_type;
				PAD16;

				TagGroups::s_shader_postprocess_parameter* parameter;

				Time::Interpolation::i_interpolator* interpolator;

				t_evaluate_runtime_value evaluate_runtime_value;

				union {
					void*				pointer;
					bool*				boolean;
					int32*				integer32;
					real*				real32;
					real_vector2d*		vector2d;
					real_vector3d*		vector3d;
					real_quaternion*	quaternion;
					real_argb_color*	color;
				} m_values[2];
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
			TagGroups::s_shader_postprocess_parameter* GetParameter();
			void SetParameter(TagGroups::s_shader_postprocess_parameter* parameter);

			/////////////////////////////////////////////////
			// initializers
		public:
			void Ctor()
			{
				m_members.parameter_type = Enums::_parameter_process_type_none;
				m_members.m_flags.is_overriden = false;
				m_members.parameter = NULL;
				m_members.interpolator = NULL;
				m_members.evaluate_runtime_value = NULL;

				m_members.m_values[0].pointer = NULL;
				m_members.m_values[1].pointer = NULL;
			}

			void Dtor()
			{
				m_members.parameter_type = Enums::_parameter_process_type_none;
				m_members.m_flags.is_overriden = false;
				m_members.parameter = NULL;

				DeleteInterpolator();

				m_members.evaluate_runtime_value = NULL;

				DeleteValueMemory();
			}

			/////////////////////////////////////////////////
			// parameter instance setup
		public:
			virtual void SetupParameterInstance();
			void SetupParameterOverride();
		private:
			void SetupStaticValue();
			void SetupAnimatedValue();
			void SetupRuntimeValue();
			void SetupTextureValue();

			void CreateInterpolatorFunction();
			void CreateInterpolatorLinear();
			void DeleteInterpolator();

			template<class T>
			void AllocateValueMemory();
			void DeleteValueMemory();

			/////////////////////////////////////////////////
			// parameter instance application
		public:
			virtual void Update(real delta_time);
			virtual void SetParameterVariable(LPD3DXEFFECT effect);

			/////////////////////////////////////////////////
			// parameter instance override application
		public:
			void SetOverrideInterp(bool value, real change_time);
			void SetOverrideInterp(int32 value, real change_time);
			void SetOverrideInterp(real value, real change_time);
			void SetOverrideInterp(const real_vector2d& value, real change_time);
			void SetOverrideInterp(const real_vector3d& value, real change_time);
			void SetOverrideInterp(const real_quaternion& value, real change_time);
			void SetOverrideInterp(const real_argb_color& value, real change_time);
		private:
			void SetStartValueToCurrent();
		};
	}; }; };
};
#endif