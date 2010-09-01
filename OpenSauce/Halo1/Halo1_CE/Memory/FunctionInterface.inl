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
template<const void* TAddress, bool TIsDisabled> const void* t_function_process<TAddress, TIsDisabled>::
	k_address = TAddress;
template<const void* TAddress, bool TIsDisabled> bool t_function_process<TAddress, TIsDisabled>::
	g_is_disabled = TIsDisabled;

// Have to define the function outside the template declaration due to our use of API_FUNC_NAKED
template<const void* TAddress, bool TIsDisabled> API_FUNC_NAKED void t_function_process<TAddress, TIsDisabled>::
	FunctionHook()
{
	__asm {
		mov		al, g_is_disabled
		test	al, al
		jnz		is_disabled
		call	k_address
is_disabled:
		retn
	}
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// t_function_process_block
template<const void* TAddress, 
	const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,
	const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,
	bool TIsDisabled>
const void* t_function_process_block<TAddress, TBlockPreprocess, TBlockPreprocessCount, TBlockPostprocess, TBlockPostprocessCount, TIsDisabled>::
	k_address = TAddress;

template<const void* TAddress, 
	const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,
	const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,
	bool TIsDisabled>
const function_process_proc* t_function_process_block<TAddress, TBlockPreprocess, TBlockPreprocessCount, TBlockPostprocess, TBlockPostprocessCount, TIsDisabled>::
	k_block_preprocess = TBlockPreprocess;

template<const void* TAddress, 
	const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,
	const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,
	bool TIsDisabled>
const function_process_proc* t_function_process_block<TAddress, TBlockPreprocess, TBlockPreprocessCount, TBlockPostprocess, TBlockPostprocessCount, TIsDisabled>::
	k_block_postprocess = TBlockPostprocess;

template<const void* TAddress, 
	const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,
	const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,
	bool TIsDisabled>
bool t_function_process_block<TAddress, TBlockPreprocess, TBlockPreprocessCount, TBlockPostprocess, TBlockPostprocessCount, TIsDisabled>::
	g_is_disabled = TIsDisabled;

// Have to define the function outside the template declaration due to our use of API_FUNC_NAKED
template<const void* TAddress, 
	const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,
	const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,
	bool TIsDisabled>
API_FUNC_NAKED void t_function_process_block<TAddress, TBlockPreprocess, TBlockPreprocessCount, TBlockPostprocess, TBlockPostprocessCount, TIsDisabled>::
	FunctionHook()
{
	__asm {
		pushad
		pushfd

		mov		edx, k_block_preprocess
		test	edx, edx
		jz		function_preprocess_end
		xor		ecx, ecx
function_preprocess:
		mov		eax, [edx+ecx*4]
		test	eax, eax
		jz		function_preprocess_is_null
		push	ecx	// playing it safe: ecx *could* be modified in our code without being preserved (silly compiler)
		push	edx
		call	eax
		pop		edx
		pop		ecx
function_preprocess_is_null:
		inc		ecx
		cmp		ecx, k_block_preprocess_count
		jl		function_preprocess
function_preprocess_end:

		mov		al, g_is_disabled
		test	al, al
		jnz		is_disabled
		popfd
		popad
		call	k_address
		pushad
		pushfd
is_disabled:

		mov		edx, k_block_postprocess
		test	edx, edx
		jz		function_postprocess_end
		xor		ecx, ecx
function_postprocess:
		mov		eax, [edx+ecx*4]
		test	eax, eax
		jz		function_postprocess_is_null
		push	ecx	// playing it safe: ecx *could* be modified in our code without being preserved (silly compiler)
		push	edx
		call	eax
		pop		edx
		pop		ecx
function_postprocess_is_null:
		inc		ecx
		cmp		ecx, k_block_postprocess_count
		jl		function_postprocess
function_postprocess_end:

		popfd
		popad
		retn
	}
}
//////////////////////////////////////////////////////////////////////////