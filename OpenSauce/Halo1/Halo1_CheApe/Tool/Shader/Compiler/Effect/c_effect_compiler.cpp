/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_compiler.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Console.hpp"
#include "Rasterizer/DX9/DX9.hpp"

using namespace boost::filesystem;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		bool c_effect_compiler::CompileEffect(const path& file_location, const std::string& target, LPD3DXEFFECT& effect_out)
		{
			// compile the shader
			D3DXMACRO macros[4] = {
					{ "pc", "1" },
					{ "TGT", target.c_str() },
					{ NULL, NULL },
			};

			LPD3DXBUFFER error_buffer = NULL;
			HRESULT hr = E_FAIL;
			
			Console::ColorPrint(0x000A, "Compiling...");

			// attempt to compile shader
			hr = D3DXCreateEffectFromFile(
				Rasterizer::DX9::Direct3DDevice(),
				file_location.string().c_str(),
				&macros[0], NULL,
				D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_NO_PRESHADER,
				NULL,
				&effect_out,
				&error_buffer);

			if (FAILED(hr))
			{
				Console::ColorPrint(0x00C, "failed\r\n");

				if (error_buffer)
				{
					char* error_text = (char*)error_buffer->GetBufferPointer();
					Console::ColorPrintF(0x00C, "%s\r\n", error_text);
				}
				safe_release(error_buffer);
				safe_release(effect_out);
				return false;
			}
			safe_release(error_buffer);

			Console::ColorPrint(0x000A, "done\r\n");

			return true;
		}
	};};};
};
#endif