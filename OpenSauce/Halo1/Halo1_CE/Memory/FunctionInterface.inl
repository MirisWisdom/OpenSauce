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