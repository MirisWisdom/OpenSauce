/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Render
    {
        struct s_quad;
        class i_render_device;

        class i_quad_pass_handler
        {
        public:
            virtual ~i_quad_pass_handler() {}

            virtual void PrePass() = 0;
            virtual void PostPass() = 0;
        };

        class c_quad_renderer final
        {
            static void Draw(IDirect3DDevice9& render_device, i_quad_pass_handler& pass, ID3DXEffect& effect, const s_quad& quad);
        };
    }
}
#endif