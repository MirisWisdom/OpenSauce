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
#pragma once

//High resolution timer code originally from dreamincode member Aphex19
//http://www.dreamincode.net/forums/showtopic48006.htm

class HRTimer 
{
private:
	LARGE_INTEGER mqStart;
	LARGE_INTEGER mqEnd;
	LARGE_INTEGER mqFreq;

	bool bTimerRunning; PAD24;
	PAD32;
public:
	HRTimer();
	~HRTimer() {}

	void Start();
	void Stop();
	double GetTimeInSeconds();
	double GetTimeInMilliseconds();
	double GetTimeInMicroseconds();
	double GetTimeInNanoseconds();
};