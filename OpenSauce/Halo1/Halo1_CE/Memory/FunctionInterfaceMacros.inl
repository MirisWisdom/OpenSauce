/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
		NULL, 0,																										\
		NULL, 0, __VA_ARGS__)
#define DEFINE_HOOK_BLOCK_PROCESS_PRE(name, address, preprocess_name, ...)												\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##),	NUMBEROF( GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##) ),		\
		NULL, 0, __VA_ARGS__)
#define DEFINE_HOOK_BLOCK_PROCESS_POST(name, address, postprcess_name, ...)												\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		NULL, 0,																										\
		GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##), NUMBEROF( GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##) ),	\
		__VA_ARGS__)
#define DEFINE_HOOK_BLOCK_PROCESS(name, address, preprocess_name, postprcess_name, ...)									\
	DEFINE_HOOK_BLOCK_PROCESS_IMPL(name, address,																		\
		GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##),	NUMBEROF( GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##) ),		\
		GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##),NUMBEROF( GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##) ),	\
		__VA_ARGS__)

// Old macro definition:
// #define DEFINE_HOOK_BLOCK_PROCESS(name, address, preprocess_name, postprcess_name, ...)							\
// 	Yelo::Memory::t_function_process_block<address,																\
// 	GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##),	NUMBEROF( GET_HOOK_BLOCK_PREPROCESS(##preprocess_name##) ),	\
// 	GET_HOOK_BLOCK_POSTPROCESS(##postprcess_name##), NUMBEROF( GET_HOOK_BLOCK_PREPROCESS(##postprcess_name##) ),\
// 	__VA_ARGS__>																								\
// 	GET_HOOK_BLOCK_PROCESS(##name##)
//////////////////////////////////////////////////////////////////////////


#define WRITE_HOOK_BLOCK_PROCESS(name, address)\
	Memory::WriteRelativeCall(GET_HOOK_BLOCK_PROCESS(##name##).FunctionHook, CAST_PTR(void*,address))