/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_texture.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL

#include "Rasterizer/DX9/DX9.hpp"

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		bool c_packed_file_processor_texture::ProcessElement(c_packed_file::s_element_editor& element, const boost::filesystem::path& file_path)
		{
			c_auto_release<IDirect3DTexture9> texture;
			c_scope_end_execute release_device([]() { Rasterizer::DX9::Dispose(); });

			Rasterizer::DX9::Initialize();
			HRESULT hr;

			// Attempt to create a texture from the specified file
			hr = D3DXCreateTextureFromFile(Rasterizer::DX9::Direct3DDevice()
				, file_path.string().c_str()
				, &texture);

			// If texture creation failed return false
			if(FAILED(hr))
			{
				puts("Failed to create the texture");
				return false;
			}
			
			// Convert the texture into a dds file
			c_auto_release<ID3DXBuffer> dds_buffer;
			hr = D3DXSaveTextureToFileInMemory(&dds_buffer, D3DXIFF_DDS, texture, nullptr);

			if(FAILED(hr))
			{
				puts("Failed to create dds file for the texture");
				return false;
			}

			// Copy the texture data to the element
			element.element_size = dds_buffer->GetBufferSize();
			element.source_data = new char[element.element_size];
			errno_t error = memcpy_s(element.source_data, 
				element.element_size, 
				dds_buffer->GetBufferPointer(), 
				element.element_size);

			if(error != 0)
			{
				return false;
			}

			return true;
		}
	};};
};
#endif