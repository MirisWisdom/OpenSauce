/*
    BlamLib: .NET SDK for the Blam Engine

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
#pragma once

using namespace System;

namespace LowLevel {

	public mcpp_class StructBitConverter abstract sealed
	{
	mcpp_public

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static array<mcpp_byte>^ GetBytes(mcpp_ref(T) value_ref);

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static void GetBytes(array<mcpp_byte>^ buffer, mcpp_int start_index, mcpp_ref(T) value_ref);

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static T ToValue(array<mcpp_byte>^ buffer, mcpp_int start_index);

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static void ToValue(array<mcpp_byte>^ buffer, mcpp_int start_index, mcpp_out(T) value_ref);
	};

	public mcpp_class ByteSwap abstract sealed
	{
	mcpp_public
		/// <summary>
		/// Swaps a single-precision number and returns the value
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static mcpp_real SwapSingle(mcpp_real dword);

		/// <summary>
		/// Swaps a single-precision number by reference
		/// </summary>
		/// <param name="dword"></param>
		static void SwapSingle(mcpp_ref(mcpp_real) dword);
	};
}