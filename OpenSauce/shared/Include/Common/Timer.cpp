/*
    Yelo: Open Sauce SDK

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
#include <Common/Precompile.hpp>
#include <Common/Timer.hpp>

//High resolution timer code originally from dreamincode member Aphex19
//http://www.dreamincode.net/forums/showtopic48006.htm

HRTimer::HRTimer(){
	QueryPerformanceFrequency(&mqFreq);
}
void HRTimer::Start(){
	QueryPerformanceCounter(&mqStart); bTimerRunning = true;
}

void HRTimer::Stop() {
	QueryPerformanceCounter(&mqEnd); bTimerRunning = false;
}

double HRTimer::GetTimeInSeconds() 
{
	LARGE_INTEGER mqCurrent;
	if(bTimerRunning)
		QueryPerformanceCounter(&mqCurrent);
	else
		mqCurrent = mqEnd;
	return (mqCurrent.QuadPart - mqStart.QuadPart)/(double)mqFreq.QuadPart;
}

double HRTimer::GetTimeInMilliseconds() 
{
	LARGE_INTEGER mqCurrent;
	if(bTimerRunning)
		QueryPerformanceCounter(&mqCurrent);
	else
		mqCurrent = mqEnd;
	return (1.0e3*(mqCurrent.QuadPart - mqStart.QuadPart))/mqFreq.QuadPart;
}

double HRTimer::GetTimeInMicroseconds() 
{
	LARGE_INTEGER mqCurrent;
	if(bTimerRunning)
		QueryPerformanceCounter(&mqCurrent);
	else
		mqCurrent = mqEnd;
	return (1.0e6*(mqCurrent.QuadPart - mqStart.QuadPart))/mqFreq.QuadPart;
}

double HRTimer::GetTimeInNanoseconds() 
{
	LARGE_INTEGER mqCurrent;
	if(bTimerRunning)
		QueryPerformanceCounter(&mqCurrent);
	else
		mqCurrent = mqEnd;
	return (1.0e9*(mqCurrent.QuadPart - mqStart.QuadPart))/mqFreq.QuadPart;
}