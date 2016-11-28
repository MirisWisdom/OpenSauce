/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingCacheComponent.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingUpdatable.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingRenderable.hpp"

#include "Rasterizer/PostProcessing/c_effect_render_set.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_instance_generic.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_shader_internal.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_effect_internal.hpp"
#include "Rasterizer/PostProcessing/Generic/c_effect_instance_generic.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Generic { namespace Internal
	{
		class c_system_internal
			: public IPostProcessingCacheComponent
			, public IPostProcessingUpdatable
			, public IPostProcessingRenderable
		{
			struct s_effect_set
			{
				c_effect_internal* effect;

				uint32 shader_instance_count;
				c_shader_instance_generic* shader_instances;
			};

			/////////////////////////////////////////////////
			// static members
		private:
			static c_system_internal g_internal_system;

			/////////////////////////////////////////////////
			// static member accessors
		public:
			static c_system_internal& Instance();

			/////////////////////////////////////////////////
			// members
		private:
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

			struct
			{
				TagGroups::s_effect_postprocess_collection* cache_shader_collection;

				c_effect_render_set m_render_sets[5];

				struct
				{
					uint16 count;
					PAD16;
					c_shader_internal* shader_list;
				}m_shaders;

				struct
				{
					uint16 count;
					PAD16;
					s_effect_set* effect_list;
				}m_effects;

				struct
				{
					uint16 count;
					PAD16;
					c_effect_instance_generic* effect_instance_list;
				}m_effect_instances;
			}m_members_internal;

			void ClearMembers();

			/////////////////////////////////////////////////
			// member accessors
		public:
			bool& Enabled();
			bool IsReady();
			bool IsUnloaded();

			/////////////////////////////////////////////////
			// IPostProcessingComponent
		public:
			void Initialize();
			void Dispose();

			void OnLostDevice_Base();
			void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters);
			
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
			// IPostProcessingCacheComponent
		public:
			void Initialize_Cache();
			void Dispose_Cache();

			void InitializeResources_Cache();
			void ReleaseResources_Cache();

			/////////////////////////////////////////////////
			// system setup
		private:
			void GetShaderCollection();
			void ClearShaderCollection();

			void GetInternalShaders();
			void ClearInternalShaders();
			void GetInternalEffects();
			void ClearInternalEffects();

			HRESULT LoadShaders();
			void UnloadShaders();
			HRESULT LoadEffects();
			void UnloadEffects();

			void SetupEffect(s_effect_set& effect_set, TagGroups::s_effect_postprocess_collection_effect* definition);
			void DestroyEffect(s_effect_set& effect_set);
			void SetupEffectInstance(c_effect_instance_generic* instance, TagGroups::s_effect_postprocess_generic_effect_instance* definition);
			void DestroyEffectInstance(c_effect_instance_generic* instance);

			c_shader_postprocess* GetShaderByIndex(datum_index index);

			void SetupRenderSets();
			void ClearRenderSets();
			void SetRenderSet(c_effect_render_set& set, Enums::postprocess_render_stage render_stage);

			void ValidateSystem();

			void UpdateStatus();

			/////////////////////////////////////////////////
			// scripting
		public:
			int16 GetEffectInstanceIndexByName(const char* name);
			void SetEffectInstanceActive(int16 index, bool active);
			void SetEffectInstanceFade(int16 index, real start, real end, real time);
			real GetEffectInstanceCurrentFade(int16 index);
			int16 GetEffectInstanceFadeDirection(int16 index);

			int16 GetEffectIndexByName(const char* name);
			bool GetEffectIsValid(int16 index);
			int16 GetEffectShaderVariableIndexByName(int16 index, const char* name);
			void SetEffectShaderVariableBoolean(int16 index, int16 variable_index, bool value, real time);
			void SetEffectShaderVariableInteger(int16 index, int16 variable_index, int32 value, real time);
			void SetEffectShaderVariableReal(int16 index, int16 variable_index, real value0, real value1, real value2, real value3, real time);

			void SetEffectShaderInstanceActive(int16 index, int16 instance_index, bool active);
		};
	};};};};
};
#endif