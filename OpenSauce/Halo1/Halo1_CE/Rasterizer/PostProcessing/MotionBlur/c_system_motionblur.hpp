/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingComponent.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingUpdatable.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingRenderable.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace MotionBlur
	{
		class c_system_motionblur
			: public IPostProcessingComponent
			, public IPostProcessingUpdatable
			, public IPostProcessingRenderable
		{
			/////////////////////////////////////////////////
			// static members
		private:
			static c_system_motionblur g_motionblur_system;

			/////////////////////////////////////////////////
			// static member accessors
		public:
			static c_system_motionblur& Instance();

			/////////////////////////////////////////////////
			// members
		private:
			class c_system_settings
				: public Configuration::c_configuration_container
			{
			public:
				Configuration::c_configuration_value<bool> m_enabled;
				Configuration::c_configuration_value<real> m_blur_amount;

				c_system_settings()
					: Configuration::c_configuration_container("Rasterizer.PostProcessing.MotionBlur")
					, m_enabled("Enabled", true)
					, m_blur_amount("BlurAmount", 1.0f)
				{ }
				
			protected:
				const std::vector<i_configuration_value* const> GetMembers()
				{
					std::vector<i_configuration_value* const> values =
					{
						&m_enabled,
						&m_blur_amount
					};

					return values;
				}
			};
			std::unique_ptr<c_system_settings> m_settings;

			struct
			{
				struct
				{
					bool is_ready;
					bool is_unloaded;
					bool is_enabled;
					PAD8;
				}m_flags;

				Enums::pp_component_status status;
				PAD16;
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
			bool& Enabled();
			bool IsReady();
			bool IsUnloaded();
			real& BlurAmount();

			/////////////////////////////////////////////////
			// IPostProcessingComponent
		public:
			void Initialize();
			void Dispose();

			void InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters);
			void OnLostDevice_Base();
			void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters);
			void ReleaseResources_Base();

			void Unload();
			void Load();

			/////////////////////////////////////////////////
			// IPostProcessingRenderable
		public:
			bool Render(Enums::postprocess_render_stage render_stage);

			/////////////////////////////////////////////////
			// IPostProcessingUpdatable
		public:
			void Update(real delta_time);

			/////////////////////////////////////////////////
			// system setup
		private:
			HRESULT CreateShader();
			void DestroyShader();

			HRESULT CreateEffect();
			void DestroyEffect();

			void SetDeviceLost();
			HRESULT SetDeviceReset();

			void Validate();

			void UpdateStatus();

			/////////////////////////////////////////////////
			// system application
		private:
			bool Render(IDirect3DDevice9* render_device);
		};
	}; }; };
};
#endif