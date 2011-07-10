/*
    OpenSauce: HaloWars Data Interop

    Copyright (C)  Kornner Studios (http://kornner.com)

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

#include <xtl.h>
#include <xboxmath.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>

#include <shell/Platform.hpp>
#include <cseries/MacrosCpp.hpp>
#include <cseries/BasicLogWriter.hpp>
#include <xbox/xenon/GPRLR.hpp>

extern c_basic_log_writer g_log_writer;