/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Scripting
	{
		struct s_hs_compile_globals
		{
			bool initialized;
			PAD24;
			rsize_t compiled_source_size;
			char* compiled_source;

			struct {
				size_t offset;
				char* base_address;
				rsize_t size;
			}string_constant;

			bool has_compiled_source; // a source file has been compiled
			PAD24;

			cstring error; // is either null, points to error_buffer, or constant (non-DMA) string
			size_t error_offset;
			string256 error_buffer;

			bool compiled_source_is_dma; // compiled_source is dynamically allocated
			bool compiled_for_runtime_evaluate;
			bool cant_compile_thread_blocking; // in the current context, thread blocking statements can't be used
			bool cant_compile_variable_sets; // in the current context, the 'set' function can't be used
			bool postprocessing;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_compile_globals) == 0x12C );


		s_hs_compile_globals* HsCompileGlobals();
	};
};