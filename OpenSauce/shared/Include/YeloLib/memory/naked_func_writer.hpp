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
			template< void (__cdecl* )(), void (__cdecl* )() >
			friend class c_naked_func_writer_with_undo;

			// NOTE: MSDN says "/INCREMENTAL is implied when /DEBUG is specified"
			// So we have to jump on the function trampoline to get to the actual bytes address
			static byte* GetNakedFuncBytes()
			{
#if _DEBUG && !defined(NAKED_FUNC_WRITER_NO_INCREMETNAL_BUILD_WORKAROUND)
				static byte* g_bytes = nullptr;

				if (g_bytes == nullptr)
				{
					byte* bytes = CAST_PTR(byte*, k_naked_func);

					// If the first byte starts a jmp instruction, we assume the linker made a trampoline
					// Add the 'jmp' offset (can be negative) plus the size of a 'jmp' instruction
					// (since the offset is relative to the instruction -after- the 'jmp')
					if( bytes[0] == Enums::_x86_opcode_jmp_near )
						bytes += *CAST_PTR(intptr_t*, bytes+1) + sizeof(Opcode::s_call);

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
						if( *CAST_PTR(const uint32*, bytes+x) == NAKED_FUNC_WRITER_EMIT_EOF_WORD )
						{
							g_length = x;
							break;
						}
				}

				return g_length;
			}
		public:
			// Write the asm code of [k_naked_func] to [address]
			static size_t Write(void* address)
			{
				size_t asm_length = DetermineAsmLength();

				byte* bytes = GetNakedFuncBytes();
				byte* addr = CAST_PTR(byte*, address);

				WriteMemory(addr, bytes, asm_length);

				return asm_length;
			}

			// Write the asm code of [k_naked_func] to [address]
			// Then write a no-op code [nop_count] times
			// Only use this if k_naked_func is written multiple times and needs filler bytes in one or more places
			// Otherwise add the 'nop' instructions to the k_naked_func's asm directly
			static size_t Write(void* address, size_t nop_count)
			{
				size_t asm_length = DetermineAsmLength();

				byte* bytes = GetNakedFuncBytes();
				byte* addr = CAST_PTR(byte*, address);

				WriteMemory(addr, bytes, asm_length);
				WriteMemory(addr+asm_length, Enums::_x86_opcode_nop, nop_count);

				return asm_length + nop_count;
			}
		};

		// [k_naked_undo_func] - original asm bytes
		// [k_naked_func] - new asm bytes
		template< void (__cdecl* k_naked_undo_func)(), void (__cdecl* k_naked_func)() >
		class c_naked_func_writer_with_undo
		{
			typedef c_naked_func_writer<k_naked_undo_func> old_func_t;
			typedef c_naked_func_writer<k_naked_func> new_func_t;

#ifdef API_DEBUG
			static void AssertAsmLengths(cstring asm_mismatch_msg)
			{
				size_t old_func_length = old_func_t::DetermineAsmLength();
				size_t new_func_length = new_func_t::DetermineAsmLength();
				YELO_ASSERT_DISPLAY( old_func_length==new_func_length, "%s", asm_mismatch_msg );
			}
			static void Verify(const void* code_addr, cstring asm_mismatch_msg)
			{
				DebugRunOnce( AssertAsmLengths(asm_mismatch_msg) );

				size_t old_func_length = old_func_t::DetermineAsmLength();
				byte* old_func_bytes = old_func_t::GetNakedFuncBytes();
				YELO_ASSERT_DISPLAY( memcmp(code_addr, old_func_bytes, old_func_length)==0, "%s", asm_mismatch_msg );
			}
#endif
		public:
			// Write the asm code of [k_naked_func] to [address]
			static size_t Write(void* address DebugOnly(, cstring asm_mismatch_msg))
			{
				DebugOnly( Verify(address, asm_mismatch_msg) );

				return new_func_t::Write(address);
			}
			// Write the asm code of [k_naked_undo_func] to [address]
			static size_t Undo(void* address)
			{
				return old_func_t::Write(address);
			}
		};
	};
};