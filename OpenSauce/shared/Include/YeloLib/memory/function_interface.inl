/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// t_function_process
#define T_FUNCTION_PROCESS_SIGNATURE(...)	\
	template<const void* TAddress, bool TIsDisabled> __VA_ARGS__ t_function_process<TAddress, TIsDisabled>

T_FUNCTION_PROCESS_SIGNATURE(const void*)::
	k_address = TAddress;
T_FUNCTION_PROCESS_SIGNATURE(bool)::
	g_is_disabled = TIsDisabled;

#undef T_FUNCTION_PROCESS_SIGNATURE
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// t_function_process_block
#define T_FUNCTION_PROCESS_BLOCK_SIGNATURE(...)															\
	template<const void* TAddress,																		\
		const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,				\
		const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,			\
		bool TIsDisabled>																				\
	__VA_ARGS__ t_function_process_block<TAddress, TBlockPreprocess, TBlockPreprocessCount, TBlockPostprocess, TBlockPostprocessCount, TIsDisabled>

T_FUNCTION_PROCESS_BLOCK_SIGNATURE(const void*)::
	k_address = TAddress;

T_FUNCTION_PROCESS_BLOCK_SIGNATURE(bool)::
	g_is_disabled = TIsDisabled;

#undef T_FUNCTION_PROCESS_BLOCK_SIGNATURE
//////////////////////////////////////////////////////////////////////////