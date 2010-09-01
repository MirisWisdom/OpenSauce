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

DEFINE_HOOK_PROCESS(test1, CAST_PTR(void*, 0x1111));
DEFINE_HOOK_PROCESS(test2, CAST_PTR(void*, 0x2222));
DEFINE_HOOK_PROCESS(test3, CAST_PTR(void*, 0x3333));

DEFINE_HOOK_BLOCK_PREPROCESS(test_block) = {
	GET_HOOK_PROCESS(test1).FunctionHook,
	GET_HOOK_PROCESS(test2).FunctionHook,
	GET_HOOK_PROCESS(test3).FunctionHook,
};

DEFINE_HOOK_PROCESS(test4, CAST_PTR(void*, 0x4444));
DEFINE_HOOK_PROCESS(test5, CAST_PTR(void*, 0x5555));
DEFINE_HOOK_PROCESS(test6, CAST_PTR(void*, 0x6666));

DEFINE_HOOK_BLOCK_POSTPROCESS(test_block) = {
	GET_HOOK_PROCESS(test4).FunctionHook,
	GET_HOOK_PROCESS(test5).FunctionHook,
	GET_HOOK_PROCESS(test6).FunctionHook,
};

DEFINE_HOOK_BLOCK_PROCESS_PRE(test_pre, CAST_PTR(void*, 0xDDDD), test_block);
DEFINE_HOOK_BLOCK_PROCESS_POST(test_post, CAST_PTR(void*, 0xEEEE), test_block);
DEFINE_HOOK_BLOCK_PROCESS(test, CAST_PTR(void*, 0xFFFF), test_block, test_block);

const function_process_proc test_function1 = GET_HOOK_BLOCK_PROCESS(test_pre).FunctionHook;
const function_process_proc test_function2 = GET_HOOK_BLOCK_PROCESS(test_post).FunctionHook;
const function_process_proc test_function3 = GET_HOOK_BLOCK_PROCESS(test).FunctionHook;