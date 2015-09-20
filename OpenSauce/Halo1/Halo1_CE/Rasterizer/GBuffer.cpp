/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer.hpp"

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/game/player_structures.hpp>
#include <blamlib/Halo1/game/game_time.hpp>
#include <blamlib/Halo1/game/game_time_structures.hpp>
#include <blamlib/Halo1/game/game_allegiance.hpp>
#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Game/GameEngine.hpp"
#include "Game/Players.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Rasterizer/RenderTargetChain.hpp"
#include "Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_rtclear_effect.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_debug_effect.hpp"
#include "Rasterizer/GBuffer/c_gbuffer_settings.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_effect_factory.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_GBUFFER
#include "Memory/_EngineLayout.inl"

            enum render_progress
            {
                _render_progress_sky,
                _render_progress_objects,
                _render_progress_objects_transparent,
                _render_progress_structure,
                _render_progress_none,

                _render_progress
            };

            std::unique_ptr<c_gbuffer> g_gbuffer;
            std::unique_ptr<c_gbuffer_rtclear_effect> g_gbuffer_clear;
            std::unique_ptr<c_gbuffer_debug_effect> g_gbuffer_debug;

            bool g_enabled;
            bool g_rendered;
            static render_progress g_render_progress;
            int16 g_debug_index;
            datum_index g_object_index;
            int32 g_object_level_of_detail;
            bool g_output_velocity;
            bool g_output_tbn;

            API_FUNC_NAKED void Hook_RenderObjectList_GetObjectIndex()
            {
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RENDER_OBJECT_LIST_HOOK_RETN);

                __asm {
                    mov g_object_index, eax
                    and eax, 0FFFFh
                    lea edi, [eax+eax*2]
                    jmp RETN_ADDRESS
                }
            }

            API_FUNC_NAKED void Hook_RenderObjectList_ClearObjectIndex()
            {
                __asm {
                    mov g_object_index, NONE//datum_index::null
                    retn
                }
            }

            API_FUNC_NAKED void Hook_FirstPersonWeaponDraw_GetObjectIndex()
            {
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(FIRST_PERSON_WEAPON_DRAW_HOOK_RETN);

                __asm {
                    mov edx, [edx+34h]
                    mov eax, esi
                    mov g_object_index, eax
                    jmp RETN_ADDRESS
                }
            }

            API_FUNC_NAKED void Hook_RenderObject_GetCurrentLOD()
            {
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RENDER_OBJECT_OBJECT_LOD_HOOK_RETN);

                _asm{
                    mov ecx, [ebp+28h]
                    mov eax, [ebp+1Ch]

                    push eax
                    mov eax, [ebp-8]
                    mov g_object_level_of_detail, eax
                    pop eax

                    jmp RETN_ADDRESS
                };
            }

            // hooks for controlling when to disable velocity
            API_FUNC_NAKED void Hook_CommandCameraSet()
            {
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(COMMAND_CAMERA_SET_HOOK_RETN);

                _asm{
                    lea edx, [esi+28h]
                    mov eax, [edx]

                    push eax
                    mov eax, [ebp+8]
                    cmp eax, 0x00000000
                    jg skip_disable_velocity
                    mov g_output_velocity, 0
                    skip_disable_velocity:
                    pop eax
                    jmp RETN_ADDRESS
                };
            }

            API_FUNC_NAKED void Hook_CommandSwitchBSP()
            {
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(COMMAND_SWITCH_BSP_HOOK_RETN);

                _asm{
                    cmp si, dx
                    mov [esp+7], bl

                    mov g_output_velocity, 0

                    jmp RETN_ADDRESS
                };
            }

            API_FUNC_NAKED void Hook_CommandGameSave()
            {
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(COMMAND_GAME_SAVE_HOOK_RETN);

                _asm{
                    mov g_output_velocity, 0

                    mov ecx, [esp+8]
                    xor eax, eax
                    jmp RETN_ADDRESS
                };
            }

            // the hooked function takes arguments, for which FunctionInterface is unsuited
            API_FUNC_NAKED void Hook_RenderObjectsTransparent()
            {
                static const uintptr_t CALL_ADDRESS = GET_FUNC_PTR(RENDER_OBJECTS_TRANSPARENT);
                static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN);

                using namespace Enums;

                _asm{
                    mov g_render_progress, _render_progress_objects_transparent
                    call CALL_ADDRESS
                    mov g_render_progress, _render_progress_none
                    jmp RETN_ADDRESS
                }
            }

            HRESULT DrawObject(IDirect3DDevice9* device, D3DPRIMITIVETYPE type, INT base_vertex_index, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT primitive_count)
            {
                auto result = device->DrawIndexedPrimitive(type, base_vertex_index, min_vertex_index, num_vertices, start_index, primitive_count);
                if (g_rendered && Available() && !Render::IsRenderingReflection() &&
                    (g_render_progress == _render_progress_sky || g_render_progress == _render_progress_objects))
                {
                    byte mesh_index = 0;
                    byte team_index = 0;
                    if (g_render_progress == _render_progress_sky)
                    {
                        mesh_index = 1;
                    }
                    else if (!g_object_index.IsNull())
                    {
                        auto current_object = blam::object_get(g_object_index);

                        // add one to account for '_game_team_none'
                        team_index = current_object->owner_team + 1;
                        mesh_index = current_object->type + 3;

                        if (GameEngine::Current() != nullptr)
                        {
                            team_index += Enums::k_number_of_game_teams;
                        }

                        if (current_object->VerifyType(Enums::_object_type_mask_unit))
                        {
                            auto player = Players::LocalPlayer();
                            auto player_object = blam::object_get(player->slave_unit_index);

                            if (blam::game_team_is_enemy(player_object->owner_team, current_object->owner_team))
                            {
                                team_index |= 1 << 5;
                            }

                            if (current_object->damage.health <= 0.0f)
                            {
                                team_index |= 1 << 6;
                            }
                        }
                    }

                    s_draw_call_object draw_call;
                    draw_call.m_primitive.m_type = type;
                    draw_call.m_primitive.m_base_vertex_index = base_vertex_index;
                    draw_call.m_primitive.m_min_vertex_index = min_vertex_index;
                    draw_call.m_primitive.m_num_vertices = num_vertices;
                    draw_call.m_primitive.m_start_index = start_index;
                    draw_call.m_primitive.m_primitive_count = primitive_count;
                    if (g_render_progress != _render_progress_sky)
                    {
                        draw_call.m_option_flags |= g_output_tbn ?
                                                        SET_FLAG(draw_call.m_option_flags, Flags::gbuffer_render_options_flags::_gbuffer_render_options_tangent_normals_binormals_bit, true) :
                                                        SET_FLAG(draw_call.m_option_flags, Flags::gbuffer_render_options_flags::_gbuffer_render_options_normals_bit, true);
                    }
                    else
                    {
                        SET_FLAG(draw_call.m_option_flags, Flags::gbuffer_render_options_flags::_gbuffer_render_options_none_bit, true);
                    }

                    draw_call.m_level_of_detail = g_object_level_of_detail;
                    draw_call.m_team_index = team_index;
                    draw_call.m_mesh_index = mesh_index;

                    result &= g_gbuffer->DrawObject(*device, draw_call);
                }
                return result;
            }

            HRESULT DrawStructure(IDirect3DDevice9* device, D3DPRIMITIVETYPE type, INT base_vertex_index, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT primitive_count)
            {
                auto result = device->DrawIndexedPrimitive(type, base_vertex_index, min_vertex_index, num_vertices, start_index, primitive_count);
                if (g_rendered && Available() && !Render::IsRenderingReflection() &&
                    g_render_progress == _render_progress_structure)
                {
                    s_draw_call draw_call;
                    draw_call.m_primitive.m_type = type;
                    draw_call.m_primitive.m_base_vertex_index = base_vertex_index;
                    draw_call.m_primitive.m_min_vertex_index = min_vertex_index;
                    draw_call.m_primitive.m_num_vertices = num_vertices;
                    draw_call.m_primitive.m_start_index = start_index;
                    draw_call.m_primitive.m_primitive_count = primitive_count;
                    SET_FLAG(draw_call.m_option_flags, Flags::gbuffer_render_options_flags::_gbuffer_render_options_normals_bit, true);
                    if (g_output_velocity && !(GameState::GameTimeGlobals() && GameState::GameTimeGlobals()->paused))
                    {
                        SET_FLAG(draw_call.m_option_flags, Flags::gbuffer_render_options_flags::_gbuffer_render_options_velocity_bit, true);
                    }

                    result &= g_gbuffer->DrawStructure(*device, draw_call);
                }
                return result;
            }

            void DestroyGBuffer()
            {
                if (g_gbuffer)
                {
                    g_gbuffer->Destroy();
                }
                if (g_gbuffer_clear)
                {
                    g_gbuffer_clear->Destroy();
                }
                if (g_gbuffer_debug)
                {
                    g_gbuffer_debug->Destroy();
                }
            }

            void CreateGBuffer(IDirect3DDevice9& device, D3DPRESENT_PARAMETERS* params)
            {
                if (FAILED(device.TestCooperativeLevel()))
                {
                    return;
                }

                if (RasterizerConfig()->disable_render_targets || RasterizerConfig()->disable_alpha_render_targets)
                {
                    return;
                }

                struct
                {
                    byte minor_version;
                    byte major_version;
                } ps_version, vs_version;
                DX9::GetSMVersion(DX9::D3DCaps()->VertexShaderVersion, vs_version.major_version, vs_version.minor_version);
                DX9::GetSMVersion(DX9::D3DCaps()->PixelShaderVersion, ps_version.major_version, ps_version.minor_version);
                if ((vs_version.major_version < 3) || (ps_version.major_version < 3))
                {
                    return;
                }

                if (g_gbuffer)
                {
                    if (!g_gbuffer->Create(device, params->BackBufferWidth, params->BackBufferHeight))
                    {
                        DestroyGBuffer();
                        return;
                    }
                }
                if (g_gbuffer_clear)
                {
                    if (!g_gbuffer_clear->Create(device, params->BackBufferWidth, params->BackBufferHeight))
                    {
                        DestroyGBuffer();
                        return;
                    }
                }
                if (g_gbuffer_debug)
                {
                    if (!g_gbuffer_debug->Create(device, params->BackBufferWidth, params->BackBufferHeight))
                    {
                        DestroyGBuffer();
                        return;
                    }
                }
            }

            void Initialize()
            {
                c_settings_gbuffer::Register(Settings::Manager());

                if (CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
                {
                    return;
                }

                c_gbuffer_effect_factory::Get().Initialize();
                g_gbuffer = std::make_unique<c_gbuffer>(ShaderExtension::ExtensionsEnabled());
                g_gbuffer_clear = std::make_unique<c_gbuffer_rtclear_effect>(g_gbuffer->OutputMap());
                g_gbuffer_debug = std::make_unique<c_gbuffer_debug_effect>();

                Memory::WriteRelativeJmp(&Hook_RenderObjectList_GetObjectIndex, GET_FUNC_VPTR(RENDER_OBJECT_LIST_HOOK), true);
                Memory::WriteRelativeJmp(&Hook_RenderObjectList_ClearObjectIndex, GET_FUNC_VPTR(RENDER_OBJECT_LIST_END_HOOK), true);
                Memory::WriteRelativeJmp(&Hook_FirstPersonWeaponDraw_GetObjectIndex, GET_FUNC_VPTR(FIRST_PERSON_WEAPON_DRAW_HOOK), true);
                Memory::WriteRelativeJmp(&Hook_RenderObject_GetCurrentLOD, GET_FUNC_VPTR(RENDER_OBJECT_OBJECT_LOD_HOOK), true);

                Memory::WriteRelativeJmp(&Hook_CommandCameraSet, GET_FUNC_VPTR(COMMAND_CAMERA_SET_HOOK), true);
                Memory::WriteRelativeJmp(&Hook_CommandSwitchBSP, GET_FUNC_VPTR(COMMAND_SWITCH_BSP_HOOK), true);
                Memory::WriteRelativeJmp(&Hook_CommandGameSave, GET_FUNC_VPTR(COMMAND_GAME_SAVE_HOOK), true);

                Memory::WriteRelativeJmp(&Hook_RenderObjectsTransparent, GET_FUNC_VPTR(RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK), true);

                byte NOP = Enums::_x86_opcode_nop;
                byte* call_address;

                call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK));
                Memory::WriteRelativeCall(&DrawObject, call_address, true);
                Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));

                call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK));
                Memory::WriteRelativeCall(&DrawStructure, call_address, true);
                Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));
            }

            void Dispose()
            {
                c_settings_gbuffer::Unregister(Settings::Manager());

                if (g_gbuffer)
                {
                    g_gbuffer.reset();
                }
                if (g_gbuffer_clear)
                {
                    g_gbuffer_clear.reset();
                }
                if (g_gbuffer_debug)
                {
                    g_gbuffer_debug.reset();
                }
                c_gbuffer_effect_factory::Get().Dispose();
            }

            void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
            {
                if (g_gbuffer)
                {
                    CreateGBuffer(*device, params);
                }
            }

            void OnLostDevice()
            {
                if (g_gbuffer)
                {
                    DestroyGBuffer();
                }
            }

            void OnResetDevice(D3DPRESENT_PARAMETERS* params)
            {
                if (g_gbuffer)
                {
                    CreateGBuffer(*DX9::Direct3DDevice(), params);
                }
            }

            void Render() { }

            void Release()
            {
                if (g_gbuffer)
                {
                    DestroyGBuffer();
                }
            }

            void Update(real delta_time)
            {
                if (g_gbuffer)
                {
                    g_output_velocity = true;
                    g_gbuffer->Update();
                }
            }

            void Clear(IDirect3DDevice9* device)
            {
                if (g_gbuffer && g_gbuffer_clear)
                {
                    g_gbuffer_clear->Render(*device);
                }
            }

            void SetWorldViewProjection(const D3DMATRIX& matrix)
            {
                if (g_gbuffer && !Render::IsRenderingReflection())
                {
                    g_gbuffer->StoreWorldViewProj(matrix);
                }
            }

            void SkyPreProcess()
            {
                g_render_progress = _render_progress_sky;
            }

            void SkyPostProcess()
            {
                g_render_progress = _render_progress_none;
            }

            void ObjectsPreProcess()
            {
                g_render_progress = _render_progress_objects;
            }

            void ObjectsPostProcess()
            {
                g_render_progress = _render_progress_none;
            }

            void StructurePreProcess()
            {
                g_render_progress = _render_progress_structure;
            }

            void StructurePostProcess()
            {
                g_render_progress = _render_progress_none;
            }

            void UIPreProcess()
            {
                if (g_gbuffer && g_gbuffer_debug && g_debug_index != 0)
                {
                    g_gbuffer_debug->SetDebugTarget(g_debug_index);
                    g_gbuffer_debug->Render(*DX9::Direct3DDevice());
                }
            }

            bool& OutputTbn()
            {
                return g_output_tbn;
            }

            bool Available()
            {
                return g_gbuffer != nullptr && g_enabled;
            }

            bool& Enabled()
            {
                return g_enabled;
            }

            void SetRendered(const bool rendered)
            {
                g_rendered = rendered;
            }

            c_gbuffer& GetGBuffer()
            {
                return *g_gbuffer;
            }

            int16& DebugIndex()
            {
                return g_debug_index;
            }
        };
    };
};
#endif
