/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Debug
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the crash handler. </summary>
		///-------------------------------------------------------------------------------------------------
		void DumpInitialize();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Clean up the crash handler and uninstall crasrpt. </summary>
		///-------------------------------------------------------------------------------------------------
		void DumpDispose();
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates the crash handler to reset the freeze dump timer if needed. </summary>
		/// <param name="delta">	The time that has passed since the last update. </param>
		///-------------------------------------------------------------------------------------------------
		void Update(real delta);
	};
};