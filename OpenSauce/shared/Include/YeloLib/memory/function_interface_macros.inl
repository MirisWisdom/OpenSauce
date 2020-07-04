/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// t_function_process
#define GET_HOOK_PROCESS(name)					function_process_##name##_definition

#define DEFINE_HOOK_PROCESS(name, address, ...)	Yelo::Memory::t_function_process<address, __VA_ARGS__> GET_HOOK_PROCESS(##name##)
//////////////////////////////////////////////////////////////////////////


#define GET_HOOK_BLOCK_PREPROCESS(name)		function_preprocess_##name##_definition			
#define GET_HOOK_BLOCK_POSTPROCESS(name)	function_postprocess_##name##_definition

#define DEFINE_HOOK_BLOCK_PREPROCESS(name)	Yelo::Memory::function_process_proc GET_HOOK_BLOCK_PREPROCESS(##name##)[]
#define DEFINE_HOOK_BLOCK_POSTPROCESS(name)	Yelo::Memory::function_process_proc GET_HOOK_BLOCK_POSTPROCESS(##name##)[]


//////////////////////////////////////////////////////////////////////////
// t_function_process_block
#define GET_HOOK_BLOCK_PROCESS(name)		function_process_block_##name##_definition

#define DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address, preprocess, pre_count, postprcess, post_count, ...)	\
	Yelo::Memory::t_function_process_block<address,															\
	preprocess,	pre_count,																					\
	postprcess, post_count,																					\
	__VA_ARGS__>																							\
	GET_HOOK_BLOCK_PROCESS(##name##)

#define DEFINE_HOOK_BLOCK_PROCESS_NONE(name, address, ...)																\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		nullptr, 0,																										\
		nullptr, 0, __VA_ARGS__)
#define DEFINE_HOOK_BLOCK_PROCESS_PRE(name, address, preprocess_name, ...)												\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##),	NUMBEROF( GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##) ),		\
		nullptr, 0, __VA_ARGS__)
#define DEFINE_HOOK_BLOCK_PROCESS_POST(name, address, postprcess_name, ...)												\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		nullptr, 0,																										\
		GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##), NUMBEROF( GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##) ),	\
		__VA_ARGS__)
#define DEFINE_HOOK_BLOCK_PROCESS(name, address, preprocess_name, postprcess_name, ...)									\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##),	NUMBEROF( GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##) ),		\
		GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##),NUMBEROF( GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##) ),	\
		__VA_ARGS__)
//////////////////////////////////////////////////////////////////////////


#define WRITE_HOOK_BLOCK_PROCESS(name, address)\
	Memory::WriteRelativeCall(GET_HOOK_BLOCK_PROCESS(##name##).FunctionHook, CAST_PTR(void*,address))