/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_shader_include_manager.hpp"

#if !PLATFORM_IS_DEDI

#include "Common/FileIO.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            c_shader_include_manager::c_shader_include_manager(const char* path)
                : m_include_path(path) {}

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>Loads a file included in a shader into memory.</summary>
            ///
            /// <remarks>The Open function creates an absolute path to the included file, then loads it into
            ///     memory for the compiler to use.</remarks>
            ///
            /// <param name="IncludeType">The type of file being included.</param>
            /// <param name="pFileName">  The relative path of the file being included.</param>
            /// <param name="pParentData">The shader file that includes the requested file.</param>
            /// <param name="ppData">     [in,out] If non-null, the output data buffer.</param>
            /// <param name="pBytes">     [in,out] If non-null, the size of the included file.</param>
            ///
            /// <returns>Returns S_OK if the file exists and has been loaded, otherwise E_FAIL.</returns>
            HRESULT API_FUNC c_shader_include_manager::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
            {
                // create the file location path
                char location[MAX_PATH];
                location[0] = 0;
                strcat_s(location, MAX_PATH, m_include_path);
                strcat_s(location, MAX_PATH, pFileName);

                FileIO::s_file_info info;
                if (FileIO::OpenFile(info, location) != Enums::_file_io_open_error_none)
                {
                    return E_FAIL;
                }

                auto success = E_FAIL;
                do
                {
                    if (ReadFileToMemory(info, CAST_QUAL(LPVOID*, ppData)) != Enums::_file_io_read_error_none)
                    {
                        break;
                    }
                    *pBytes = info.file_size;

                    success = S_OK;
                } while (false);

                CloseFile(info);

                return success;
            }

            HRESULT API_FUNC c_shader_include_manager::Close(LPCVOID pData)
            {
                delete [] pData;
                return S_OK;
            }
        }
    }
}
#endif
