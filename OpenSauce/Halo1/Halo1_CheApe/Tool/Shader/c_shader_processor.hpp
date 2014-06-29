/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
namespace Yelo
{
	namespace Tool { namespace Shader
	{
		class c_shader_processor
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Disassembles shader byte code to string based assembly. </summary>
			///
			/// <param name="byte_code">		The shader byte code. </param>
			/// <param name="output_buffer">	[out] Buffer for output data. </param>
			static void Disassemble(const DWORD* byte_code, LPD3DXBUFFER& output_buffer);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Assembles string based shader assembly into shader byte code. </summary>
			///
			/// <param name="assembly">			[in] The assembly string. </param>
			/// <param name="output_buffer">	[out] Buffer for output data. </param>
			static void Assemble(std::string& assembly, LPD3DXBUFFER& output_buffer);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Removes the comment data from shader byte code by disassembling and reassembling it.
			/// </summary>
			///
			/// <param name="byte_code">		The byte code to process. </param>
			/// <param name="output_buffer">	[out] Buffer for output data. </param>
			static void RemoveComments(const DWORD* byte_code, LPD3DXBUFFER& output_buffer);
		};
	};};
};
#endif