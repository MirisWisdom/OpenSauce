/*
    Kornner Studios: Shared Code

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
/*!
 * Microsoft Visual C++ compiler warning disabling
 */
#pragma once

//#define _CRT_SECURE_NO_DEPRECATE

// 'reinterpret_cast' : pointer truncation
#pragma warning(disable: 4311)

// 'type cast' : conversion from '<type>' to '<type> *' of greater size
#pragma warning(disable: 4312)

// macro redefinition
// used to get rid of the fucking warning in CSeries/CppMacros.h
// for the interface macro (complains in file objbase.h, a
// system include)
#pragma warning(disable: 4005)