/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/memory/memory_interface_base.hpp>

// We apply '__declspec(noinline)' to the memory functions to avoid
// any invalid inlining cases with the linker when it thinks a supplied
// address is actually in our DLL's code when it's not...it's a pointer to halo's
// code.
// Ex:
// 1>\memory\memoryinterface.cpp(149) : fatal error C1001: An internal error has occurred in the compiler.
// 1>(compiler file 'F:\SP\vctools\compiler\utc\src\P2\main.c[0x10BEFEAB:0x003A0073]', line 182)
// 1> To work around this problem, try simplifying or changing the program near the locations listed above.
// 
// This was from 'WriteMemory' before it was marked as noinline, and was trying
// to inline with a halo address

#if PLATFORM_TARGET == PLATFORM_TARGET_XBOX
	#define memcpy Std_memcpy
	#define memcpy_s(dst, dst_size, src, src_size) (dst==memcpy(dst, src, src_size))
#endif

namespace Yelo
{
	namespace Memory
	{
		__declspec(noinline) BOOL WriteMemory(void* address, const void* src, size_t size)
		{
			return address == memcpy(address, src, size);
		}

		void WriteMemory(void* address, int value, size_t count)
		{
			memset(address, value, count);
		}

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast
		__declspec(noinline) BOOL WriteMemory(void* address, void* value)
		{
			*CAST_PTR(uint32*, address) = CAST_PTR(uintptr_t, value);
			return true;
		}

		__declspec(noinline) void CreateHookRelativeCall(void* hook, void* hook_address, byte end)
		{
			struct relative_call_bytes : Opcode::s_call
			{
				byte End;
			}; BOOST_STATIC_ASSERT(sizeof(relative_call_bytes) == 6);
			// call near ....
			// retn\nop
			relative_call_bytes asm_bytes;

			asm_bytes.Op = Enums::_x86_opcode_call_near;

			// relative call to [hook]
			asm_bytes.Address =  CAST_PTR(intptr_t, hook) -
								(CAST_PTR(intptr_t, hook_address) + 
								 sizeof(Opcode::s_call));

			asm_bytes.End = end;

			WriteMemory(hook_address, &asm_bytes, sizeof(asm_bytes));
		}

		__declspec(noinline) uintptr_t WriteRelativeJmp(void* to_address, void* jmp_address, bool write_opcode)
		{
			byte real_opcode = Enums::_x86_opcode_jmp_near; // jmp [function]
			if (write_opcode)
				WriteMemory(jmp_address, &real_opcode, sizeof(real_opcode));

			uintptr_t original = CAST_PTR(intptr_t, jmp_address) + 
								*CAST_PTR(intptr_t*, CAST_PTR(uint32, jmp_address)+1) + 
								sizeof(Opcode::s_call);

			uintptr_t relative = CAST_PTR(intptr_t, to_address) - 
								(CAST_PTR(intptr_t, jmp_address) + 
								 sizeof(Opcode::s_call));
			WriteMemory(CAST_PTR(void*, CAST_PTR(uintptr_t, jmp_address) + 1), CAST_PTR(void*, relative));

			return original;
		}

		__declspec(noinline) uintptr_t WriteRelativeCall(void* to_address, void* call_address, bool write_opcode)
		{
			byte real_opcode = Enums::_x86_opcode_call_near; // call [function]
			if (write_opcode)
				WriteMemory(call_address, &real_opcode, sizeof(real_opcode));

			uintptr_t original = CAST_PTR(intptr_t, call_address) + 
								*CAST_PTR(intptr_t*, CAST_PTR(uint32, call_address)+1) + 
								sizeof(Opcode::s_call);

			uintptr_t relative = CAST_PTR(intptr_t, to_address) - 
								(CAST_PTR(intptr_t, call_address) + 
								 sizeof(Opcode::s_call));
			WriteMemory(CAST_PTR(void*, CAST_PTR(uintptr_t, call_address)+1 ), CAST_PTR(void*, relative));

			return original;
		}


		void Write(Opcode::s_call& call, void* address)
		{
			memcpy(address, &call, sizeof(call));
		}
		void WriteCall(void* call_buffer, void* address, const void* target)
		{
			Opcode::s_call* call = CAST_PTR(Opcode::s_call*, call_buffer);
			Opcode::s_call* call_address = CAST_PTR(Opcode::s_call*, address);

			call->Op = call_address->Op;					// copy the old
			call_address->Op = Enums::_x86_opcode_call_near;// set the new
			call->Address = call_address->Address;			// copy the old
			call_address->Address =							// set the new
				CAST_PTR(intptr_t, target) -				// cast the pointer to a number to perform math on
				(
					CAST_PTR(intptr_t, address) + sizeof(Opcode::s_call)
				);
		}

		void WriteCallRet(void* call_ret_buffer, void* address, const void* target)
		{	
			CAST_PTR(Opcode::s_call_ret*, call_ret_buffer)->Ret = 
				CAST_PTR(Opcode::s_call_ret*, address)->Ret;						// copy the old
			WriteCall(call_ret_buffer, address, target);
			CAST_PTR(Opcode::s_call_ret*, address)->Ret = Enums::_x86_opcode_ret;	// set the new
		}
		void WriteCallRet(void* call_ret_buffer, void* address, const void* target, const uint16 count)
		{
			CAST_PTR(Opcode::s_call_ret*, call_ret_buffer)->Ret = 
				CAST_PTR(Opcode::s_call_ret*, address)->Ret;						// copy the old
			WriteCall(call_ret_buffer, address, target);
			CAST_PTR(Opcode::s_call_ret*, address)->Ret = Enums::_x86_opcode_retn;	// set the new
			CAST_PTR(Opcode::s_call_ret*, call_ret_buffer)->Count = 
				CAST_PTR(Opcode::s_call_ret*, address)->Count;						// copy the old
			CAST_PTR(Opcode::s_call_ret*, address)->Count = (count * 4);			// set the new
		}

		void WriteRet(Opcode::s_call_ret& call, void* address)
		{
			memcpy(address, &call, sizeof(call) - sizeof(uint16)); // don't include the retn's count
		}
		void WriteRetn(Opcode::s_call_ret& call, void* address)
		{
			memcpy(address, &call, sizeof(call));
		}

		void WriteJmp(void* jmp_buffer, void* address, const void* target)
		{
			Opcode::s_call* jmp = CAST_PTR(Opcode::s_call*, jmp_buffer);
			Opcode::s_call* jmp_address = CAST_PTR(Opcode::s_call*, address);

			jmp->Op = jmp_address->Op;						// copy the old
			jmp_address->Op = Enums::_x86_opcode_jmp_near;	// set the new
			jmp->Address = jmp_address->Address;			// copy the old
			jmp_address->Address =							// set the new
				CAST_PTR(intptr_t, target) -				// cast the pointer to a number to perform math on
				(
					CAST_PTR(intptr_t, address) + sizeof(Opcode::s_call)
				);
		}

		void OverwriteJmp(void* jmp_buffer, void* address, const void* target)
		{
			Opcode::s_call* jmp_address = CAST_PTR(Opcode::s_call*, address);

			CAST_PTR(Opcode::s_call*, jmp_buffer)->Address = jmp_address->Address; // copy the old
			jmp_address->Address =				// set the new
				CAST_PTR(intptr_t, target) -	// cast the pointer to a number to perform math on
				(
					CAST_PTR(intptr_t, address) + sizeof(Opcode::s_call)
				);
		}
#pragma warning( pop )


		__declspec(noinline) void s_memory_exec_change_data::ApplyAsRelativeJmp()
		{
			this->Initialize();
			WriteRelativeJmp(ExecAddress, MemoryAddress, WriteOpcode);
		}

		__declspec(noinline) void s_memory_exec_change_data::ApplyAsRelativeCall()
		{
			this->Initialize();
			WriteRelativeCall(ExecAddress, MemoryAddress, WriteOpcode);
		}

		__declspec(noinline) void s_memory_exec_change_data::Undo()
		{
			if(this->IsInitialized)
				this->IsInitialized = 
				!(memcpy_s(MemoryAddress, UndoData.size(), UndoData.data(), UndoData.size()) == k_errnone);
		}

		__declspec(noinline) void s_memory_exec_change_data::Initialize()
		{
			if(!this->IsInitialized)
				this->IsInitialized = 
				memcpy_s(UndoData.data(), UndoData.size(), MemoryAddress, UndoData.size()) == k_errnone;
		}


		uintptr_t AlignValue(uintptr_t value, unsigned alignment_bit)
		{
			const uintptr_t alignment_mask = MASK(alignment_bit);

			if (value & alignment_mask)
				value = (value | alignment_mask) + 1;

			return value;
		}
	};
};

//////////////////////////////////////////////////////////////////////////
// CRC32 hacks
// http://stackoverflow.com/questions/2587766/how-is-a-crc32-checksum-calculated

template<uint32_t k_polynomial, uint32_t k_value>
struct crc_table_generator_round_piecemeal
{
	enum {
		value=(k_value  & 1)
			? (k_value >> 1) ^ k_polynomial
			:  k_value >> 1
	};
};
template<uint32_t k_polynomial, uint32_t k_round_index>
struct crc_table_generator_round
{
	enum {
		value = crc_table_generator_round_piecemeal<k_polynomial,
					crc_table_generator_round_piecemeal<k_polynomial,
						crc_table_generator_round_piecemeal<k_polynomial,
							crc_table_generator_round_piecemeal<k_polynomial,
								crc_table_generator_round_piecemeal<k_polynomial,
									crc_table_generator_round_piecemeal<k_polynomial,
										crc_table_generator_round_piecemeal<k_polynomial,
											crc_table_generator_round_piecemeal<k_polynomial,k_round_index>::value
										>::value
									>::value
								>::value
							>::value
						>::value
					>::value
				>::value,
	};
};

// TODO: my mind is a blank right now on how to use variadics to expand something like:
// crc_table_generator<CRC32_POLYNOMIAL, CRC_TABLE_SIZE>::value
// where [value] is an std::array<uint, CRC_TABLE_SIZE>. so, we have the hack seen below

#define CRC_TABLE_DEFINE_ROUND(z, n, polynomial)	\
	crc_table_generator_round<polynomial, n##0+0>::value, crc_table_generator_round<polynomial, n##0+1>::value, \
	crc_table_generator_round<polynomial, n##0+2>::value, crc_table_generator_round<polynomial, n##0+3>::value, \
	crc_table_generator_round<polynomial, n##0+4>::value, crc_table_generator_round<polynomial, n##0+5>::value, \
	crc_table_generator_round<polynomial, n##0+6>::value, crc_table_generator_round<polynomial, n##0+7>::value, \
	crc_table_generator_round<polynomial, n##0+8>::value, crc_table_generator_round<polynomial, n##0+9>::value
#define CRC32_TABLE_DEFINE_END_ROUNDS(polynomial)	\
	crc_table_generator_round<polynomial, 250+0>::value, crc_table_generator_round<polynomial, 250+1>::value,	\
	crc_table_generator_round<polynomial, 250+2>::value, crc_table_generator_round<polynomial, 250+3>::value,	\
	crc_table_generator_round<polynomial, 250+4>::value, crc_table_generator_round<polynomial, 250+5>::value,

#define CRC_TABLE_SIZE 256
#define CRC32_POLYNOMIAL 0xEDB88320UL
static std::array<uint32_t, CRC_TABLE_SIZE> g_crc32_table = {
	// 000 to 250
	BOOST_PP_ENUM(25, CRC_TABLE_DEFINE_ROUND, CRC32_POLYNOMIAL),
	// 250 to 255
	CRC32_TABLE_DEFINE_END_ROUNDS(CRC32_POLYNOMIAL)
};
#undef CRC_TABLE_DEFINE_ROUND
#undef CRC32_TABLE_DEFINE_END_ROUNDS

namespace Yelo
{
	namespace Memory
	{
		uint32 CRC(uint32& crc_reference, const void* buffer, int32 size)
		{
			auto p = CAST_PTR(const byte*, buffer);

			while (size--)
			{
				uint32 a = (crc_reference >> 8) & 0x00FFFFFFL;
				uint32 b = g_crc32_table[( (int32) crc_reference ^ *p++) & 0xFF];
				crc_reference = a^b;
			}

			return crc_reference;
		}
	};
};

#if PLATFORM_TARGET == PLATFORM_TARGET_XBOX
	#undef memcpy
	#undef memcpy_s
#endif