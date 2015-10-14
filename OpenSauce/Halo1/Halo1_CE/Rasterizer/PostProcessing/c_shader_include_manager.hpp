/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>Handles loading shader includes.</summary>
            ///
            /// <remarks>Used to control where shader includes are loaded from. This is necessary when loading
            ///     an ASCII shader that is stored in memory since it has no path to use as a start point.</remarks>
            class c_shader_include_manager final : public ID3DXInclude
            {
                const char* m_include_path;
            public:
                explicit c_shader_include_manager(const char* path);

            private:
                HRESULT API_FUNC Open(D3DXINCLUDE_TYPE IncludeType,
                    LPCSTR pFileName,
                    LPCVOID pParentData,
                    LPCVOID* ppData,
                    UINT* pBytes) override;
                HRESULT API_FUNC Close(LPCVOID pData) override;
            };
        }
    }
}
#endif
