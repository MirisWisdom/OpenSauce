/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/rasterizer/dx9/rasterizer_dx9_render_targets.hpp>
#include "Rasterizer/GBuffer/Effects/c_gbuffer_rtclear_effect.hpp"

namespace Yelo
{
    namespace Flags
    {
        enum gbuffer_render_options_flags
        {
            _gbuffer_render_options_none_bit = 0,
            _gbuffer_render_options_velocity_bit = 1,
            _gbuffer_render_options_normals_bit = 2,
            _gbuffer_render_options_tangent_normals_binormals_bit = 3,

            _gbuffer_render_options
        };
    };

    namespace Rasterizer
    {
        namespace GBuffer
        {
            class c_gbuffer_rtclear_effect;

            enum
            {
                k_maximum_multi_render_target = 3,
                k_target_count = 4,
            };

            enum
            {
                k_gbuffer_depth_x = 0, //R32_F      X

                k_gbuffer_velocity_x = 0, //R16_F   X
                k_gbuffer_velocity_y = 1, //R16_F   Y

                k_gbuffer_normals_x = 0, //R8G8B8A8 X
                k_gbuffer_normals_y = 1, //R8G8B8A8 Y

                k_gbuffer_index_x = 2, //R8G8B8A8   Z
                k_gbuffer_index_y = 3, //R8G8B8A8   W
            };

            struct s_draw_primitive_parameters
            {
                D3DPRIMITIVETYPE m_type;
                int32 m_base_vertex_index;
                uint32 m_min_vertex_index;
                uint32 m_num_vertices;
                uint32 m_start_index;
                uint32 m_primitive_count;

                s_draw_primitive_parameters()
                    : m_type(),
                      m_base_vertex_index(0),
                      m_min_vertex_index(0),
                      m_num_vertices(0),
                      m_start_index(0),
                      m_primitive_count(0) {}
            };

            struct s_draw_call
            {
                s_draw_primitive_parameters m_primitive;
                uint32 m_option_flags;
                PAD16;

                s_draw_call()
                    : m_primitive(),
                      m_option_flags(0) {}
            };

            struct s_draw_call_object : s_draw_call
            {
                int16 m_team_index;
                int16 m_mesh_index;
                int32 m_level_of_detail;

                s_draw_call_object()
                    : s_draw_call(),
                      m_team_index(0),
                      m_mesh_index(0),
                      m_level_of_detail(0) {}
            };

            struct s_gbuffer_output_map
            {
                LPDIRECT3DSURFACE9 m_outputs[k_target_count][k_maximum_multi_render_target];
                uint32 m_output_count;
            };

            class c_gbuffer final
            {
                const bool m_use_os_shaders;

                bool m_wvp_stored;
                D3DXMATRIX m_stored_worldviewproj[2];
                int16 m_stored_wvp_index;

                LPD3DXEFFECT m_pixel_shader;
                LPD3DXEFFECT m_vertex_shader;

                D3DXVECTOR4 m_pixel_shader_input;

                struct
                {
                    struct
                    {
                        D3DXHANDLE n;
                        D3DXHANDLE n_v;
                    } ps_bsp_techniques;

                    struct
                    {
                        D3DXHANDLE n;
                        D3DXHANDLE n_v;
                    } vs_bsp_techniques;

                    struct
                    {
                        D3DXHANDLE n;
                        D3DXHANDLE tbn;
                        D3DXHANDLE n_v;
                        D3DXHANDLE tbn_v;
                        D3DXHANDLE none;
                    } ps_object_techniques;

                    struct
                    {
                        D3DXHANDLE n;
                        D3DXHANDLE tbn;
                        D3DXHANDLE n_v;
                        D3DXHANDLE tbn_v;
                        D3DXHANDLE none;
                    } vs_object_techniques;
                } m_techniques;

                Render::s_render_target m_rt_depth;
                Render::s_render_target m_rt_velocity;
                Render::s_render_target m_rt_normals_index;
                s_gbuffer_output_map m_output_map;

                struct
                {
                    LPDIRECT3DSURFACE9 m_texture;
                    LPDIRECT3DVERTEXSHADER9 m_vertex_shader;
                    DWORD m_z_write_enable;
                } m_stored_state;

            public:
                explicit c_gbuffer(const bool use_os_shaders);

                bool Create(IDirect3DDevice9& device, const uint32 width, const uint32 height);
                void Destroy();
                void Update();

                HRESULT DrawStructure(IDirect3DDevice9& device, const s_draw_call& parameters);
                HRESULT DrawObject(IDirect3DDevice9& device, const s_draw_call_object& parameters);
                void StoreWorldViewProj(const D3DMATRIX& matrix);

                bool SetDepth(ID3DXEffect& effect, bool& variable_used) const;
                bool SetVelocity(ID3DXEffect& effect, bool& variable_used) const;
                bool SetNormals(ID3DXEffect& effect, bool& variable_used) const;
                bool SetIndex(ID3DXEffect& effect, bool& variable_used) const;
                Render::s_render_target& GetDepthTarget();
                Render::s_render_target& GetVelocityTarget();
                Render::s_render_target& GetNormalsIndexTarget();
                s_gbuffer_output_map& OutputMap();

            private:
                bool CreateTargets(IDirect3DDevice9& device, const uint32 width, const uint32 height);
                void DestroyTargets();
                bool CreateShaders(IDirect3DDevice9& device);
                void PushState(IDirect3DDevice9& device);
                void PopState(IDirect3DDevice9& device);
                void SetVertexShaderConstants(IDirect3DDevice9& device) const;
                void PreDrawStructure(const s_draw_call& parameters);
                void PreDrawObject(const s_draw_call_object& parameters);
                HRESULT Draw(IDirect3DDevice9& device, const s_draw_primitive_parameters& parameters);

                bool SetEffectVariables(ID3DXEffect& effect,
                    bool& variable_used,
                    const cstring texture_semantic,
                    const Render::s_render_target& target,
                    const cstring x_handle_semantic = nullptr, const int32 x_index = 0,
                    const cstring y_handle_semantic = nullptr, const int32 y_index = 0,
                    const cstring z_handle_semantic = nullptr, const int32 z_index = 0,
                    const cstring w_handle_semantic = nullptr, const int32 w_index = 0) const;
                bool SetEffectInteger(ID3DXEffect& effect, const cstring semantic, const int32 value) const;
            };
        };
    };
};
#endif
