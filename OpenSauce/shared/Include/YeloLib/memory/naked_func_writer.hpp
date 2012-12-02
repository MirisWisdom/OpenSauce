/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/memory/memory_interface_base.hpp>

// Uncomment, or define this before the header, if the debug build isn't incremental
//#define NAKED_FUNC_WRITER_NO_INCREMETNAL_BUILD_WORKAROUND

#define NAKED_FUNC_WRITER_EMIT_EOF_CHAR 0xCC
#define NAKED_FUNC_WRITER_EMIT_EOF_WORD 0xCCCCCCCC
#define NAKED_FUNC_WRITER_EMIT_EOF				\
	__asm _emit NAKED_FUNC_WRITER_EMIT_EOF_CHAR	\
	__asm _emit NAKED_FUNC_WRITER_EMIT_EOF_CHAR	\
	__asm _emit NAKED_FUNC_WRITER_EMIT_EOF_CHAR	\
	__asm _emit NAKED_FUNC_WRITER_EMIT_EOF_CHAR

#define NAKED_FUNC_WRITER_ASM_BEGIN(func_name)	\
	API_FUNC_NAKED void __cdecl func_name ()	\
	{											\
		__asm {
#define NAKED_FUNC_WRITER_ASM_END()				\
		}										\
		NAKED_FUNC_WRITER_EMIT_EOF				\
	}

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_naked_func_writer_max_asm_length = 256
		};
	};

	namespace Memory
	{
		/*
		Usage:

		Define the asm to a naked func:
			NAKED_FUNC_WRITER_ASM_BEGIN(lea_eax_some_array)
			lea eax, some_array
			nop													NAKED_FUNC_WRITER_ASM_END();

		Somewhere in your code, write the naked func to an address:
			Memory::c_naked_func_writer<lea_eax_some_array>::
				Write( K_SOME_ADDRESS1 );

		Use the nop_count overload when you have to fill the bytes of pre-existing opcodes or such
			Memory::c_naked_func_writer<lea_eax_some_array>::
				Write( K_SOME_ADDRESS2, 2 );
		*/
		template< void (__cdecl* k_naked_func)() >
		class c_naked_func_writer
		{
			// NOTE: MSDN says "/INCREMENTAL is implied when /DEBUG is specified"
			// So we have to jump on the function trampoline to get to the actual bytes address
			static byte* GetNakedFuncBytes()
			{
#if _DEBUG && !defined(NAKED_FUNC_WRITER_NO_INCREMETNAL_BUILD_WORKAROUND)
				static byte* g_bytes = NULL;

				if(g_bytes == NULL)
				{
					byte* bytes = CAST_PTR(byte*, k_naked_func);

					// If the first byte starts a jmp instruction, we assume the linker made a trampoline
					// Add the 'jmp' offset (can be negative) plus the size of a 'jmp' instruction
					// (since the offset is relative to the instruction -after- the 'jmp')
					if( bytes[0] == Enums::_x86_opcode_jmp_near )
						bytes += *CAST_PTR(int32*, bytes+1) + sizeof(Opcode::s_call);

					g_bytes = bytes;
				}
				return g_bytes;
#else
				return CAST_PTR(byte*, k_naked_func);
#endif
			}
			// Determine the assembly code length of [k_naked_func]
			static size_t DetermineAsmLength()
			{
				static size_t g_length = 0;

				if(g_length == 0)
				{
					const byte* bytes = GetNakedFuncBytes();
					for(size_t x = 0; x < Enums::k_naked_func_writer_max_asm_length; x++)
						if( *CAST_PTR(const int32*, bytes+x) == NAKED_FUNC_WRITER_EMIT_EOF_WORD )
						{
							g_length = x;
							break;
						}
				}

				return g_length;
			}
		public:
			// Write the asm code of [k_naked_func] to [address]
			static void Write(void* address)
			{
				size_t asm_length = DetermineAsmLength();

				byte* bytes = GetNakedFuncBytes();
				byte* addr = CAST_PTR(byte*, address);

				for(; asm_length != 0; asm_length--, addr++, bytes++)
					*addr = *bytes;
			}

			// Write the asm code of [k_naked_func] to [address]
			// Then write a no-op code [nop_count] times
			// Only use this if k_naked_func is written multiple times and needs filler bytes in one or more places
			// Otherwise add the 'nop' instructions to the k_naked_func's asm directly
			static void Write(void* address, size_t nop_count)
			{
				size_t asm_length = DetermineAsmLength();

				byte* bytes = GetNakedFuncBytes();
				byte* addr = CAST_PTR(byte*, address);

				for(; asm_length != 0; asm_length--, addr++, bytes++)
					*addr = *bytes;
				for(; nop_count != 0; nop_count--, addr++)
					*addr = Enums::_x86_opcode_nop;
			}
		};
	};
};